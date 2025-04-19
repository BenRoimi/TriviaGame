#pragma once
#include <vector>
#include "Game.h"
#include "IDatabase.h"
#include "Room.h"
#include <memory>

class GameManager {
public:
    GameManager(std::shared_ptr<IDatabase> database) : m_database(database) {}

    Game createGame(Room room);
    void deleteGame(unsigned int gameId);

private:
    std::shared_ptr<IDatabase> m_database;
    std::vector<Game> m_games;

    void submitGameStatsToDB(const GameData& gameData, const std::string& user) const;
};
