Intro
Internet Relay Chat or IRC is a text-based communication protocol on the Internet. It offers real-time messaging that can be either public or private. Users can exchange direct messages and join group channels. IRC clients connect to IRC servers in order to join channels. IRC servers are connected together to form a network.

This program creates and runs an IRC server. It follows the IRC protocols when communicating with IRC clients and has several bots into its programming to interact with.

Table of Contents
Usage
Client
Bots
Information
Environment
Sockets
Ports
Poll
Classes
Namespaces
Evaluation
Sources
Creators
Usage
The server has been created for and tested on Linux, but also has been designed to be compatible with MacOS. It can be compiled and run using the following terminal commands.

Terminal Commands
Command	Arguments	Functionality
make		Compiles the files into ircserv
./ircserv	[port] [password]	Runs the binary
make	subject	Compiles the files and runs the binary with port '6667' and password set to 'password'
./nctest.sh		Runs a shell script that in turn runs nc. Automatically logs in into the server
Client
While this server, in theory, is compatible with all IRC Clients that follow the IRC protocol. It has been tested using Pidgin and is fully compatible with it. To connect with Pidgin make a new Account and set the following settings.

Set Protocol to 'IRC'
Set Username to the desired Nickname
Set Server to the Local IP Address provided by the IRC Server
Set Password to the password passed to the server
Set Port(Advanced) to the port passed to the server
Set Ident name(Advanced) to the desired name
Set Real name(Advanced) to the desired full name
ModifyAccountBasic ModifyAccountAdvanced

Alternatively nc can be used to connect. This will however require the user to type in full IRC Protocol. The Shell Script nctest.sh can be used to quickly connect to the IRC Server and will register the user and execute a few test commands.

Bots
While the IRC Server is mainly for client to client communication. It also contains several bots for users to interact with.

Bots
Main	Primary Call	Functionality
ServerBot	//info	A bot that only works through private messages. He can provide some general server information and shutdown the server
TicTacBot	//play	A bot that can play Tic Tac Toe
RockBot	//throw	A bot that can play Rock Paper Scissors
Information
The IRC Server has been written in C++ and runs a server that responds to IRC Protocol messages.

Environment
The project contains an .env file which valuable information about the server. This includes the server name, the password for admins and the level of verbose. Whenever this file is changes, the server automatically updates accordingly.

The IRC_VERBOSE setting in .env can be set to adjust the types of output are sent to the standard output.

Verbose Settings
Setting	enum	Message Type
0	V_SILENT	Silent
1	V_ADMIN	Include Admin messages
2	V_CHANNEL	Include Channel events
3	V_USER	Include User events
4	V_DETAILS	Include Details on construction and deconstruction of users
5	V_MSG	Include all messages sent to and from the Server
Sockets
Using the int socket() function, a socket is created for internet communication (AF_INET) over which bi-directional communication (SOCK_STREAM) is possible. Using the int setsockopt() function, the socket is set to bind a socket, even if the address is already in use. For MacOS systems the function int fcntl() is used to ensure the socket is non-blocking (O_NONBLOCK).

Ports
Then using the int bind() function, the socket is bound to the specified port and the function int listen() sets it to listen to the port for incoming streams and maintaining a backlog, which can store 128 (SOMAXCONN) incoming messages. The accepted ports have been restricted to avoid usage ports that have predefined uses.

Restricted Ports
Port	Usage
0 ... 1023	Well-Known Ports(0-1023)
1433	Microsoft SQL Server
1521	Oracle Database
3306	MySQL Database
5000	UPnP (Universal Plug and Play) control point
5222	XMPP (Extensible Messaging and Presence Protocol)
5432	PostgreSQL Database
7070	Real-Time Messaging Protocol (RTMP)
8080	Alternate HTTP (commonly used for web servers)
8443	HTTPS Alternate (often used for secure web traffic)
9090	Alternate Web Server (often used for web proxy servers)
Poll
The chosen function to monitor incomming messages is poll(). It is called at two points. In the Server to monitor new connections and in the Client to monitor for new messages.

Classes
The bulk of this program consists of classes. There are classes for the Server, Channels and the Clients. The Clients are derived for the abstract class AClient, from which the bots are also derived. Once a Bot or Client is added the IRC Server no longer differentiates between them. The Server has been structured so that new Bots can easily be implemented. They do require definitions for the following public member functions.

AClient Pure Virtual Public Member Functions
Return	Public Member Function	Usage
bool	stillActive(void) const	Returns whether an Aclient is active
void	closeFD(void)	Clean up function for any File Descriptor or used alternatives
std::string	getMsg(void)	Messages the AClient wants to send to the server
void	sendMsg(std::string msg)	Messages sent to the AClient
Namespaces
Namespace-Based Encapsulation for Command and Parse Functionality

This program utilizes namespaces, namely Command and Parse, to organize and encapsulate related functions.

While classes are typically used for encapsulation in C++, the program makes use of namespaces as an alternative approach that provides logical grouping and enhances code maintainability. The use of namespaces eliminates the need to instantiate objects for accessing commands or parsing functionality, simplifying code usage and reducing memory overhead.

The Command namespace employs static functions to restrict their usage within the namespace itself. This effectively prevents unintended external access, simulating private access control. Only the main function is declared in the header file, while the remaining functions are called locally within the source file.

The Parse namespace demonstrates another encapsulation strategy. It declares the main function in the header file and defines it in the source file, while auxiliary functions are declared and defined within an unnamed namespace in the source file. This approach limits the visibility of auxiliary functions to the parseMsg() function, preventing their direct use outside the namespace.

These two namespace implementations showcase different techniques to achieve encapsulation and protect the usage of methods within a namespace context.


Sources
https://pidgin.im/
https://modern.ircdocs.horse/#introduction
