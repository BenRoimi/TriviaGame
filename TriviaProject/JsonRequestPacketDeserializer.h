#pragma once
#include "IRequestHandler.h"
#include <vector>
#include <ctime>
#include <iostream>

enum REQUEST_CODES {
	loginRequestCode = 1,
	signupRequestCode = 2,
	createRoomRequestCode = 3,
	getRoomsRequestCode = 4,
	getPlayersInRoomRequestCode = 5,
	joinRoomRequestCode = 6,
	getStatisticsRequestCode = 7,
	logoutRequestCode = 8,
	getHighScoreRequestCode = 9,
	closeRoomRequestCode = 10,
	startGameRequestCode = 11,
	getRoomStateRequestCode = 12,
	getQuestionRequestCode = 13,
	submitAnswerRequestCode = 14,
	getGameResultsRequestCode = 15,
	leaveGameRequestCode = 16
};

struct LoginRequest
{
	std::string username;
	std::string password;
};

struct SignupRequest
{
	std::string username;
	std::string password;
	std::string email;
};

struct GetPlayersInRoomRequest
{
	unsigned int roomId;
};

struct JoinRoomRequest
{
	unsigned int roomId;
};

struct CreateRoomRequest
{
	std::string roomName;
	unsigned int maxUsers;
	unsigned int questionCount;
	unsigned int answerTimeout;
};

struct SubmitAnswerRequest
{
	unsigned int answerId;
};





class JsonRequestPacketDeserializer : IRequestHandler
{
public:
	static LoginRequest deserializeLoginRequest(const std::vector<unsigned char>& Buffer);
	static SignupRequest deserializeSingnupRequest(const std::vector<unsigned char>& Buffer);
	static GetPlayersInRoomRequest deserializeGetPlayersRequest(const std::vector<unsigned char>& Buffer);
	static JoinRoomRequest deserializeJoinRoomRequest(const std::vector<unsigned char>& Buffer);
	static CreateRoomRequest deserializeCreateRoomRequest(const std::vector<unsigned char>& Buffer);
	static SubmitAnswerRequest deserializeSubmitAnswerRequest(const std::vector<unsigned char>& Buffer);

};
