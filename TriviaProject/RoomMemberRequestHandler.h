#pragma once
#include "IRequestHandler.h"
#include "RoomManager.h"
#include "RequestHandlerFactory.h"

class RoomMemberRequestHandler : public IRequestHandler
{
public:
	RoomMemberRequestHandler(const unsigned int& roomId, const LoggedUser& user, RoomManager& roomManager, RequestHandlerFactory& handlerFactory);
	bool isRequestRelevant(const RequestInfo& Request) const override;
	RequestResult handleRequest(const RequestInfo& Request)override;

private:

	unsigned int  m_roomId;
	LoggedUser m_user;
	RoomManager& m_roomManager;
	RequestHandlerFactory& m_handlerFactory;

	RequestResult leaveRoom(const RequestInfo& Request) const;
	RequestResult getRoomState(const RequestInfo& Request)const;

	RequestResult createErrorRequestResult() const;
	RequestResult createLeaveRoomRequestResult() const;
	RequestResult createGetRoomStateRequestResult(const ROOM_STATUS& roomState, const std::vector<std::string>& players, const unsigned int& questionCount, const unsigned int& answerTimeout) const;

};