#include "../inc/Command.hpp"

Command Command::commandInstance;

Command& Command::getInstance(Server& server)
{
    commandInstance.serverInstance = &server; 
    return commandInstance;
}

std::map<std::string, int> &Command::getServernicknameToSocketFd(void)
{
    return serverInstance->getNicknameToSocketFd();
}

std::map<int, Client> &Command::getServerSocketFdToClient(void)
{
    return serverInstance->getSocketFdToClient();
}

std::map<std::string, Channel> &Command::getServerChannel(void)
{
    return serverInstance->getChannel();
}

std::string &Command::getServerPassWord(void)
{
    return serverInstance->getPassWord();
}

const std::string &Command::getClientNickname(Message &message)
{
    return serverInstance->getSocketFdToClient()[message.getSocket()]
        .getNickname();
}

const std::string &Command::getClientUsername(Message &message)
{
    return serverInstance->getSocketFdToClient()[message.getSocket()]
        .getUsername();
}

void Command::password_incorrect_464(Message &message)
{
    Client &client = serverInstance->getSocketFdToClient()[message.getSocket()];
    std::string nickname = client.getNickname();
    std::string error_message = ":irc.local 464 " + nickname +
                                " :Password Incorrect, Command disconnected";
    client.sendMessage(error_message);
}

void Command::error_no_nick_given_431(Message &message)
{
    std::string error_message =
        ":irc.local 431 :No nickname given";
    Client &client = serverInstance->getSocketFdToClient()[message.getSocket()];
    client.sendMessage(error_message);
}

void Command::command_empty_argument_461(Message &message)
{
    std::string error_message = ":irc.local 461 " + getClientNickname(message) +
                                " " + message.getCommand() +
                                " :Not enough parameters";
    Client &client = serverInstance->getSocketFdToClient()[message.getSocket()];
    client.sendMessage(error_message);
}

void Command::duplicate_check_433(Message &message)
{
    std::string error_message = ":irc_local 433 " + message.getArg()[0] + " " +
                                message.getArg()[0] +
                                " :Nickname is already in use";
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        error_message);
}

void Command::no_such_channel_403(Message &message)
{
    std::string error_message = ":irc.local 403 " + getClientNickname(message) +
                                " " + message.getArg()[0] + " :No such channel";
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        error_message);
}

void Command::cannot_join_i_473(Message &message)
{
    std::string error_message = ":irc.local 473 " + getClientNickname(message) +
                                " " + message.getArg()[0] +
                                " :Cannot join channel (+i)";
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        error_message);
}

void Command::cannot_join_k_475(Message &message)
{
    std::string error_message = ":irc.local 475 " + getClientNickname(message) +
                                " " + message.getArg()[0] +
                                " :Cannot join channel (+k)";
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        error_message);
}

void Command::cannot_join_l_471(Message &message)
{
    std::string error_message = ":irc.local 471 " + getClientNickname(message) +
                                " " + message.getArg()[0] +
                                " :Cannot join channel (+l)";
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        error_message);
}

void Command::bad_channel_mask_476(Message &message)
{
    std::string error_message = ":irc.local 476 " + getClientNickname(message) +
                                " " + message.getArg()[0] +
                                " :Bad Channel Mask";
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        error_message);
}

void Command::no_member_channel_442(Message &message)
{
    std::string error_message = ":irc.local 442 " + getClientNickname(message) +
                                " " + message.getArg()[0] +
                                " :You're not on that channel";
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        error_message);
}

void Command::no_operator_channel_482(Message &message)
{
    std::string channelName;
    if (message.getCommand() == "INVITE")
        channelName = message.getArg()[1];
    else
        channelName = message.getArg()[0];
    std::string error_message = ":irc.local 482 " + getClientNickname(message) +
                                " " + channelName +
                                " :You're not channel operator";
    Client &client = serverInstance->getSocketFdToClient()[message.getSocket()];
    client.sendMessage(error_message);
}

void Command::no_users_channel_441(Message &message)
{
    std::string error_message = ":irc_local 441 " + getClientNickname(message) +
                                " " + message.getArg()[1] + " " +
                                message.getArg()[0] +
                                " :They aren't on that channel";

    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        error_message);
}

void Command::user_already_channel_443(Message &message)
{
    std::string error_message = ":irc.local 443 " + getClientNickname(message) +
                                " " + message.getArg()[0] + " " +
                                message.getArg()[1] + " :is already on channel";
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        error_message);
}

void Command::channel_mode_324(Message &message, Channel channel)
{
    std::string error_message = ":irc.local 324 " + getClientNickname(message) +
                                " " + channel.getName();

    if (channel.getMODE_I())
        error_message += " +i";
    if (channel.getMODE_T())
        error_message += " +t";
    if (channel.getKey() != "")
        error_message += " +k";
    if (channel.getLimit() != 0)
    {
        std::stringstream ss;
        ss << channel.getLimit();
        std::string tmp = ss.str();
        error_message += " +l" + tmp;
    }
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        error_message);
}

void Command::characters_not_allowed_432(Message &message)
{
    std::string error_message = ":irc.local 432 " + getClientNickname(message) +
                                " " + message.getArg()[0] +
                                " :Erroneous nickname";
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        error_message);
}

void Command::no_topic_channel_331(Message &message)
{
    std::string reply = ":irc.local 331 " + getClientNickname(message) + " " +
                        message.getArg()[0] + " :No topic is set";
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        reply);
}

void Command::yes_topic_channel_332(Message &message, std::string topic)
{
    std::string reply = ":irc.local 332 " + getClientNickname(message) + " " +
                        message.getArg()[0] + " " + topic;
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        reply);
}

void Command::success_invite_341(Message &message)
{
    std::string success_message =
        ":irc.local 341 " + getClientNickname(message) + " " +
        message.getArg()[0] + " " + message.getArg()[1];
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        success_message);
}

void Command::no_such_server_402(std::string channelName, Message &message)
{
    std::string error_message = ":irc.local 402 " + getClientNickname(message) +
                                " " + channelName + " :No such server";
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        error_message);
}

void Command::no_nick_member_401(std::string no_nick, Message &message)
{
    std::string error_message =
        ":irc.local 401 " + no_nick + " :No such nick/channel";
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        error_message);
}

void Command::no_reciver_411(Message &message)
{
    std::string error_message = ":irc.local 411 " + getClientNickname(message) +
                                " " + ":No recipient given " +
                                message.getCommand();
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        error_message);
}

void Command::no_exist_message_412(Message &message)
{
    std::string error_message = ":irc.local 412 " + getClientNickname(message) +
                                " " + ":No text to send";
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        error_message);
}

void Command::no_member_channel_404(std::string channel, Message &message)
{
    std::string error_message = ":irc.local 404 " + getClientNickname(message) +
                                " " + channel + " :Cannot send to channel";
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        error_message);
}

void Command::join_RPL_NAMREPLY_353(Message &message, std::string channelName)
{
    std::map<std::string, Channel> &channel = getServerChannel();
    Channel &mini_channel = channel[channelName];

    std::map<std::string, int> &member = mini_channel.getMembers();

    std::map<std::string, int>::iterator iterMb = member.begin();
    std::string user_list;

    while (iterMb != member.end())
    {
        if (iterMb->second == 1)
            user_list += "@" + iterMb->first + " ";
        else
            user_list += iterMb->first + " ";
        iterMb++;
    }

    std::string error_message = ":irc.local 353 " + getClientNickname(message) +
                                " = " + message.getArg()[0] + " :" + user_list;
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
        error_message);
}

void Command::join_RPL_ENDOFNAMES_366(Message &message, std::string channelName)
{
    std::string ret = ":irc.local 366 " + getClientNickname(message) + " " +
                      channelName + " :End of /NAMES list.";
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(ret);
}

//----command--//

void Command::pass(Message &message)
{
    std::string password = getServerPassWord();

    int count = message.getArg().size();
    if (count == 0 || message.getArg()[0].empty() ||
        password != message.getArg()[0])
    {
        password_incorrect_464(message);
        serverInstance->terminateConnection(message.getSocket());
        throw std::runtime_error("pass error");
    }
    Client &client = serverInstance->getSocketFdToClient()[message.getSocket()];
    client.setIsAuthenticated(true);
}

void Command::nick(Message &message)
{
    if (message.getArg().size() < 1)
    {
        error_no_nick_given_431(message);
        return;
    }

    int socket = message.getSocket();
    std::map<int, Client> &socketFdToClient = getServerSocketFdToClient();
    Client &client = socketFdToClient[socket];

    std::string newNickname = message.getArg()[0];
    std::map<int, Client>::iterator iter = socketFdToClient.find(socket);
    std::map<std::string, int> &nicknameToSocketFd =
        getServernicknameToSocketFd();
    std::map<std::string, int>::iterator iterNicknameToSocket =
        nicknameToSocketFd.find(newNickname);

    if (iterNicknameToSocket == nicknameToSocketFd.end()) 
    {
       
        
        int i = 0;
        std::string nick = message.getArg()[0];
        while (nick[i++])
            if (nick[i] == '\r' || nick[i] == '\n' || nick[i] == ' ')
                return characters_not_allowed_432(message);
        if (iter->second.getNickname() != "") 
        {
            Message messageToSend(client.getSocket(), client.makePrefix() + " NICK " + newNickname);
            client.sendMessage(messageToSend);
            nicknameToSocketFd.erase(iter->second.getNickname());
            iter->second.setNickname(newNickname);
            nicknameToSocketFd[newNickname] = message.getSocket();
        }
        else
        {
            iter->second.setNickname(newNickname);
            nicknameToSocketFd[newNickname] = message.getSocket();
        }
    }
    else
        return duplicate_check_433(message); 
}

void Command::user(Message &message)
{
    if (message.getArg().size() < 4 || message.getArg()[0].empty())
    {
        command_empty_argument_461(message);
        return;
    }
    std::map<int, Client> &socketFdToClient = getServerSocketFdToClient();
    Client &client = socketFdToClient[message.getSocket()];
    client.setUsername(message.getArg()[0]);
    Message messageToSend(client.getSocket(), ":irc.local 001 " + 
        client.getNickname() + " :Welcome to the Internet Relay Network");
    client.sendMessage(messageToSend);
}


void Command::privmsg(Message &message)
{
    
    if (message.getArg().size() < 1 || message.getArg()[0].empty())
    {
        no_reciver_411(message);
        return;
    }

    if (message.getArg().size() < 2 || message.getArg()[1].empty())
    {
        no_exist_message_412(message);
        return;
    }
    std::string receiver = message.getArg()[0];
    std::vector<std::string> textToBeSent = message.getArg();

    std::map<int, Client> &socketFdToClient = getServerSocketFdToClient();
    std::map<std::string, int> &nicknameToSocketFd =
        getServernicknameToSocketFd();
    Client &fromClient = socketFdToClient[message.getSocket()];
    std::string fromNickname = fromClient.getNickname();

    if (receiver[0] == '#' || receiver[0] == '&')
    {
        std::map<std::string, Channel> &channel = getServerChannel();
        
        std::map<std::string, Channel>::iterator iter = channel.find(receiver);

    
        if (iter == channel.end())
        {
            no_nick_member_401(receiver, message);
            return;
        }
        
        if (!iter->second.isMember(fromNickname))
        {
            no_member_channel_404(iter->first, message);
            return;
        }

        iter->second.broadcasting(fromClient.makePrefix(), message);
    }
    else 
    {
       
        std::map<std::string, int>::iterator iter =
            nicknameToSocketFd.find(receiver);
        if (iter == nicknameToSocketFd.end())
        {
            no_nick_member_401(receiver, message);
            return;
        }

        int socketToSend = nicknameToSocketFd[receiver];
        Client &clientToSend = socketFdToClient[socketToSend];
        Message messageToBeSent = Message(socketToSend, fromClient.makePrefix(),
                                          "PRIVMSG", textToBeSent);
        clientToSend.sendMessage(messageToBeSent);
    }
}

void Command::ping(Message &message)
{
    if (message.getArg().size() < 1 || message.getArg()[0].empty())
    {
        command_empty_argument_461(message);
        return;
    }
    pong(message);
}

void Command::pong(Message &message)
{
    std::vector<std::string> textToBeSent;
    for (size_t i = 0; i < message.getArg().size(); i++)
        textToBeSent.push_back(message.getArg()[i]);

    std::map<int, Client> &socketFdToClient = getServerSocketFdToClient();
    std::string prefix = ":irc.local";

    Message toSendMessage(message.getSocket(), prefix, "PONG", textToBeSent);

    socketFdToClient[message.getSocket()].sendMessage(toSendMessage);
}

int Command::joinChannelNameCheck(std::string name)
{
    if ((name[0] != '&' && name[0] != '#') || name.size() > 50)
        return false;
    int index = 0;
    while (name[index])
    {
        if (isspace(name[index] != 0) || name[index] == 7 || name[index] == ',')
            return false;
        index++;
    }
    return true;
}

void Command::join(Message &message)
{
    if (message.getArg().size() < 1 || message.getArg()[0].empty())
    {
        command_empty_argument_461(message);
        return;
    }

    std::map<std::string, Channel> &channel = getServerChannel();
    std::map<int, Client> &socketFdToClient = getServerSocketFdToClient();

    Client &fromClient = socketFdToClient[message.getSocket()];
    std::string channelName = message.getArg()[0];
    std::map<std::string, Channel>::iterator iter = channel.find(channelName);

    if (iter == channel.end())
    {
        if (joinChannelNameCheck(channelName) == false)
        {
            bad_channel_mask_476(message);
            return;
        }
        Channel newChannel(channelName);
        newChannel.setMembers(
            socketFdToClient[message.getSocket()].getNickname(), 1);
        channel.insert(make_pair(channelName, newChannel));

        std::map<std::string, Channel>::iterator iter2 =
            channel.find(channelName);
        Client &clientToJoin = socketFdToClient[message.getSocket()];
        Message reply = message;
        reply.setPrefix(fromClient.makePrefix());

        clientToJoin.sendMessage(reply);
        iter2->second.broadcasting(fromClient.makePrefix(), reply);

        yes_topic_channel_332(message, "");
        join_RPL_NAMREPLY_353(message, channelName);
        join_RPL_ENDOFNAMES_366(message, channelName);
    }
    else
    {
        Client &clientToJoin = socketFdToClient[message.getSocket()];
        
        if (iter->second.getMODE_I() &&
            !iter->second.isInvited(clientToJoin.getNickname()))
        {
            cannot_join_i_473(message);
            return;
        }
        if (iter->second.getKey() != "")
        {
            if (message.getArg()[1] != iter->second.getKey())
            {
                cannot_join_k_475(message);
                return;
            }
        }
        if (iter->second.getLimit() != 0)
        {
            if (iter->second.getMembers().size() >=
                (size_t)iter->second.getLimit())
            {
                cannot_join_l_471(message);
                return;
            }
        }
        iter->second.setMembers(
            socketFdToClient[message.getSocket()].getNickname(), 0);
        
        iter->second.deleteMemberFromInvitedList(
            socketFdToClient[message.getSocket()].getNickname());

        
        Message reply = message;
        reply.setPrefix(fromClient.makePrefix());
        clientToJoin.sendMessage(reply);
        iter->second.broadcasting(fromClient.makePrefix(), reply);

        yes_topic_channel_332(message, iter->second.getTopic());
        join_RPL_NAMREPLY_353(message, channelName);
        join_RPL_ENDOFNAMES_366(message, channelName);
    }
}


void Command::part(Message &message)
{
    if (message.getArg().size() < 1 || message.getArg()[0].empty())
    {
        command_empty_argument_461(message);
        return;
    }

    std::map<int, Client> &socketFdToClient = getServerSocketFdToClient();
    std::map<std::string, Channel> &channel = getServerChannel();
    std::string channelName = message.getArg()[0];
    std::map<std::string, Channel>::iterator iterCh = channel.find(channelName);
    if (iterCh == channel.end())
    {
        no_such_channel_403(message);
        return;
    }

    Client &fromClient = socketFdToClient[message.getSocket()];
    std::string nickname = fromClient.getNickname();
    std::map<std::string, int> &members = iterCh->second.getMembers();
    std::map<std::string, int>::iterator iterNick = members.find(nickname);
    if (iterNick == members.end()) 
    {
        no_member_channel_442(message);
        return;
    }
    else
    {
        std::string cmd = "PART " + channelName;
        if (message.getArg().size() > 1)
            cmd += " " + message.getArg()[1];
        Message reply(message.getSocket(), cmd);
        serverInstance->getChannel()[channelName].broadcasting(
            fromClient.makePrefix(), reply);
        cmd = fromClient.makePrefix() + " " + cmd;
        serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(
            cmd);
        members.erase(nickname);
        std::map<std::string, int>::iterator iterOp = members.begin();
        while (iterOp != members.end())
        {
            if (iterOp->second == 1)
                
                break;
            iterOp++;
        }
        if (iterOp == members.end())
        
        {
            
            std::map<std::string, Channel> &channels =
                this->serverInstance->getChannel();
            channels[iterCh->first].partAll();
            channels.erase(iterCh->first);
        }
    }
    return;
}


void Command::kick(Message &message)
{
    if (message.getArg().size() < 2 || message.getArg()[0].empty() ||
        message.getArg()[1].empty())
    {
        command_empty_argument_461(message);
        return;
    }

    
    std::map<int, Client> &socketFdToClient = getServerSocketFdToClient();

    std::map<std::string, Channel> &channel = getServerChannel();
    std::string channelName = message.getArg()[0];
    std::map<std::string, Channel>::iterator iterCh = channel.find(channelName);
    if (iterCh == channel.end())
    {
        no_such_channel_403(message);
        return;
    }

    Client &fromClient = socketFdToClient[message.getSocket()];
    std::string nickname = fromClient.getNickname();
    std::map<std::string, int> &members = iterCh->second.getMembers();
    std::map<std::string, int>::iterator iterNick = members.find(nickname);
    if (iterNick == members.end())
    {
        no_member_channel_442(message);
        return;
    }
    if (iterNick->second != 1) 
    {
        no_operator_channel_482(message);
        return;
    }

    std::string kickName = message.getArg()[1];
    iterNick = members.find(kickName);
    if (iterNick == members.end()) 
    {
        no_users_channel_441(message);
        return;
    }

    int kickFd = serverInstance->getNicknameToSocketFd()[kickName];
    Client &kickClient = serverInstance->getSocketFdToClient()[kickFd];
    std::string partmsg = kickClient.makePrefix() + " PART " + channelName;
    kickClient.sendMessage(partmsg);

    std::string cmd = "KICK " + channelName + " " + kickName;
    if (message.getArg()[2].length() > 1)
        cmd += " " + message.getArg()[2];
    Message reply(message.getSocket(), cmd);
    serverInstance->getChannel()[iterCh->first].broadcasting(
        fromClient.makePrefix(), reply);
    cmd = fromClient.makePrefix() + " " + cmd;
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(cmd);

    members.erase(kickName);

    std::map<std::string, int>::iterator iterOp = members.begin();
    while (iterOp != members.end())
    {
        if (iterOp->second == 1)
            
            break;
        iterOp++;
    }
    if (iterOp == members.end()) 
    {
       
        members.clear();
        std::map<std::string, Channel> &channels =
            this->serverInstance->getChannel();
        channels.erase(iterCh->first);
    }
    return;
}


void Command::topic(Message &message)
{
    if (message.getArg().size() < 1 || message.getArg()[0].empty())
    {
        command_empty_argument_461(message);
        return;
    }

    std::map<int, Client> &socketFdToClient = getServerSocketFdToClient();
    std::map<std::string, Channel> &channel = getServerChannel();
    std::string channelName = message.getArg()[0];
    std::map<std::string, Channel>::iterator iterCh = channel.find(channelName);
    if (iterCh == channel.end())
    {
        no_such_channel_403(message);
        return;
    }

    Client &fromClient = socketFdToClient[message.getSocket()];
    std::string nickname = fromClient.getNickname();
    std::map<std::string, int> members = iterCh->second.getMembers();
    std::map<std::string, int>::iterator iterNick = members.find(nickname);
    if (iterNick == members.end())
    {
        no_member_channel_442(message);
        return;
    }
    if (message.getArg().size() == 1)
    {
        if (channel[channelName].getTopic().empty())
            no_topic_channel_331(message);
        else
            yes_topic_channel_332(message, channel[channelName].getTopic());
        return;
    }

    if (channel[channelName].getMODE_T() && iterNick->second != 1)
    {
        no_operator_channel_482(message);
        return;
    }

    if (!message.getArg()[1].empty())
    {
        std::string &topic = channel[channelName].getTopic();
        topic = message.getArg()[1];
        yes_topic_channel_332(message, message.getArg()[1]);
        Message reply(message.getSocket(),
                      "TOPIC " + channelName + " " + message.getArg()[1]);
        serverInstance->getChannel()[iterCh->first].broadcasting(
            fromClient.makePrefix(), reply);
    }
    return;
}


void Command::invite(Message &message)
{
    if (message.getArg().size() < 2 || message.getArg()[0].empty() ||
        message.getArg()[1].empty())
    {
        command_empty_argument_461(message);
        return;
    }

   
    std::map<int, Client> &socketFdToClient = getServerSocketFdToClient();
    std::map<std::string, Channel> &channel = getServerChannel();

    std::string channelName = message.getArg()[1];
    std::map<std::string, Channel>::iterator iterCh = channel.find(channelName);
    if (iterCh == channel.end())
    {
        no_nick_member_401(channelName, message);
        return;
    }

    Client &fromClient = socketFdToClient[message.getSocket()];
    std::string &nickname = fromClient.getNickname();
    std::map<std::string, int> &members = iterCh->second.getMembers();
    std::map<std::string, int>::iterator iterNick = members.find(nickname);
    if (iterNick == members.end())
    {
        no_member_channel_442(message);
        return;
    }
    if (iterNick->second != 1)
    {
        no_operator_channel_482(message);
        return;
    }

    std::string &newMemberName = message.getArg()[0];
    iterNick = members.find(newMemberName);
    if (iterNick != members.end()) 
    {
        user_already_channel_443(message);
        return;
    }
    
    std::map<std::string, int> &socket_map = getServernicknameToSocketFd();
    if (socket_map.find(newMemberName) == socket_map.end())
    {
        no_nick_member_401(newMemberName, message);
        return;
    }

    
    iterCh->second.inviteNewMember(newMemberName);

   
    {
        int socketToSend =
            getServernicknameToSocketFd().find(newMemberName)->second;
        std::vector<std::string> argsToSend;
        argsToSend.push_back(newMemberName);
        argsToSend.push_back(channelName);
        Message messageToNewMember(socketToSend, fromClient.makePrefix(),
                                   "INVITE", argsToSend);
        socketFdToClient[socketToSend].sendMessage(messageToNewMember);
    }
    success_invite_341(message);
    return;
}


void Command::mode(Message &message)
{
    if (message.getArg().size() < 1 || message.getArg()[0].empty())
    {
        command_empty_argument_461(message);
        return;
    }
    if (message.getArg()[0][0] != '&' && message.getArg()[0][0] != '#')
        return;

    std::map<int, Client> &socketFdToClient = getServerSocketFdToClient();
    std::map<std::string, Channel> &channel = getServerChannel();

    std::string channelName = message.getArg()[0];
    std::map<std::string, Channel>::iterator iterCh = channel.find(channelName);
    if (iterCh == channel.end())
    {
        no_such_channel_403(message);
        return;
    }

    Client &fromClient = socketFdToClient[message.getSocket()];
    std::string nickname = fromClient.getNickname();
    std::map<std::string, int> &members = iterCh->second.getMembers();
    std::map<std::string, int>::iterator iterNick = members.find(nickname);
    if (iterNick == members.end())
    {
        no_member_channel_442(message);
        return;
    }

    
    if (message.getArg().size() == 1)
    {
        channel_mode_324(message, iterCh->second);
        return;
    }

    
    if (iterNick->second != 1)
    {
        no_operator_channel_482(message);
        return;
    }

    if (message.getArg()[1][1] == 'o')
    {
        if (message.getArg().size() < 3 || message.getArg()[2].empty())
        {
            command_empty_argument_461(message);
            return;
        }
        std::string targetName = message.getArg()[2];
        iterNick = members.find(targetName);
        if (iterNick == members.end()) 
        {
            no_nick_member_401(message.getArg()[2], message);
            return;
        }
        else if (message.getArg()[1][0] == '+')
            members[targetName] = 1;
        else if (message.getArg()[1][0] == '-')
            members[targetName] = 0;
        else
        {
            // wrong arg
            return;
        }
    }

    else if (message.getArg()[1][1] == 'k')
    {
        if (message.getArg().size() < 3 || message.getArg()[2].empty())
        {
            command_empty_argument_461(message);
            return;
        }
        std::string &oldPassword = iterCh->second.getKey();
        std::string newPassword = message.getArg()[2];
        if (message.getArg()[1][0] == '+' && oldPassword == "")
            oldPassword = newPassword;
        else if (message.getArg()[1][0] == '-' && oldPassword == newPassword)
            oldPassword = "";
        else
        {

            return;
        }
    }

    else if (message.getArg()[1][1] == 'l')
    {
        if (message.getArg()[1][0] == '+' && (message.getArg().size() < 3 || message.getArg()[2].empty()))
        {
            command_empty_argument_461(message);
            return;
        }

        int &oldLimit = iterCh->second.getLimit();
        size_t newLimit = atoi(message.getArg()[2].c_str());
        if (message.getArg()[1][0] == '+')
        {
            if (newLimit < members.size())
            {
               
                return;
            }
            oldLimit = newLimit;
        }
        else if (message.getArg()[1][0] == '-')
            oldLimit = 0;
        else
        {
            //  wrong arg
            return;
        }
    }

    else if (setMode(message, iterCh->second) == false)
    {
        //  wrong arg
        return;
    }
    std::string cmd = "MODE " + channelName + " " + message.getArg()[1];
    if (message.getArg().size() > 2)
        cmd += " :" + message.getArg()[2];
    Message reply(message.getSocket(), cmd);
    serverInstance->getChannel()[iterCh->first].broadcasting(
        fromClient.makePrefix(), reply);
    cmd = fromClient.makePrefix() + " " + cmd;
    serverInstance->getSocketFdToClient()[message.getSocket()].sendMessage(cmd);
    return;
}

bool Command::setMode(Message &message, Channel &channel)
{
    bool MODE;

    if (message.getArg()[1][0] == '+')
        MODE = true;
    else if (message.getArg()[1][0] == '-')
        MODE = false;
    else
        return false;

    if (message.getArg()[1][1] == 'i')
        channel.setMODE_I(MODE);
    else if (message.getArg()[1][1] == 't')
        channel.setMODE_T(MODE);
    else
        return false;

    return true;
}

void Command::quit(Message &message)
{
    serverInstance->terminateConnection(message.getSocket());
}