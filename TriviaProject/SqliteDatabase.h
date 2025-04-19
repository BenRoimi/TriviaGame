#pragma once
#include "IDatabase.h"
#include "sqlite3.h"
#include <shared_mutex>

#define DB_FILE_NAME "triviaDB.sqlite"

class SqliteDatabase : public IDatabase
{
public:
	SqliteDatabase();
	~SqliteDatabase();

	bool open() override;
	void close() override;
	bool doesUserExist(const std::string& user) const override;
	bool doesPasswordMatch(const std::string& username, const std::string& password) const override;
	bool addNewUser(const std::string& username, const std::string& password, const std::string& mail) override;
	std::list<Question> getQuestions(const int questionsAmount) const override;
	float getPlayerAverageAnswerTime(const std::string& username)const override;
	int getNumOfCorrectAnswers(const std::string& username)const override;
	int getNumOfTotalAnswers(const std::string& username)const override;
	int getNumOfPlayerGames(const std::string& username)const override;
	int getPlayerScore(const std::string& username)const override;
	std::vector<std::string> getHighScore()const override;
	bool submitGameStatistics(const std::string& user, const GameData& data) const override;

private:
	sqlite3* db;
	// shared mutex
	mutable std::shared_mutex databaseMutex;
};


