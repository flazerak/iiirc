#include <iostream>
#include "../inc/Server.hpp"
using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 3)
        return (std::cout << "Wrong arguments" << std::endl, 1);
    int i = 0;
    int j;
    while (++i < argc)
    {
        j = 0;
        while (argv[i][j])
        {
            if (isspace(argv[i][j]) != 0)
                return (std::cout << "Wrong arguments" << std::endl, 1);
            j++;
        }
    
    }
	std::string argv_str = argv[1];
    if (argv_str.size() == 0 || argv_str.size() > 10)
        return (std::cout << "Wrong arguments" << std::endl, 1);
    Server server(atoi(argv[1]), argv[2]);
    try
    {
        server.openSocket();
        server.init();
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        return 1;
    }
    
	server.run();

}