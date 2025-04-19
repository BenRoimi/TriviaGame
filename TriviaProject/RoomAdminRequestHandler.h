#pragma once
#include "IRequestHandler.h"
#include "RoomManager.h"
#include "RequestHandlerFactory.h"

class RoomAdminRequestHandler : public IRequestHandler {
public:
    RoomAdminRequestHandler(const unsigned int& roomId, const LoggedUser& user, RoomManager& roomManager, RequestHandlerFactory& handlerFactory);

    bool isRequestRelevant(const RequestInfo& requestInfo) const override;
    RequestResult handleRequest(const RequestInfo& requestInfo) override;

private:
    unsigned int  m_roomId;
    LoggedUser m_user;
    RoomManager& m_roomManager;
    RequestHandlerFactory& m_handlerFactory;

    RequestResult closeRoom(const RequestInfo& requestInfo);
    RequestResult startGame(const RequestInfo& requestInfo);
    RequestResult getRoomState(const RequestInfo& requestInfo);

    RequestResult createErrorRequestResult() ;
    RequestResult createCloseRoomRequestResult() ;
    RequestResult createStartGameRequestResult() ;
    RequestResult createGetRoomStateRequestResult(const ROOM_STATUS& roomState, const std::vector<std::string>& players, const unsigned int& questionCount, const unsigned int& answerTimeout) ;
};