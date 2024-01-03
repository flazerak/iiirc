#ifndef SERVER_HPP
#define SERVER_HPP

#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <string>
#include <sys/event.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <list>
#include <map>

#include "../inc/Channel.hpp"
#include "../inc/Client.hpp"
#include "../inc/Message.hpp"
#include "../inc/Command.hpp"

#define LISTEN_BACKLOG_NUM 5

class Client;

class Server
{
  private:
    int portNumber;
    std::string password;
    int serverSocket;
    int kque;

    std::map<std::string, int> nicknameToSocketFd;
    std::map<int, Client> socketFdToClient;
    std::map<std::string, Channel> channel;

    Server();

  public:
    Server(int portNumber, std::string password);
    ~Server();

    int getKque() const;

    std::map<std::string, int>& getNicknameToSocketFd();
    std::map<int, Client>& getSocketFdToClient();
    std::map<std::string, Channel>& getChannel();
    std::string& getPassWord();

    void openSocket();
    void init();
    void run();
    void handleNewConnection(int sockFd);
    void handleExistingConnection(int sockFd, struct kevent event);
    void handleExistingConnection_send_client(int fd);
    bool isConnected(struct kevent event);
    void terminateConnection(int fd);
    void execCommand(Message message);
    bool password_checker(const std::string &str);

    Client &getClientByNickname(const std::string &nickname);

};

#endif
