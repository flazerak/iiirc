#ifndef COMMAND_HPP
# define COMMAND_HPP

#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include "../inc/Client.hpp"
#include "../inc/Server.hpp"
#include "../inc/Message.hpp"
#include "../inc/Channel.hpp"
#include <cstdlib>

class Client;
class Server;
class Channel;


class Command
{
private:
    Server* serverInstance;
    static Command commandInstance;

public:

    static Command& getInstance(Server& server);
    std::map<int, Client> &getServerSocketFdToClient(void);
    std::map<std::string,int> &getServernicknameToSocketFd(void); 
    std::string &getServerPassWord(void);
    std::map<std::string, Channel> &getServerChannel(void);

    const std::string &getClientUsername(Message &message);
    const std::string &getClientNickname(Message &message);

    
    void nick(Message &message);
    void pass(Message &message);
    void privmsg(Message &message);
    void user(Message &message);
    void pong(Message &message);
    void ping(Message &message);
    int joinChannelNameCheck(std::string name);
    void join(Message &message);
    void part(Message &message);
    void topic(Message &message);
    void kick(Message &message);
    void mode(Message &message);
    void invite(Message &message);
    void join_success(Message &message, std::string channelName);
    void exit(Message &message);
    void quit(Message &message);

    bool setMode(Message &message, Channel &channel);
    

    
    void password_incorrect_464(Message &message);
    void channel_mode_324(Message &message, Channel channel);
    void duplicate_check_433(Message &message);
    void command_empty_argument_461(Message &message);
    void error_no_nick_given_431(Message &message);
    void no_such_channel_403(Message &message);
    void no_operator_channel_482(Message &message);
    void no_member_channel_442(Message &message);
    void no_users_channel_441(Message &message);
    void no_topic_channel_331(Message &message);
    void user_already_channel_443(Message &message);
    void characters_not_allowed_432(Message &message);
    void success_invite_341(Message &message);
    void yes_topic_channel_332(Message &message, std::string topic);
    void no_such_server_402(std::string channel_name, Message &message);
    void no_nick_member_401(std::string no_nick, Message &message);
    void no_reciver_411(Message &message);
    void no_member_channel_404(std::string channel, Message &message);
    void bad_channel_mask_476(Message &message);
    void no_exist_message_412(Message &message);
    void cannot_join_i_473(Message &message);
    void cannot_join_l_471(Message &message);
    void cannot_join_k_475(Message &message);
    void join_RPL_ENDOFNAMES_366(Message &message, std::string channelName);
    void join_RPL_NAMREPLY_353(Message &message, std::string channelName);
};

#endif