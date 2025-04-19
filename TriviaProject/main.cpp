#pragma comment (lib, "ws2_32.lib")

#include "WSAInitializer.h"
#include "Communicator.h"
#include "Server.h"
#include <iostream>
#include <exception>
#include <thread>

#define PORT 8826

/*
for now helper isnt useable
*/

int main()
{
	try
	{
		WSAInitializer wsaInit;
		Server myServer;

		myServer.run();
	}
	catch (std::exception& e)
	{
		std::cout << "Error occured: " << e.what() << std::endl;
	}
	system("PAUSE");
	return 0;
}