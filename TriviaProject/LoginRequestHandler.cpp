#include "LoginRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"

LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory& factory): m_handlerFactory(factory)
{
	
}

bool LoginRequestHandler::isRequestRelevant(const RequestInfo& req) const
{
	if (req.RequestId == loginCode || req.RequestId == signupCode)
	{
		return true;
	}
	
	return false;
}

RequestResult LoginRequestHandler::handleRequest(const RequestInfo& req)
{
	RequestResult myReq;
	if (req.RequestId == loginRequestCode)
	{
		myReq = LoginRequestHandler::login(req);
	}
	else
	{
		myReq = LoginRequestHandler::signup(req);
	}
	return myReq;

}

RequestResult LoginRequestHandler::login(const RequestInfo& req)
{
	LoginManager manager = m_handlerFactory.getLoginManager();
	RequestResult result;
	LoginRequest userLoginRequest = JsonRequestPacketDeserializer::deserializeLoginRequest(req.buffer);
	if (manager.login(userLoginRequest.username, userLoginRequest.password) == true)
	{
		LoginResponse userLoginResponse;
		userLoginResponse.status = SUCCESS;
		result.response = JsonResponsePacketSerializer::serializeResponse(userLoginResponse);
		result.newHandler = m_handlerFactory.createMenuRequestHandler(LoggedUser(userLoginRequest.username));
		return result;
	}
	ErrorResponse error;
	error.message = "Error occured while logging in.";
	result.response = JsonResponsePacketSerializer::serializeResponse(error);
	return result;
}

RequestResult LoginRequestHandler::signup(const RequestInfo& req)
{
	LoginManager manager = m_handlerFactory.getLoginManager();
	SignupRequest userSignupRequest = JsonRequestPacketDeserializer::deserializeSingnupRequest(req.buffer);
	RequestResult result;
	if (manager.signup(userSignupRequest.username, userSignupRequest.password, userSignupRequest.email) == true)
	{
		SignupResponse userSignupResponse;
		userSignupResponse.status = SUCCESS;
		result.response = JsonResponsePacketSerializer::serializeResponse(userSignupResponse);
		result.newHandler = m_handlerFactory.createMenuRequestHandler(LoggedUser(userSignupRequest.username));
		return result;
	}
	/*else
	{
		LoginResponse userLoginResponse;
		userLoginResponse.status = errorCode;
		result.response = JsonResponsePacketSerializer::serializeResponse(userLoginResponse);
	}*/
	ErrorResponse error;
	error.message = "Error occured while signing up.";
	result.response = JsonResponsePacketSerializer::serializeResponse(error);
	return result;

}
