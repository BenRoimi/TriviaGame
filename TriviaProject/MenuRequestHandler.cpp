#include "MenuRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"

MenuRequestHandler::MenuRequestHandler(const LoggedUser& user,  RequestHandlerFactory& handlerFactory)
    : m_user(user), m_handlerFactory(handlerFactory)
{
}

bool MenuRequestHandler::isRequestRelevant(const RequestInfo& requestInfo) const
{
    int code = requestInfo.RequestId;

    return ((code == createRoomRequestCode) || (code == getRoomsRequestCode) ||
        (code == getPlayersInRoomRequestCode) || (code == joinRoomRequestCode) ||
        (code == getStatisticsRequestCode) || (code == logoutRequestCode) || (code == getHighScoreRequestCode));
}

RequestResult MenuRequestHandler::createErrorRequestResult() 
{
    return RequestResult{ JsonResponsePacketSerializer::serializeResponse
        (ErrorResponse{ "No relevant response" }), nullptr };
}

RequestResult MenuRequestHandler::createLogoutRequestResult() 
{
    return RequestResult{ JsonResponsePacketSerializer::serializeResponse
        (LogoutResponse{ SUCCESS }), m_handlerFactory.createLoginRequestHandler() };
}

RequestResult MenuRequestHandler::createGetRoomsRequestResult(const std::vector<RoomData>& rooms) 
{
    return RequestResult{ JsonResponsePacketSerializer::serializeResponse
        (GetRoomsResponse{ SUCCESS, rooms }), m_handlerFactory.createMenuRequestHandler(m_user) };
}

RequestResult MenuRequestHandler::createGetPlayersInRoomRequestResult(const std::vector<std::string>& players) 
{
    return RequestResult{ JsonResponsePacketSerializer::serializeResponse
        (GetPlayersInRoomResponse{ players }), m_handlerFactory.createMenuRequestHandler(m_user) };
}

RequestResult MenuRequestHandler::createGetPersonalStatsRequestResult(const std::vector<std::string>& statistics) 
{
    return RequestResult{ JsonResponsePacketSerializer::serializeResponse
        (GetPersonalStatsResponse{ SUCCESS, statistics }), m_handlerFactory.createMenuRequestHandler(m_user) };
}

RequestResult MenuRequestHandler::createGetHighScoreRequestResult(const std::vector<std::string>& statistics) 
{
    return RequestResult{ JsonResponsePacketSerializer::serializeResponse
        (GetHighScoreResponse{ SUCCESS, statistics }), m_handlerFactory.createMenuRequestHandler(m_user) };
}

RequestResult MenuRequestHandler::createJoinRoomRequestResult() 
{
    return RequestResult{ JsonResponsePacketSerializer::serializeResponse
        (JoinRoomResponse{ SUCCESS }), m_handlerFactory.createMenuRequestHandler(m_user) };
}

RequestResult MenuRequestHandler::createCreateRoomRequestResult() 
{
    return RequestResult{ JsonResponsePacketSerializer::serializeResponse
        (CreateRoomResponse{ SUCCESS }), m_handlerFactory.createMenuRequestHandler(m_user) };
}

RequestResult MenuRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
    int code = requestInfo.RequestId;

    switch (code)
    {
    case createRoomRequestCode:
        return createRoom(requestInfo);
    case getRoomsRequestCode:
        return getRooms(requestInfo);
    case getPlayersInRoomRequestCode:
        return getPlayersInRoom(requestInfo);
    case joinRoomRequestCode:
        return joinRoom(requestInfo);
    case getStatisticsRequestCode:
        return getPersonalStats(requestInfo);
    case logoutRequestCode:
        return signout(requestInfo);
    case getHighScoreCode:
        return getHighScore(requestInfo);
    default:
        return createErrorRequestResult();
    }
}

RequestResult MenuRequestHandler::signout(const RequestInfo& requestInfo)
{
    m_handlerFactory.getLoginManager().logout(m_user.getUsername());

    return createLogoutRequestResult();
}

RequestResult MenuRequestHandler::getRooms(const RequestInfo& requestInfo)
{
    return createGetRoomsRequestResult(m_handlerFactory.getRoomManager().getRooms());
}

RequestResult MenuRequestHandler::getPlayersInRoom(const RequestInfo& requestInfo)
{
    int roomID = JsonRequestPacketDeserializer::deserializeGetPlayersRequest(requestInfo.buffer).roomId;

    return createGetPlayersInRoomRequestResult(m_handlerFactory.getRoomManager().getRoom(roomID).getAllUsers());
}

RequestResult MenuRequestHandler::getPersonalStats(const RequestInfo& requestInfo)
{
    return createGetPersonalStatsRequestResult(m_handlerFactory.getStatisticsManager().getUserStatistics(m_user.getUsername()));
}

RequestResult MenuRequestHandler::getHighScore(const RequestInfo& requestInfo)
{
    return createGetHighScoreRequestResult(m_handlerFactory.getStatisticsManager().getHighScore());
}

RequestResult MenuRequestHandler::joinRoom(const RequestInfo& requestInfo)
{
    return createJoinRoomRequestResult();
}

RequestResult MenuRequestHandler::createRoom(const RequestInfo& requestInfo)
{
    CreateRoomRequest roomReq = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(requestInfo.buffer);
    unsigned int id = m_handlerFactory.getRoomManager().getRooms().size() + 1;
    std::string name = roomReq.roomName;
    unsigned int maxPlayers = roomReq.maxUsers;
    unsigned int numOfQuestionsInGame = roomReq.questionCount;
    unsigned int timePerQuestion = roomReq.answerTimeout;
    ROOM_STATUS status = waiting;
    RoomData data{ id , name , maxPlayers , numOfQuestionsInGame , timePerQuestion , status };
    m_handlerFactory.getRoomManager().createRoom(m_user, data);

    return createCreateRoomRequestResult();
}
