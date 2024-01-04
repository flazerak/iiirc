#include "../inc/Server.hpp"

Server::Server() {}

bool Server::password_checker(const std::string &str)
{
    //password validation function
    if (str.empty())
    {
        return (true);
    }
    size_t i = 0;
    while (i < str.length())
    {
        if ((std::isspace(str[i])) || !std::isprint(str[i]))
            return (true);
        i++;
    }
    return (false);
}

Server::Server(int portNumber, std::string password)
{
    this->portNumber = portNumber;
    this->password = password;

    // Validation for password and portNumber
    if (password_checker(this->password))
    {
        std::cout << "input password invaild" << std::endl;
        std::exit(1);
    }

    if (!(portNumber >= 0 && 65535 >= portNumber))
    {
        std::cout << "input port invaild" << std::endl;
        std::exit(1);
    }
    this->kque = kqueue(); 
    if (this->kque == -1)
        throw std::runtime_error("kqueue error");

    // Setting pointers in Client class to this server instance
    Client::setServerPtr(this);
    Channel::setServerPtr(this);
}

void Server::openSocket()
{
    sockaddr_in serverAddr;

    this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    // Creating a socket
    if (this->serverSocket == -1)
        throw std::runtime_error("socket error");

    // Setting up server address structure
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr =
        htonl(INADDR_ANY);
    serverAddr.sin_port = htons(this->portNumber);

    // Setting socket options
    int option = 1;
    setsockopt(this->serverSocket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    // Binding the socket to the address
    if (bind(this->serverSocket, reinterpret_cast<const sockaddr *>(&serverAddr), sizeof(serverAddr)) == -1)
        throw std::runtime_error("bind error");
    // Listening for incoming connections
    listen(this->serverSocket, LISTEN_BACKLOG_NUM); 
                                                    
}

void Server::init()
{
    struct kevent event;

    // Adding the server socket to the kqueue for monitoring read events
    EV_SET(&event, this->serverSocket, EVFILT_READ, EV_ADD, 0, 0, NULL);
    if (kevent(this->kque, &event, 1, NULL, 0, NULL) == -1)
        throw std::runtime_error("kevent attach error");
}

Server::~Server()
{
    // Closing server socket and kqueue
    close(serverSocket);
    close(kque);
}

void Server::run()
{
    while (true)
    {
        struct kevent events[32];
        int triggered = kevent(this->kque, NULL, 0, events, 32, NULL);
        try
        {   int i = 0;
            if (triggered == -1)
                throw std::runtime_error("kevent event error");
            while ( i < triggered)
            {
                if ((int)events[i].ident ==
                    this->serverSocket) 
                {
                    handleNewConnection(events[i].ident);
                }
                else
                { 
                    if (events[i].filter == EVFILT_READ)
                    {
                        handleExistingConnection(events[i].ident, events[i]);
                    }
                    else if (events[i].filter == EVFILT_WRITE)
                    {
                        
                        handleExistingConnection_send_client(events[i].ident);
                    }
                }
                i++;
            }
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
    }
}

void Server::handleNewConnection(int sockFd)
{
    // Handling a new connection, accepting the connection and configuring kqueue for the new socket
    // Also creating a new Client object for the connection
    // Adding the client to the socketFdToClient map
    // Outputting a message to indicate a new connection

    struct sockaddr_in client_addr;
    struct kevent event;
    int client_addr_size = sizeof(client_addr);

    int newFd = accept(sockFd, (sockaddr *)&client_addr, (socklen_t *)&client_addr_size);

    if (newFd == -1)
        throw std::runtime_error("accept error");
    fcntl(newFd, F_SETFL, O_NONBLOCK); 
    EV_SET(&event, newFd, EVFILT_READ, EV_ADD, NULL, 0, NULL);
    kevent(this->kque, &event, 1, NULL, 0, NULL);

    EV_SET(&event, newFd, EVFILT_WRITE, EV_ADD | EV_DISABLE, NULL, 0, NULL);
    kevent(this->kque, &event, 1, NULL, 0, NULL);

    Client client(newFd);
    client.setIpaddress(inet_ntoa(client_addr.sin_addr));
    socketFdToClient.insert(std::make_pair(newFd, client));

    std::cout << newFd << " : new connect!" << std::endl;
}

void Server::handleExistingConnection_send_client(int fd)
{
    // Handling an existing connection for sending data to the client
    socketFdToClient[fd].sendData();
}

// #define RED "\e[0;31m"
// #define NC "\e[0m"

void Server::handleExistingConnection(int sockFd, struct kevent event)
{
    // Handling an existing connection for reading data from the client
    // Checking if the connection is still valid
    // Outputting information about the client and processing received messages

    if (isConnected(event) == false)
    {
        this->terminateConnection(sockFd);
        return;
    }

    std::cout << "--------" << sockFd << ": "
              << socketFdToClient[sockFd].getNickname() << "--------"
              << std::endl;

    std::vector<Message> messages = this->socketFdToClient[sockFd].readData();
    size_t i = 0;
    while ( i < messages.size())
    {
        try
        {
            execCommand(messages[i]);
        }
        catch (std::exception &e)
        {
            std::cout << e.what() << std::endl;
            break;
        }
        i++;
    }
}

bool Server::isConnected(struct kevent event)
{
    // Checking if a connection is still valid based on event flags

    if (event.flags & EV_EOF)
    {
        return false;
    }
    return true;
}

void Server::terminateConnection(int fd)
{
    // Terminating a connection, removing the client from associated channels
    // Deleting the client from maps and closing the socket

    std::map<std::string, Channel>::iterator iterCh = this->channel.begin();
    std::string &nickname = socketFdToClient[fd].getNickname();
    std::vector<std::string> channelNamestoDelete;
    while ( iterCh != channel.end())
    {
        Channel &mini_channel = iterCh->second;
        std::map<std::string, int> &members = mini_channel.getMembers();
        if (members.find(nickname) != members.end())
        {
            members.erase(members.find(nickname));
            if (mini_channel.isNoOperator())
                channelNamestoDelete.push_back(iterCh->first);
        }
        iterCh++;
    }
    size_t i = 0;
    while ( i < channelNamestoDelete.size())
    {
        this->channel[channelNamestoDelete[i]].partAll();
        this->channel.erase(channelNamestoDelete[i]);
        i++;
    }

   
    struct kevent temp_event;
    EV_SET(&temp_event, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    kevent(kque, &temp_event, 1, NULL, 0, NULL);
    close(fd);
    std::cout << fd << " : close!" << std::endl;

   
    this->socketFdToClient.erase(fd);
    this->nicknameToSocketFd.erase(nickname);
}

void Server::execCommand(Message message)
{
    // Executing a command received from a client based on the message type
    // Commands include PASS, NICK, USER, PRIVMSG, PING, JOIN, PART, KICK, TOPIC, INVITE, MODE, QUIT

    Command &command = Command::getInstance(*this);
    Client &client = socketFdToClient[message.getSocket()];

    if (message.getCommand() == "PASS")
        command.pass(message);
    else if (client.getIsAuthenticated() == false)
        terminateConnection(message.getSocket());
    else if (message.getCommand() == "NICK")
        command.nick(message);
    else if (client.getNickname() == "")
    {
        std::cout<<"No Nick!"<<std::endl;
        return ;
    }
    else if (message.getCommand() == "USER")
        command.user(message);
    else if (client.getUsername() == "" || client.getIpaddress() == "")
    {
        std::cout<<"No User!"<<std::endl;
        return ;
    }
    else if (message.getCommand() == "PRIVMSG")
        command.privmsg(message);
    else if (message.getCommand() == "PING")
        command.ping(message);
    else if (message.getCommand() == "JOIN")
        command.join(message);
    else if (message.getCommand() == "PART")
        command.part(message);
    else if (message.getCommand() == "KICK")
        command.kick(message);
    else if (message.getCommand() == "TOPIC")
        command.topic(message);
    else if (message.getCommand() == "INVITE")
        command.invite(message);
    else if (message.getCommand() == "MODE")
        command.mode(message);
    else if (message.getCommand() == "QUIT")
        command.quit(message);
}

int Server::getKque() const { return kque; }

Client &Server::getClientByNickname(const std::string &nickname)
{
    // Getting a client object by nickname
    std::map<std::string, int>::iterator iter =
        nicknameToSocketFd.find(nickname);

    return socketFdToClient[iter->second];
}

std::map<std::string, int> &Server::getNicknameToSocketFd()
{
    // Getting the nicknameToSocketFd map
    return nicknameToSocketFd;
}

std::map<int, Client> &Server::getSocketFdToClient()
{
    // Getting the socketFdToClient map
    return socketFdToClient;
}

std::map<std::string, Channel> &Server::getChannel() { return channel; }

std::string &Server::getPassWord() { return password; }