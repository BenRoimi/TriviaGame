#pragma once
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"

class LoginRequestHandler : public IRequestHandler
{
public:

	LoginRequestHandler(RequestHandlerFactory& factory);
	// Inherited via IRequestHandler
	bool isRequestRelevant(const RequestInfo& req) const override;
	RequestResult handleRequest(const RequestInfo& req) override;

private:
	RequestHandlerFactory& m_handlerFactory;

	RequestResult login(const RequestInfo& req);
	RequestResult signup(const RequestInfo& req);

	
};