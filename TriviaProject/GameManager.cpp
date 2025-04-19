#include "GameManager.h"
#include <stdexcept>

Game GameManager::createGame(Room room)
{
    Game newGame = Game(room.getRoomData().id, m_database->getQuestions(room.getRoomData().numOfQuestionsInGame));
    m_games.push_back(newGame);

    return newGame;
}

void GameManager::deleteGame(unsigned int gameId)
{
    auto it = m_games.begin();

    while (it != m_games.end()) {
        if (it->getGameID() == gameId) {
            m_games.erase(it);
            return;
        }
        else {
            it++;
        }
    }

    throw std::runtime_error("Couldn't delete game with ID " + std::to_string(gameId) + ", not found");
}

void GameManager::submitGameStatsToDB(const GameData& gameData, const std::string& user) const
{
    m_database->submitGameStatistics(user, gameData);
}
