#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Server.hpp"
#include "Message.hpp"
#include <iterator>
#include <iostream>
#include <string>
#include <map>
#include <set>

class Server;

class Channel
{
  private:
    static Server *serverPtr;

    std::string name;
    std::string topic;
    std::map<std::string, int> members; // 0 : member, 1 : operator
    std::string key;
    int memberLimit;
    bool MODE_T;
    bool MODE_I;
    std::set<std::string> invitedNickname;

  public:
    Channel();
    ~Channel();
    Channel(const std::string &name);


    void setMembers(const std::string &nickname, const int operator_check);
    static void setServerPtr(Server *server_ptr);
    bool isInvited(std::string nickname);
    void inviteNewMember(std::string nickname);
    bool isMember(const std::string& nickname);
    void deleteMemberFromInvitedList(std::string nickname);
    bool isNoOperator();


    const std::string &getName() const;
    std::map<std::string, int> &getMembers();
    std::string &getTopic();
    int &getLimit();
    std::string &getKey();

    bool &getMODE_T();
    bool &getMODE_I();

    void setMODE_T(bool mode);
    void setMODE_I(bool mode);

    void broadcasting(std::string fromNickname, Message &message);
    void partAll();
};

#endif
