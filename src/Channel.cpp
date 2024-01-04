#include "../inc/Channel.hpp"

Server *Channel::serverPtr;

Channel::Channel() {}

Channel::~Channel() {}

Channel::Channel(const std::string &name) : name(name)
{
    // Initialization of member variables
    this->topic = "";
    this->key = "";
    this->memberLimit = 0;
    MODE_I = false;
    MODE_T = false;
}

void Channel::setMembers(const std::string &nickname, const int operator_check)
{
    // Setting the members map with the provided nickname and operator_check
    this->members[nickname] = operator_check;
}

void Channel::inviteNewMember(std::string nickname)
{
    // Adding a nickname to the list of invited members
    invitedNickname.insert(nickname);
}

bool Channel::isInvited(std::string nickname)
{
    // Adding a nickname to the list of invited members
    if (invitedNickname.find(nickname) != invitedNickname.end())
        return true;
    else
        return false;
}

void Channel::deleteMemberFromInvitedList(std::string nickname)
{
    // Deleting a nickname from the list of invited members
    if (isInvited(nickname))
        invitedNickname.erase(nickname);
}

const std::string &Channel::getName() const { return name; }

std::string &Channel::getTopic() { return topic; }

std::string &Channel::getKey() { return key; }

int &Channel::getLimit() { return memberLimit; }

std::map<std::string, int> &Channel::getMembers()
{
    return members;
}

bool &Channel::getMODE_I() { return this->MODE_I; }

bool &Channel::getMODE_T() { return this->MODE_T; }

void Channel::setMODE_I(bool mode)
{
    MODE_I = mode;
}
void Channel::setMODE_T(bool mode)
{
    MODE_T = mode;
}

void Channel::broadcasting(std::string fromNickname, Message &message)
{
    // Broadcasting a message to all channel members except the sender
    std::map<std::string, int>::iterator iter = this->members.begin();

    while (iter != members.end())
    {
        Client &clientToSend = this->serverPtr->getClientByNickname(iter->first);
        if (fromNickname.substr(1, fromNickname.find('!') - 1) != iter->first)
        {
            Message messageToSend = message;
            messageToSend.setPrefix(fromNickname);
            clientToSend.sendMessage(messageToSend);
        }
        iter++;
    }
}

void Channel::setServerPtr(Server *server_ptr)
{
    // Setting the server pointer for the Channel class

    serverPtr = server_ptr;
}

bool Channel::isMember(const std::string& nickname)
{
    /// Checking if a nickname is a member of the channel

    if (members.find(nickname) != members.end())
        return true;
    else
        return false;
}

bool Channel::isNoOperator()
{
    // Checking if the channel has no operators
    std::map<std::string, int>::iterator iter = members.begin();

    while (iter != members.end())
    {
        if (iter->second == 1)
            return false;
        iter++;
    }
    return true;
}

void Channel::partAll()
{
    // Sending PART messages to all channel members when the channel is closed
    std::map<std::string, int>::iterator iter = members.begin();
    while (iter != members.end())
    {
        Client &client = serverPtr->getClientByNickname(iter->first);
        std::string str = client.makePrefix()  + " PART " + name;
        client.sendMessage(str);
        iter++;
    }
}
