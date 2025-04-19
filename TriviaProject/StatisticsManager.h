#pragma once
#include <vector>
#include <memory>
#include <string>
#include "IDatabase.h"

class StatisticsManager
{
public:
    StatisticsManager(std::shared_ptr<IDatabase> db);
    std::vector<std::string> getHighScore() const;
    std::vector<std::string> getUserStatistics(const std::string& username) const;

private:
    std::shared_ptr<IDatabase> _m_database;
};
