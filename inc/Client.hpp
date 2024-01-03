#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <vector>
#include <string>
#include "Server.hpp"
#include "Message.hpp"

#define IN_BUFFER_SIZE 1024

class Server;

class Client
{
  private:
    static Server *serverPtr;

    int socket;
    std::string username;
    std::string nickname;
    std::string outBuffer;
    std::string inBuffer;
    std::string ipaddress;
    bool isAuthenticated;

  public:
    static void setServerPtr(Server *server_ptr);

    Client();
    Client(int socket);
    ~Client();
    void setNickname(std::string nickname);
    void setIsAuthenticated(bool auth);
    void setUsername(std::string username);

    std::string &getUsername();
    std::string &getNickname();
    bool &getIsAuthenticated();
    int getSocket() const;
    void setIpaddress(const std::string &ipaddress);
    const std::string &getIpaddress() const;

    std::vector<Message> extractMessageFromBuffer();
    std::vector<Message> readData();
    void sendMessage(std::string &string);
    void sendMessage(Message &message);

    void writeOff();
    void writeOn();

    void sendData();

    std::string makePrefix();
};

#endif