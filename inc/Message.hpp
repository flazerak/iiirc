#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>

class Message
{
  private:
    int socket;
    std::vector<std::string> argument;
    std::string command;
    std::string prefix;

  public:
    Message(int socket, std::string msg);
    Message(int socket,
            const std::string &prefix,
            const std::string &command,
            const std::vector<std::string> &argument);
    ~Message();

    const std::string &getCommand() const;
    const std::string &getPrefix() const;
    void setPrefix(const std::string &prefix);
    std::vector<std::string> &getArg();
    int getSocket();
    void parse(std::string msg);
};

std::vector<std::string> split(std::string input, char delimiter);

#endif