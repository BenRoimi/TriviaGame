#pragma once
#include <vector>
#include <list>
#include "Question.h"
#include "Game.h"

class IDatabase
{
public:
	virtual ~IDatabase() = default;

	virtual bool open() = 0;
	virtual void close() = 0;
	virtual bool doesUserExist(const std::string& user) const = 0;
	virtual bool doesPasswordMatch(const std::string& username, const std::string& password) const = 0;
	virtual bool addNewUser(const std::string& username, const std::string& password, const std::string& mail) = 0;
	virtual std::list<Question> getQuestions(const int questionsAmount) const = 0;
	virtual float getPlayerAverageAnswerTime(const std::string& username) const = 0;
	virtual int getNumOfCorrectAnswers(const std::string& username) const = 0;
	virtual int getNumOfTotalAnswers(const std::string& username) const = 0;
	virtual int getNumOfPlayerGames(const std::string& username) const = 0;
	virtual int getPlayerScore(const std::string& username) const = 0;
	virtual std::vector<std::string> getHighScore()const = 0;
	virtual bool submitGameStatistics(const std::string& user, const GameData& data) const = 0;
};

