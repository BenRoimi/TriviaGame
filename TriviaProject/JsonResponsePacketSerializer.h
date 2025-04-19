#pragma once
#include <vector>
#include "Room.h"
#include <string>
#include <map>

enum ROOM_RESPONSE_CODES {
	loginCode = 48, // '0'
	signupCode = 49,
	errorCode = 50,
	logoutCode = 51,
	joinRoomCode = 52,
	createRoomCode = 53,
	getRoomsCode = 54,
	getPlayersCode = 55,
	getHighScoreCode = 56,
	getPersonalStatsCode = 57,
	closeRoomCode = 58,
	startGameCode = 59,
	getRoomStateCode = 60,
	leaveRoomCode = 61,
	getGameResultsCode = 62,
	submitAnswerCode = 63,
	getQuestionCode = 64,
	leaveGameCode = 65
};

enum STATUS_STATE {
	SUCCESS = 1,
	FAIL = 0
};

struct LoginResponse {
	STATUS_STATE status;
};

struct SignupResponse {
	STATUS_STATE status;
};

struct ErrorResponse {
	std::string message;
};

struct LogoutResponse {
	STATUS_STATE status;
};

struct JoinRoomResponse {
	STATUS_STATE status;
};

struct CreateRoomResponse {
	STATUS_STATE status;
};

struct GetRoomsResponse {
	STATUS_STATE status;
	std::vector<RoomData> rooms;
};

struct GetPlayersInRoomResponse {
	std::vector<std::string> players;
};

struct GetHighScoreResponse {
	STATUS_STATE status;
	std::vector<std::string> statistics;
};

struct GetPersonalStatsResponse {
	STATUS_STATE status;
	std::vector<std::string> statistics;
};

struct CloseRoomResponse {
	STATUS_STATE status;
};

struct StartGameResponse {
	STATUS_STATE status;
};

struct GetRoomStateResponse {
	STATUS_STATE status;
	ROOM_STATUS roomState;
	std::vector<std::string> players;
	unsigned int questionCount;
	unsigned int answerTimeout;
};

struct LeaveRoomResponse {
	STATUS_STATE status;
};

struct PlayerResults {
	std::string username;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	unsigned int averageAnswerTime;
};

struct GetGameResultsResponse {
	STATUS_STATE status;
	std::vector<PlayerResults> results;
};

struct SubmitAnswerResponse {
	STATUS_STATE status;
	unsigned int correctAnswerId;
};

struct GetQuestionResponse {
	STATUS_STATE status;
	std::string question;
	std::map<unsigned int, std::string> answers;
};

struct LeaveGameResponse {
	STATUS_STATE status;
};

class JsonResponsePacketSerializer
{
public:
	static std::vector<unsigned char> serializeResponse(const LoginResponse& loginResp);
	static std::vector<unsigned char> serializeResponse(const SignupResponse& signupResp);
	static std::vector<unsigned char> serializeResponse(const ErrorResponse& errorResp);
	static std::vector<unsigned char> serializeResponse(const LogoutResponse& logoutResp);
	static std::vector<unsigned char> serializeResponse(const JoinRoomResponse& joinRoomResp);
	static std::vector<unsigned char> serializeResponse(const CreateRoomResponse& createRoomResp);
	static std::vector<unsigned char> serializeResponse(const GetRoomsResponse& getRoomsResp);
	static std::vector<unsigned char> serializeResponse(const GetPlayersInRoomResponse& getPlayersResp);
	static std::vector<unsigned char> serializeResponse(const GetHighScoreResponse& getHighScoreResp);
	static std::vector<unsigned char> serializeResponse(const GetPersonalStatsResponse& getPersonalStatsResp);
	static std::vector<unsigned char> serializeResponse(const CloseRoomResponse& closeRoomResp);
	static std::vector<unsigned char> serializeResponse(const StartGameResponse& startGameResp);
	static std::vector<unsigned char> serializeResponse(const GetRoomStateResponse& getRoomStateResp);
	static std::vector<unsigned char> serializeResponse(const LeaveRoomResponse& leaveRoomResp);
	static std::vector<unsigned char> serializeResponse(const GetGameResultsResponse& getGameResultsResp);
	static std::vector<unsigned char> serializeResponse(const SubmitAnswerResponse& submitAnswerResp);
	static std::vector<unsigned char> serializeResponse(const GetQuestionResponse& getQuestionResp);
	static std::vector<unsigned char> serializeResponse(const LeaveGameResponse& leaveGameResp);
};