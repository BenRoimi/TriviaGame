#include "JsonRequestPacketDeserializer.h"
#include "json.hpp"
#include <iostream>


LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(const std::vector<unsigned char>& Buffer)
{
    nlohmann::json json_object = nlohmann::json::parse(Buffer);

    LoginRequest request;
    request.username = json_object["username"];
    request.password = json_object["password"];

    return request;
}

SignupRequest JsonRequestPacketDeserializer::deserializeSingnupRequest(const std::vector<unsigned char>& Buffer)
{
    for (auto i : Buffer)
    {
        std::cout << i;
    }
    std::cout << std::endl;
    nlohmann::json json_object = nlohmann::json::parse(Buffer);

    SignupRequest request;
    request.username = json_object["username"];
    request.password = json_object["password"];
    request.email = json_object["mail"];

    return request;
}

GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersRequest(const std::vector<unsigned char>& Buffer)
{
    nlohmann::json json_object = nlohmann::json::parse(Buffer);
    GetPlayersInRoomRequest request;
    request.roomId = json_object["id"];
    return request;
    
}

JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(const std::vector<unsigned char>& Buffer)
{
    nlohmann::json json_object = nlohmann::json::parse(Buffer);
    JoinRoomRequest request;
    request.roomId = json_object["id"];
    return request;
    
}

CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(const std::vector<unsigned char>& Buffer)
{
    nlohmann::json json_object = nlohmann::json::parse(Buffer);
    CreateRoomRequest request;
    request.answerTimeout = json_object["timePerQuestion"];
    request.maxUsers = json_object["maxPlayers"];
    request.questionCount = json_object["numOfQuestionsInGame"];
    request.roomName = json_object["name"];
    return request;
}

SubmitAnswerRequest JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(const std::vector<unsigned char>& Buffer)
{
    nlohmann::json json_object = nlohmann::json::parse(Buffer);
    SubmitAnswerRequest request;
    request.answerId = json_object["correctAnswerId"];

    return request;
}

