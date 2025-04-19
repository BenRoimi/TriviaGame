#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <map>
#include <string>
#include "Communicator.h"
#include "IDatabase.h"
#include "RequestHandlerFactory.h"

class Server
{
public:
	Server();
	void run();

private:
	std::shared_ptr<IDatabase> m_db;
	RequestHandlerFactory m_factory;
	Communicator m_communicator;
};
