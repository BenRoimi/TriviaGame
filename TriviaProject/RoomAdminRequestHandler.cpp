#include "RoomAdminRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"

RoomAdminRequestHandler::RoomAdminRequestHandler(const unsigned int& m_roomId, const LoggedUser& user, RoomManager& roomManager, RequestHandlerFactory& handlerFactory)
    : m_roomId(m_roomId), m_user(user), m_roomManager(roomManager), m_handlerFactory(handlerFactory)
{
}

bool RoomAdminRequestHandler::isRequestRelevant(const RequestInfo& requestInfo) const
{
    int code = requestInfo.RequestId;

    return ((code == closeRoomRequestCode) || (code == startGameRequestCode) || (code == getRoomStateRequestCode));
}

RequestResult RoomAdminRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
    int code = requestInfo.RequestId;

    switch (code)
    {
    case closeRoomRequestCode:
        return closeRoom(requestInfo);
    case startGameRequestCode:
        return startGame(requestInfo);
    case getRoomStateRequestCode:
        return getRoomState(requestInfo);
    default:
        return createErrorRequestResult();
    }
}

RequestResult RoomAdminRequestHandler::closeRoom(const RequestInfo& requestInfo)
{
    try
    {
        m_handlerFactory.getGameManager().deleteGame(m_roomManager.getRoom(m_roomId).getRoomData().id);
       
    }
    catch (const std::exception&)
    {
        return createCloseRoomRequestResult();
    }
    return createCloseRoomRequestResult();
    
}

RequestResult RoomAdminRequestHandler::startGame(const RequestInfo& requestInfo)
{
    return createStartGameRequestResult();
}

RequestResult RoomAdminRequestHandler::getRoomState(const RequestInfo& requestInfo)
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
       (ErrorResponse{ "room not found or close" }), m_handlerFactory.createMenuRequestHandler(m_user) };
    }
}

RequestResult RoomAdminRequestHandler::createErrorRequestResult() 
{
    return RequestResult{ JsonResponsePacketSerializer::serializeResponse
        (ErrorResponse{ "No relevant response" }), nullptr };
}

RequestResult RoomAdminRequestHandler::createCloseRoomRequestResult() 
{
    return RequestResult{ JsonResponsePacketSerializer::serializeResponse
        (CloseRoomResponse{ SUCCESS }), m_handlerFactory.createMenuRequestHandler(m_user) };
}

RequestResult RoomAdminRequestHandler::createStartGameRequestResult() 
{
    auto game = m_handlerFactory.getGameManager().createGame(m_roomManager.getRoom(m_roomId));
    return RequestResult{ JsonResponsePacketSerializer::serializeResponse
        (StartGameResponse{ SUCCESS }), m_handlerFactory.createGameRequestHandler(m_user, game)};
}

RequestResult RoomAdminRequestHandler::createGetRoomStateRequestResult(const ROOM_STATUS& roomState, const std::vector<std::string>& players, const unsigned int& questionCount, const unsigned int& answerTimeout) 
{
    switch (roomState)
    {
    case waiting:
        return RequestResult{ JsonResponsePacketSerializer::serializeResponse
            (GetRoomStateResponse{ SUCCESS, roomState, players, questionCount, answerTimeout }), m_handlerFactory.createRoomAdminRequestHandler(m_user, m_roomId)};

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
