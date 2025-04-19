#pragma once
#include <string>
#include "LoggedUser.h"
#include "IRequestHandler.h"
#include "LoginRequestHandler.h"
#include "Room.h"

class MenuRequestHandler : public IRequestHandler {
public:
    MenuRequestHandler(const LoggedUser& user,  RequestHandlerFactory& handlerFactory);

    bool isRequestRelevant(const RequestInfo& requestInfo) const override;
    RequestResult handleRequest(const RequestInfo& requestInfo) override;

private:
    RequestResult signout(const RequestInfo& requestInfo);
    RequestResult getRooms(const RequestInfo& requestInfo);
    RequestResult getPlayersInRoom(const RequestInfo& requestInfo);
    RequestResult getPersonalStats(const RequestInfo& requestInfo);
    RequestResult getHighScore(const RequestInfo& requestInfo);
    RequestResult joinRoom(const RequestInfo& requestInfo);
    RequestResult createRoom(const RequestInfo& requestInfo);

    RequestResult createErrorRequestResult() ;
    RequestResult createLogoutRequestResult() ;
    RequestResult createGetRoomsRequestResult(const std::vector<RoomData>& rooms) ;
    RequestResult createGetPlayersInRoomRequestResult(const std::vector<std::string>& players) ;
    RequestResult createGetPersonalStatsRequestResult(const std::vector<std::string>& statistics) ;
    RequestResult createGetHighScoreRequestResult(const std::vector<std::string>& statistics) ;
    RequestResult createJoinRoomRequestResult() ;
    RequestResult createCreateRoomRequestResult() ;

    LoggedUser m_user;
    RequestHandlerFactory m_handlerFactory;
};
