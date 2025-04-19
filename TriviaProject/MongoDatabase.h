#pragma once

#include "IDatabase.h"
#include <iostream>
#include <vector>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/exception/exception.hpp>

class MongoDatabase : public IDatabase
{
private:
    mongocxx::instance instance;  // Required to initialize MongoDB driver
    mongocxx::client client;      // MongoDB client
    mongocxx::database db;        // Database instance

public:
    MongoDatabase();
    ~MongoDatabase() override;

    bool open() override;
    void close() override;
    bool doesUserExist(const std::string& user) const override;
    bool doesPasswordMatch(const std::string& username, const std::string& password) const override;
    bool addNewUser(const std::string& username, const std::string& password, const std::string& mail) override;
    float getPlayerAverageAnswerTime(const std::string& username) const override;
    int getNumOfCorrectAnswers(const std::string& username) const override;
    int getNumOfTotalAnswers(const std::string& username) const override;
    int getNumOfPlayerGames(const std::string& username) const override;
    int getPlayerScore(const std::string& username) const override;
    std::vector<std::string> getHighScore() const override;
};
