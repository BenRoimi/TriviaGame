#include "Communicator.h"
#include "Helper.h"
#include <exception>
#include <iostream>
#include <thread>
#include <fstream>
#include "JsonResponsePacketSerializer.h"

Communicator::Communicator(RequestHandlerFactory& factory) : m_handlerFactory(factory)
{
	// this server use TCP. that why SOCK_STREAM & IPPROTO_TCP
	// if the server use UDP we will use: SOCK_DGRAM & IPPROTO_UDP
	m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (m_serverSocket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__ " - socket");
}

Communicator::~Communicator()
{
	try
	{
		// the only use of the destructor should be for freeing 
		// resources that was allocated in the constructor
		closesocket(m_serverSocket);
	}
	catch (...) {}
}

void Communicator::bindAndListen()
{
	int port = 8826;
	struct sockaddr_in sa = { 0 };
	
	sa.sin_port = htons(port); // port that server will listen for
	sa.sin_family = AF_INET;   // must be AF_INET
	sa.sin_addr.s_addr = INADDR_ANY;    // when there are few ip's for the machine. We will use always "INADDR_ANY"
	std::cout << "Starting..." << std::endl;

	// Connects between the socket and the configuration (port and etc..)
	if (bind(m_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");
	std::cout << "Binded" << std::endl;

	// Start listening for incoming requests of clients
	if (listen(m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");
	std::cout << "Listening on port " << port << std::endl;

	// the main thread is only accepting clients 
	// and add then to the list of handlers
	std::cout << "Opened for client connection requests" << std::endl;
}

void Communicator::startHandleRequests()
{
	bindAndListen();

	while (true) {
		// this accepts the client and create a specific socket from server to this client
		// the process will not continue until a client connects to the server
		SOCKET client_socket = accept(m_serverSocket, NULL, NULL);
		if (client_socket == INVALID_SOCKET)
			throw std::exception(__FUNCTION__);

		m_clients[client_socket] = m_handlerFactory.createLoginRequestHandler();

		std::cout << "Client accepted. Server and client can speak" << std::endl;
		// the function that handle the conversation with the client

		std::thread t(&Communicator::handleNewClient, this, client_socket);
		t.detach();
	}
}

void Communicator::handleNewClient(SOCKET clientSocket)
{
	try
	{
		while (m_clients[clientSocket] != nullptr)
		{
			char data[1];
			recv(clientSocket, data, 1, 0);
			int code = data[0];
			std::cout << code << std::endl;
			char* dataLength = Helper::getPartFromSocket(clientSocket, 4);
			int intdataLength = int((unsigned char)(dataLength[3]) << 24 |
				(unsigned char)(dataLength[2]) << 16 |
				(unsigned char)(dataLength[1]) << 8 |
				(unsigned char)(dataLength[0]));
			char* Pbuffer = Helper::getPartFromSocket(clientSocket, intdataLength);
			std::vector<unsigned char> buffer;
			for (int i = 0; i < intdataLength; ++i)
			{
				buffer.push_back(static_cast<unsigned char>(Pbuffer[i]));
				std::cout << static_cast<unsigned char>(Pbuffer[i]);
			}
			
			RequestInfo myRequest;
			myRequest.RequestId = code;
			myRequest.receivalTime = std::time(nullptr);
			myRequest.buffer = std::move(buffer);
			RequestResult myRequestResult = m_clients[clientSocket]->handleRequest(myRequest);
			send(clientSocket, reinterpret_cast<const char*>(myRequestResult.response.data()), myRequestResult.response.size(),0);
			m_clients[clientSocket] = std::move(myRequestResult.newHandler);
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		closesocket(clientSocket);
	}
	
}
