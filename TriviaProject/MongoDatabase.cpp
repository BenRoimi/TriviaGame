#include "MongoDatabase.h"

MongoDatabase::MongoDatabase()
    : client(mongocxx::uri{})
{
    db = client["mongodatabase_trivia"];
}

MongoDatabase::~MongoDatabase()
{
    // MongoDB driver manages connections automatically, so no explicit close needed
}

bool MongoDatabase::open()
{
    // MongoDB connects automatically when needed, so typically no explicit open() needed
    return true;
}

void MongoDatabase::close()
{
    // MongoDB driver manages connections and closes them automatically
}

bool MongoDatabase::doesUserExist(const std::string& user) const
{
    try {
        auto collection = db["users"];
        auto result = collection.find_one(bsoncxx::builder::stream::document{} << "name" << user << bsoncxx::builder::stream::finalize);
        return result ? true : false;
    }
    catch (const mongocxx::exception& e) {
        std::cerr << "MongoDB Error: " << e.what() << std::endl;
        return false;
    }
}

bool MongoDatabase::doesPasswordMatch(const std::string& username, const std::string& password) const
{
    try {
        auto collection = db["users"];
        auto result = collection.find_one(bsoncxx::builder::stream::document{} << "name" << username << "password" << password << bsoncxx::builder::stream::finalize);
        return result ? true : false;
    }
    catch (const mongocxx::exception& e) {
        std::cerr << "MongoDB Error: " << e.what() << std::endl;
        return false;
    }
}

bool MongoDatabase::addNewUser(const std::string& username, const std::string& password, const std::string& mail)
{
    try {
        auto collection = db["users"];
        auto result = collection.insert_one(
            bsoncxx::builder::stream::document{} << "name" << username << "password" << password << "mail" << mail << bsoncxx::builder::stream::finalize
        );
        return result ? true : false;
    }
    catch (const mongocxx::exception& e) {
        std::cerr << "MongoDB Error: " << e.what() << std::endl;
        return false;
    }
}

float MongoDatabase::getPlayerAverageAnswerTime(const std::string& username) const
{
    try {
        auto collection = db["statistics"];
        auto result = collection.find_one(bsoncxx::builder::stream::document{} << "user_name" << username << bsoncxx::builder::stream::finalize);
        if (result) {
            auto view = result->view();
            auto avgAnswerTime = view["avg_answer_time"];
            if (avgAnswerTime) {
                return std::stof(avgAnswerTime.get_utf8().value.to_string());
            }
        }
    }
    catch (const mongocxx::exception& e) {
        std::cerr << "MongoDB Error: " << e.what() << std::endl;
    }
    return 0.0f;  // Default return value (handle error case)
}

int MongoDatabase::getNumOfCorrectAnswers(const std::string& username) const
{
    try {
        auto collection = db["statistics"];
        auto result = collection.find_one(bsoncxx::builder::stream::document{} << "user_name" << username << bsoncxx::builder::stream::finalize);
        if (result) {
            auto view = result->view();
            auto correctAnswers = view["correct_answer_count"];
            if (correctAnswers) {
                return std::stoi(correctAnswers.get_utf8().value.to_string());
            }
        }
    }
    catch (const mongocxx::exception& e) {
        std::cerr << "MongoDB Error: " << e.what() << std::endl;
    }
    return 0;  // Default return value (handle error case)
}

int MongoDatabase::getNumOfTotalAnswers(const std::string& username) const
{
    try {
        auto collection = db["statistics"];
        auto result = collection.find_one(bsoncxx::builder::stream::document{} << "user_name" << username << bsoncxx::builder::stream::finalize);
        if (result) {
            auto view = result->view();
            auto totalAnswers = view["total_answer_count"];
            if (totalAnswers) {
                return std::stoi(totalAnswers.get_utf8().value.to_string());
            }
        }
    }
    catch (const mongocxx::exception& e) {
        std::cerr << "MongoDB Error: " << e.what() << std::endl;
    }
    return 0;  // Default return value (handle error case)
}

int MongoDatabase::getNumOfPlayerGames(const std::string& username) const
{
    try {
        auto collection = db["statistics"];
        auto result = collection.find_one(bsoncxx::builder::stream::document{} << "user_name" << username << bsoncxx::builder::stream::finalize);
        if (result) {
            auto view = result->view();
            auto numOfGames = view["number_of_games"];
            if (numOfGames) {
                return std::stoi(numOfGames.get_utf8().value.to_string());
            }
        }
    }
    catch (const mongocxx::exception& e) {
        std::cerr << "MongoDB Error: " << e.what() << std::endl;
    }
    return 0;  // Default return value (handle error case)
}

int MongoDatabase::getPlayerScore(const std::string& username) const
{
    try {
        auto collection = db["statistics"];
        auto result = collection.find_one(bsoncxx::builder::stream::document{} << "user_name" << username << bsoncxx::builder::stream::finalize);
        if (result) {
            auto view = result->view();
            auto score = view["score"];
            if (score) {
                return std::stoi(score.get_utf8().value.to_string());
            }
        }
    }
    catch (const mongocxx::exception& e) {
        std::cerr << "MongoDB Error: " << e.what() << std::endl;
    }
    return 0;  // Default return value (handle error case)
}

std::vector<std::string> MongoDatabase::getHighScore() const
{
    std::vector<std::string> highScores;
    try {
        auto collection = db["statistics"];
        mongocxx::options::find opts;
        opts.sort(bsoncxx::from_json(R"({ "score" : -1 })"));  // Sort by score descending
        opts.limit(5);  // Limit to top 5 scores

        auto cursor = collection.find({}, opts);
        for (auto&& doc : cursor) {
            auto view = doc.view();
            auto username = view["user_name"].get_utf8().value.to_string();
            auto score = view["score"].get_int32().value.to_string();
            highScores.push_back(username + " : " + score);
        }
    }
    catch (const mongocxx::exception& e) {
        std::cerr << "MongoDB Error: " << e.what() << std::endl;
    }
    return highScores;
}
