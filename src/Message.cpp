#include "../inc/Message.hpp"


Message::Message(int socket, std::string msg)
: socket(socket)
{
    parse(msg);
}

Message::~Message() {}

std::vector<std::string> split(std::string input, char delimiter)
{
    // Function to split a string into a vector of strings based on a delimiter
    // Uses a stringstream to tokenize the input string
    std::vector<std::string> answer;
    std::stringstream ss(input);
    std::string temp;
 
    while (getline(ss, temp, delimiter))
	{
        answer.push_back(temp);
    }
    return answer;
}

std::string& rtrim(std::string& s, const char* t = " \t\n\r\f\v") 
{
    // Function to trim whitespace from the right of a string
    // Erases characters in the provided set (default is whitespace) from the end of the string
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

const std::string &Message::getPrefix() const
{
	return (this->prefix);
}

const std::string &Message::getCommand() const
{
	return (this->command);
}

std::vector<std::string> &Message::getArg()
{
	return (this->argument);
}

int	Message::getSocket()
{
	return (this->socket);
}

void Message::parse(std::string msg)
{
    //responsible for parsing the input message,
    //trims leading and trailing whitespaces from the message,
    //splits it into a vector of strings using space as the delimiter,
    //and then extracts the prefix, command, and arguments from the split parts
    
    rtrim(msg);

	std::vector<std::string> msg_split;

	msg_split = split(msg, ' ');
	size_t i = 0;
    if (msg.at(0) == ':')
	{
		this->prefix = msg_split[i++];
		this->command = msg_split[i++];
    }
    else
    {
		this->prefix = "";
		this->command = msg_split[i++];
    }
	while (i < msg_split.size())
    {
        this->argument.push_back(msg_split[i]);
        i++;
    }
}

Message::Message(int socket,
                 const std::string &prefix,
                 const std::string &command,
                 const std::vector<std::string> &argument)
        : socket(socket), prefix(prefix), command(command), argument(argument)
{}

void Message::setPrefix(const std::string &prefix)
{
    Message::prefix = prefix;
}
