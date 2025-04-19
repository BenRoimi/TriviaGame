#pragma once
#include <vector>
#include <map>
#include <list>
#include "Question.h"
#include "LoggedUser.h"

struct GameData {
    Question currentQuestion;
    unsigned int correctAnswerCount;
    unsigned int wrongAnswerCount;
    unsigned int averageAnswerTime;
};




class Game {
public:
    Game(unsigned int gameId, const std::list<Question>& questions)
        : m_gameId(gameId), m_questions(questions.begin(), questions.end()) {}

    Question getQuestionForUser(const LoggedUser& user);
    void submitAnswer(const LoggedUser& user, const unsigned int answerIndex);
    void removePlayer(const LoggedUser& user);
    unsigned int getGameID() const;

private:
    std::vector<Question> m_questions;
    std::map<std::string, GameData> m_players;
    unsigned int m_gameId;
};
