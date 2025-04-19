#include "SqliteDatabase.h"
#include <io.h>
#include <iostream>
#include <functional>


SqliteDatabase::SqliteDatabase()
{
    open();
}

SqliteDatabase::~SqliteDatabase()
{
    close();
}

bool SqliteDatabase::open()
{
    {
        std::unique_lock<std::shared_mutex> lock(databaseMutex);
        int res = sqlite3_open(DB_FILE_NAME, &db);
        if (res != SQLITE_OK) {
            std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
            return false;
        }

        const char* createUsersTableQuery = "CREATE TABLE IF NOT EXISTS USERS (NAME INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, PASSWORD TEXT NOT NULL, MAIL TEXT NOT NULL); ";
        char* errMessage = nullptr;
        res = sqlite3_exec(db, createUsersTableQuery, nullptr, nullptr, &errMessage);
        if (res != SQLITE_OK) {
            return false;
        }

        const char* sqlStatement = "CREATE TABLE IF NOT EXISTS STATISTICS (USER_NAME TEXT NOT NULL COLLATE UTF16CI,  AVG_ANSWER_TIME	TEXT NOT NULL, TOTAL_ANSWER_COUNT INTEGER NOT NULL, ANSWER_COUNT INTEGER, CORRECT_ANSWER_COUNT INTEGER, NUMBER_OF_GAME INTEGER, SCORE INTEGER, FOREIGN KEY(\"USER_NAME\") REFERENCES \"USERS\"(\"NAME\")); ";
        errMessage = nullptr;
        res = sqlite3_exec(db, sqlStatement, nullptr, nullptr, &errMessage);
        if (res != SQLITE_OK) {
            return false;
        }

        return true;
    }
}

void SqliteDatabase::close()
{
    std::unique_lock<std::shared_mutex> lock(databaseMutex);
    sqlite3_close(db);
    db = nullptr;

}

bool SqliteDatabase::doesUserExist(const std::string& user) const
{
    auto callback = [](void* data, int argc, char** argv, char** azColName) -> int {
        bool* doesExists = (bool*)data;

        if (argc > 0 && argv[0] != nullptr) {
            (*doesExists) = true;
        }
        else {
            (*doesExists) = false;
        }

        return 0;
        };

    std::shared_lock<std::shared_mutex> lock(databaseMutex);
    std::string sqlStatementStr = "SELECT NAME FROM USERS WHERE NAME = \"" + user + "\" LIMIT 1; ";
    const char* sqlStatement = sqlStatementStr.c_str();
    char* errMessage = nullptr;
    bool doesExists = false;
    int res = sqlite3_exec(db, sqlStatement, callback, &doesExists, &errMessage);
    if (res != SQLITE_OK) {
        std::cerr << "Something went wrong" << std::endl;
        return false;
    }

    return doesExists;
}

bool SqliteDatabase::doesPasswordMatch(const std::string& username, const std::string& password) const
{
    auto callback = [](void* data, int argc, char** argv, char** azColName) -> int {

        std::pair<bool*, std::string>* passwordFlagPair = (std::pair<bool*, std::string>*)data;
        bool* foundPassword = passwordFlagPair->first;
        std::string password = passwordFlagPair->second;

        for (int i = 0; i < argc; i++) {
            if (std::string(azColName[i]) == "PASSWORD") {
                if (argv[i] == password) {
                    *foundPassword = true;
                }
            }
        }

        return 0;
        };

    std::shared_lock<std::shared_mutex> lock(databaseMutex);
    std::string sqlStatementStr = "SELECT PASSWORD FROM USERS WHERE NAME = \"" + username + "\"; ";
    const char* sqlStatement = sqlStatementStr.c_str();
    char* errMessage = nullptr;
    bool foundPassword = false;
    std::pair<bool*, std::string> data(&foundPassword, password);
    int res = sqlite3_exec(db, sqlStatement, callback, &data, &errMessage);
    if (res != SQLITE_OK) {
        std::cerr << "Something went wrong" << std::endl;
        return false;
    }

    return foundPassword;
}

bool SqliteDatabase::addNewUser(const std::string& username, const std::string& password, const std::string& mail)
{
    std::unique_lock<std::shared_mutex> lock(databaseMutex);
    std::string sqlStatementStr = "INSERT INTO USERS (NAME, PASSWORD, MAIL) VALUES (\"" + username + "\", \"" + password + "\", \"" + mail + "\") ;";
    const char* sqlStatement = sqlStatementStr.c_str();
    char* errMessage = nullptr;
    int res = sqlite3_exec(db, sqlStatement, nullptr, nullptr, &errMessage);
    if (res != SQLITE_OK) {
        std::cerr << "Could not insert to USERS" << std::endl;
        return false;
    }
    return true;
}

static int averageCallback(void* data, int argc, char** argv, char** azColName) {
    if (argc == 1 && argv[0]) {
        *(float*)data = std::stof(argv[0]);
    }
    return 0;
}

static int answerCallback(void* data, int argc, char** argv, char** azColName) {
    if (argc == 1 && argv[0]) {
        *(int*)data = std::stoi(argv[0]);
    }
    return 0;
}

float SqliteDatabase::getPlayerAverageAnswerTime(const std::string& username) const
{
    std::shared_lock<std::shared_mutex> lock(databaseMutex);
    float average;
    char* errMessage = nullptr;
    std::string sqlStatment = "SELECT AVG_ANSWER_TIME FROM STATISTICS WHERE USER_NAME = \"" + username + "\"; ";
    int res = sqlite3_exec(db, sqlStatment.c_str(), averageCallback, &average, &errMessage);
    if (res != SQLITE_OK)
    {
        std::cerr << "Error : " << errMessage << std::endl;
        sqlite3_free(errMessage);
    }
    return average;
}

int SqliteDatabase::getNumOfCorrectAnswers(const std::string& username) const
{
    std::shared_lock<std::shared_mutex> lock(databaseMutex);
    int answerCounters = 0;
    char* errMessage = nullptr;
    std::string sqlStatment = "SELECT CORRECT_ANSWER_COUNT FROM STATISTICS WHERE USER_NAME = \"" + username + "\"; ";
    int res = sqlite3_exec(db, sqlStatment.c_str(), answerCallback, &answerCounters, &errMessage);
    if (res != SQLITE_OK)
    {
        std::cerr << "Error : " << errMessage << std::endl;
        sqlite3_free(errMessage);
    }
    return answerCounters;
}

int SqliteDatabase::getNumOfTotalAnswers(const std::string& username) const
{
    std::shared_lock<std::shared_mutex> lock(databaseMutex);
    int answerCounters = 0;
    char* errMessage = nullptr;
    std::string sqlStatment = "SELECT TOTAL_ANSWER_COUNT FROM STATISTICS WHERE USER_NAME = \"" + username + "\"; ";
    int res = sqlite3_exec(db, sqlStatment.c_str(), answerCallback, &answerCounters, &errMessage);
    if (res != SQLITE_OK)
    {
        std::cerr << "Error : " << errMessage << std::endl;
        sqlite3_free(errMessage);
    }
    return answerCounters;
}

int SqliteDatabase::getNumOfPlayerGames(const std::string& username) const
{
    std::shared_lock<std::shared_mutex> lock(databaseMutex);
    int answerCounters = 0;
    char* errMessage = nullptr;
    std::string sqlStatment = "SELECT NUMBER_OF_GAME FROM STATISTICS WHERE USER_NAME = \"" + username + "\"; ";
    int res = sqlite3_exec(db, sqlStatment.c_str(), answerCallback, &answerCounters, &errMessage);
    if (res != SQLITE_OK)
    {
        std::cerr << "Error : " << errMessage << std::endl;
        sqlite3_free(errMessage);
    }
    return answerCounters;
}

int SqliteDatabase::getPlayerScore(const std::string& username) const
{
    std::shared_lock<std::shared_mutex> lock(databaseMutex);
    int answerCounters = 0;
    char* errMessage = nullptr;
    std::string sqlStatment = "SELECT SCORE FROM STATISTICS WHERE USER_NAME = \"" + username + "\"; ";
    int res = sqlite3_exec(db, sqlStatment.c_str(), answerCallback, &answerCounters, &errMessage);
    if (res != SQLITE_OK)
    {
        std::cerr << "Error : " << errMessage << std::endl;
        sqlite3_free(errMessage);
    }
    return answerCounters;
}
static int highScoreCallback(void* data, int argc, char** argv, char** azColName) {
    std::vector<std::string>* highScores = reinterpret_cast<std::vector<std::string>*>(data);
    if (argc == 2) { // Expecting two columns: USER_NAME and SCORE
        std::string username = argv[0] ? argv[0] : "";
        std::string score = argv[1] ? argv[1] : "0";
        highScores->push_back(username + " : " + score);
    }
    return 0;
}

std::vector<std::string> SqliteDatabase::getHighScore()const {
    std::shared_lock<std::shared_mutex> lock(databaseMutex);
    std::vector<std::string> highScores;
    const char* sqlStatement = "SELECT USER_NAME, SCORE FROM STATISTICS ORDER BY SCORE DESC LIMIT 5;";
    char* errMessage = nullptr;

    int res = sqlite3_exec(db, sqlStatement, highScoreCallback, &highScores, &errMessage);
    if (res != SQLITE_OK) {
        std::cerr << "SQL error: " << errMessage << std::endl;
        sqlite3_free(errMessage);
    }

    return highScores;
}

std::list<Question> SqliteDatabase::getQuestions(const int questionsAmount) const
{
    auto callback = [](void* data, int argc, char** argv, char** azColName) -> int {
        std::list<Question>* questions = (std::list<Question>*)data;
        std::string question;
        int correctAnswerIndex;
        std::string answers_str;

        for (int i = 0; i < argc; i++) {
            if (std::string(azColName[i]) == "question") {
                question = argv[i];
            }
            else if (std::string(azColName[i]) == "answers") {
                answers_str = argv[i];
            }
            else if (std::string(azColName[i]) == "correct_answer_index") {
                correctAnswerIndex = std::stoi(argv[i]);
            }
        }

        std::vector<std::string> answers;
        std::string temp = "";
        char del = ',';
        for (int i = 0; i < answers_str.size(); i++) {
            if (answers_str[i] != del) {
                temp += answers_str[i];
            }
            else {
                answers.push_back(temp);
                temp = "";
            }
        }

        questions->push_back(Question(question, answers, correctAnswerIndex));

        return 0;
        };

    std::list<Question> questions;
    char* errMessage = nullptr;
    std::string sqlStatment = "SELECT * FROM QUESTIONS LIMIT " + std::to_string(questionsAmount) + "; ";
    int res = sqlite3_exec(db, sqlStatment.c_str(), callback, &questions, &errMessage);
    if (res != SQLITE_OK)
    {
        std::cerr << "Error : " << errMessage << std::endl;
        sqlite3_free(errMessage);
    }

    return questions;
}


bool SqliteDatabase::submitGameStatistics(const std::string& user, const GameData& data) const
{
    int playedGames = getNumOfPlayerGames(user);
    int score = (data.wrongAnswerCount + data.correctAnswerCount + playedGames) / (data.wrongAnswerCount * data.averageAnswerTime); // score is created integer in open function
    std::string sqlStatementStr =
        "INSERT INTO STATISTICS (USER_NAME, AVG_ANSWER_TIME, TOTAL_ANSWER_COUNT, ANSWER_COUNT, CORRECT_ANSWER_COUNT, NUMBER_OF_GAME, SCORE) VALUES (\"" + user + "\", \"" + std::to_string(data.averageAnswerTime) + "\", \"" + std::to_string(data.wrongAnswerCount + data.correctAnswerCount) + "\", \"" + std::to_string(data.wrongAnswerCount) + "\", \"" + std::to_string(data.correctAnswerCount) + "\", \"" + std::to_string(playedGames + 1) + "\", \"" + std::to_string(score) + "\") ;";
    // i assumed ANSWER_COUNT is a mistake for wrong answer count
    const char* sqlStatement = sqlStatementStr.c_str();
    char* errMessage = nullptr;
    int res = sqlite3_exec(db, sqlStatement, nullptr, nullptr, &errMessage);

    if (res != SQLITE_OK) {
        std::cerr << "Could not insert to STATISTICS" << std::endl;
        return false;
    }

    return true;
}

