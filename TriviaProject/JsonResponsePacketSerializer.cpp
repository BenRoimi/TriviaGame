#include "JsonResponsePacketSerializer.h"
#include "json.hpp"

using json = nlohmann::json;

std::vector<unsigned char> serializeGeneralResponse(const unsigned char code, const std::string& dataStr)
{
    std::vector<unsigned char> buffer;
    buffer.push_back(code);

    int len = (int)dataStr.length();
    buffer.push_back((len >> 24) & 0xFF);
    buffer.push_back((len >> 16) & 0xFF);
    buffer.push_back((len >> 8) & 0xFF);
    buffer.push_back(len & 0xFF);

    buffer.insert(buffer.end(), dataStr.begin(), dataStr.end());
    return buffer;
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const LoginResponse& loginResp)
{
    json data = { {"status", loginResp.status} };
    return serializeGeneralResponse(loginCode, data.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const SignupResponse& signupResp)
{
    json data = { {"status", signupResp.status} };
    return serializeGeneralResponse(signupCode, data.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const ErrorResponse& errorResp)
{
    json data = { {"message", errorResp.message} };
    return serializeGeneralResponse(errorCode, data.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const LogoutResponse& logoutResp)
{
    json data = { {"status", logoutResp.status} };
    return serializeGeneralResponse(logoutCode, data.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const JoinRoomResponse& joinRoomResp)
{
    json data = { {"status", joinRoomResp.status} };
    return serializeGeneralResponse(joinRoomCode, data.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const CreateRoomResponse& createRoomResp)
{
    json data = { {"status", createRoomResp.status} };
    return serializeGeneralResponse(createRoomCode, data.dump());
}

void to_json(json& j, const RoomData& r) {
    j = json{
        {"id", r.id},
        {"name", r.name},
        {"maxPlayers", r.maxPlayers},
        {"numOfQuestionsInGame", r.numOfQuestionsInGame},
        {"timePerQuestion", r.timePerQuestion},
        {"status", r.status}
    };
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetRoomsResponse& getRoomsResp)
{
    json data;
    data["status"] = getRoomsResp.status;
    data["rooms"] = getRoomsResp.rooms;
    return serializeGeneralResponse(getRoomsCode, data.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetPlayersInRoomResponse& getPlayersResp)
{
    json data;
    data["PlayersInRoom"] = getPlayersResp.players;
    return serializeGeneralResponse(getPlayersCode, data.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetHighScoreResponse& getHighScoreResp)
{
    json data;
    data["HighScores"] = getHighScoreResp.statistics;
    return serializeGeneralResponse(getHighScoreCode, data.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetPersonalStatsResponse& getPersonalStatsResp)
{
    json data;
    data["UserStatistics"] = getPersonalStatsResp.statistics;
    return serializeGeneralResponse(getPersonalStatsCode, data.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const CloseRoomResponse& closeRoomResp)
{
   json data = { {"status", closeRoomResp.status} };
   return serializeGeneralResponse(closeRoomCode, data.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const StartGameResponse& startGameResp)
{
    json data = { {"status", startGameResp.status} };
    return serializeGeneralResponse(startGameCode, data.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetRoomStateResponse& getRoomStateResp)
{
   json data = { {"status", getRoomStateResp.status},
        {"roomState", getRoomStateResp.roomState},
        {"players", getRoomStateResp.players},
        {"questionCount", getRoomStateResp.questionCount},
        {"answerTimeout", getRoomStateResp.answerTimeout} };
   return serializeGeneralResponse(getRoomStateCode, data.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const LeaveRoomResponse& leaveRoomResp)
{
    json data = { {"status", leaveRoomResp.status} };
    return serializeGeneralResponse(leaveRoomCode, data.dump());
}

void to_json(json& j, const PlayerResults& r) {
    j = json{
        {"username", r.username},
        {"correctAnswerCount", r.correctAnswerCount},
        {"wrongAnswerCount", r.wrongAnswerCount},
        {"averageAnswerTime", r.averageAnswerTime}
    };
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetGameResultsResponse& getGameResultsResp)
{
    json data;
    data["status"] = getGameResultsResp.status;
    data["results"] = getGameResultsResp.results;
    return serializeGeneralResponse(getGameResultsCode, data.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const SubmitAnswerResponse& submitAnswerResp)
{
    json data = { {"status", submitAnswerResp.status}, {"correctAnswerId", submitAnswerResp.correctAnswerId} };
    return serializeGeneralResponse(submitAnswerCode, data.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const GetQuestionResponse& getQuestionResp)
{
    json answers;
    for (const auto& pair : getQuestionResp.answers) {
        json obj;
        obj["key"] = pair.first;
        obj["value"] = pair.second;
        answers.push_back(obj);
    }

    json data = { {"status", getQuestionResp.status}, {"question", getQuestionResp.question}, {"answers", getQuestionResp.answers} };
    return serializeGeneralResponse(getQuestionCode, data.dump());
}

std::vector<unsigned char> JsonResponsePacketSerializer::serializeResponse(const LeaveGameResponse& leaveGameResp)
{
    json data = { {"status", leaveGameResp.status} };
    return serializeGeneralResponse(leaveGameCode, data.dump());
}
