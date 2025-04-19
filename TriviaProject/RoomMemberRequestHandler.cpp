#include "RoomMemberRequestHandler.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"

RoomMemberRequestHandler::RoomMemberRequestHandler(const unsigned int&  roomId, const LoggedUser& user, RoomManager& roomManager, RequestHandlerFactory& handlerFactory) :
	m_roomId(roomId), m_user(user), m_roomManager(roomManager), m_handlerFactory(handlerFactory)
{
}

bool RoomMemberRequestHandler::isRequestRelevant(const RequestInfo& Request) const
{
	int RequestCode = Request.RequestId;
	if (RequestCode == leaveRoomCode || RequestCode == getRoomStateRequestCode)
	{
		return true;
	}
	return false;
}


RequestResult RoomMemberRequestHandler::handleRequest(const RequestInfo& Request)
{
	int RequestCode = Request.RequestId;
	
	if (RequestCode == leaveRoomCode)
	{
		return leaveRoom(Request);
	}
	else if (RequestCode == getRoomStateRequestCode)
	{
		return getRoomState(Request);
	}
	else
	{
		return createErrorRequestResult();
	}
}

RequestResult RoomMemberRequestHandler::leaveRoom(const RequestInfo& Request) const
{
	
	try
	{
		Room room = m_roomManager.getRoom(m_roomId);
		room.removeUser(m_user);
	}
	catch (const std::exception&)
	{
		return createLeaveRoomRequestResult();
	}
	return createLeaveRoomRequestResult();
}

RequestResult RoomMemberRequestHandler::getRoomState(const RequestInfo& Request) const
{
	try
	{
		ROOM_STATUS roomState = m_roomManager.getRoom(m_roomId).getRoomData().status;
		std::vector<std::string> players = m_roomManager.getRoom(m_roomId).getAllUsers();
		unsigned int questionCount = m_roomManager.getRoom(m_roomId).getRoomData().numOfQuestionsInGame;
		unsigned int answerTimeout = m_roomManager.getRoom(m_roomId).getRoomData().timePerQuestion;
		return createGetRoomStateRequestResult(roomState, players, questionCount, answerTimeout);
	}
	catch (const std::exception&)
	{
		return RequestResult{ JsonResponsePacketSerializer::serializeResponse
	   (ErrorResponse{ "room not found or close" }), m_handlerFactory.createMenuRequestHandler(m_user)};
	}
	
}

RequestResult RoomMemberRequestHandler::createErrorRequestResult() const
{
	return RequestResult{ JsonResponsePacketSerializer::serializeResponse
	   (ErrorResponse{ "No relevant response" }), nullptr };
}

RequestResult RoomMemberRequestHandler::createLeaveRoomRequestResult() const
{
	return RequestResult{ JsonResponsePacketSerializer::serializeResponse
	   (LeaveRoomResponse{ SUCCESS }), m_handlerFactory.createMenuRequestHandler(m_user)};
}

RequestResult RoomMemberRequestHandler::createGetRoomStateRequestResult(const ROOM_STATUS& roomState, const std::vector<std::string>& players, const unsigned int& questionCount, const unsigned int& answerTimeout) const
{
	switch (roomState)
	{
	case waiting:
		return RequestResult{ JsonResponsePacketSerializer::serializeResponse
			(GetRoomStateResponse{ SUCCESS, roomState, players, questionCount, answerTimeout }), m_handlerFactory.createRoomMemberRequestHandler(m_user, m_roomId) };

	case active:
		return RequestResult{ JsonResponsePacketSerializer::serializeResponse
			(GetRoomStateResponse{ SUCCESS, roomState, players, questionCount, answerTimeout }), NULL };/*the handler need to be game manager*/

	case unavailable:
		return RequestResult{ JsonResponsePacketSerializer::serializeResponse
	   (ErrorResponse{ "room not found or close" }), m_handlerFactory.createMenuRequestHandler(m_user) };
	default:
		return createErrorRequestResult();
		break;
	}
	return createErrorRequestResult();
}


