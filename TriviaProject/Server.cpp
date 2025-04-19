#include "Server.h"
#include "SqliteDatabase.h"
#include <thread>
#include <memory>
#include <iostream>

Server::Server(): m_db(std::make_shared<SqliteDatabase>()), m_factory(m_db), m_communicator(m_factory)
{
}

void Server::run()
{
	std::thread t2(&Communicator::startHandleRequests, &m_communicator);
	t2.detach();

	std::string input = "";
	std::cout << "Enter \"EXIT\" to quit\n";
	while (input != "EXIT") {
		std::cin >> input;
	}
}
