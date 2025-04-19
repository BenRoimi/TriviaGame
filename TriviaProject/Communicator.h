#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <map>
#include <memory>
#include "RequestHandlerFactory.h"
#include "IRequestHandler.h"

class Communicator
{
public:
	Communicator(RequestHandlerFactory& factory);
	~Communicator();
	void startHandleRequests();

private:

	void bindAndListen();
	void handleNewClient(SOCKET clientSocket);
	// void manageClientsComms(SOCKET clientSocket);
	RequestHandlerFactory& m_handlerFactory;
	SOCKET m_serverSocket;
	std::map<SOCKET, std::unique_ptr<IRequestHandler>> m_clients;
};

