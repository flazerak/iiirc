#include "../inc/Client.hpp"

Server *Client::serverPtr;

Client::Client()
{
}

Client::Client(int socket)
{
    this->socket = socket;
    nickname = "";
    username = "";
    inBuffer = "";
    outBuffer = "";
    isAuthenticated = false;
}

void Client::setUsername(std::string username)
{
    this->username = username;
}

void Client::setNickname(std::string nickname)
{
    this->nickname = nickname;
}

Client::~Client()
{}

std::string &Client::getNickname()
{
    return nickname;
}

std::string &Client::getUsername()
{
    return username;
}

#define RED "\e[0;31m"
#define NC "\e[0m"
#define YELLOW "\033[0;33m"

std::vector<Message> Client::readData()
{
    std::vector<Message> ret;

    char buffer[IN_BUFFER_SIZE];

    bzero(buffer, IN_BUFFER_SIZE);

    int received = recv(this->socket, buffer, sizeof(buffer), 0);
    if (received < 0)
        throw std::runtime_error("recv() error");

    inBuffer.append(buffer);

    std::cout << RED << inBuffer << NC << std::endl;

    ret = this->extractMessageFromBuffer();

    return ret;
}

std::vector<Message> Client::extractMessageFromBuffer()
{
    std::vector<std::string> messages;

    std::string::size_type pos = inBuffer.find("\r\n");
    while (pos != std::string::npos)
    {
        messages.push_back(inBuffer.substr(0, pos));
        inBuffer.erase(0, pos + 2);
        pos = inBuffer.find("\r\n");
    }

    std::vector<Message> ret;

    for (size_t i = 0; i < messages.size(); i++)
        ret.push_back(Message(this->socket, messages[i]));

    return ret;
}

void Client::sendMessage(Message &message)
{
    std::string toSend = "";
    toSend += message.getPrefix();
    toSend += " ";
    toSend += message.getCommand();


    for (size_t i = 0; i < message.getArg().size(); i++)
    {
        toSend += " " + message.getArg()[i];
    }
    toSend += "\r\n";

    this->outBuffer += toSend;

    writeOn();
}

void Client::sendMessage(std::string &string)
{
    std::string toSend = string;

    toSend += "\r\n";

    this->outBuffer += toSend;

    writeOn();
}

void Client::writeOn()
{
    struct kevent event;

    EV_SET(&event, this->socket, EVFILT_WRITE, EV_ENABLE, NULL, 0, NULL);
    kevent(this->serverPtr->getKque(), &event, 1, NULL, 0, NULL);
}

void Client::writeOff()
{
    struct kevent event;

    EV_SET(&event, this->socket, EVFILT_WRITE, EV_DISABLE, NULL, 0, NULL);
    kevent(this->serverPtr->getKque(), &event, 1, NULL, 0, NULL);
}

void Client::sendData()
{
    ssize_t sentLength;

    sentLength = send(socket, outBuffer.c_str(), outBuffer.size(), MSG_DONTWAIT);

    if (sentLength < 0) // send error
        throw std::runtime_error("send error");

    else if (sentLength != (ssize_t)outBuffer.size())
        this->outBuffer.erase(0, sentLength);
    
    else 
    {
        std::cout << YELLOW << outBuffer << NC << std::endl;
        this->outBuffer.clear();
        writeOff();
    }
}

void Client::setServerPtr(Server *server_ptr)
{
    serverPtr = server_ptr;
}

int Client::getSocket() const
{
    return socket;
}

const std::string &Client::getIpaddress() const
{
    return ipaddress;
}

void Client::setIpaddress(const std::string &ipaddress)
{
    Client::ipaddress = ipaddress;
}

std::string Client::makePrefix()
{
    std::string ret = "";

    ret = ":" + nickname + "!" + username + "@" + ipaddress;

    return ret;
}


void Client::setIsAuthenticated(bool auth)
{
    this->isAuthenticated = auth;
}

bool &Client::getIsAuthenticated()
{
    return isAuthenticated;
}