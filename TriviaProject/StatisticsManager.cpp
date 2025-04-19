#include "StatisticsManager.h"
#include <iostream>
#include <string>

StatisticsManager::StatisticsManager(std::shared_ptr<IDatabase> db)
{
	_m_database = db;
}

std::vector<std::string> StatisticsManager::getHighScore() const
{
	return _m_database->getHighScore();

}

std::vector<std::string> StatisticsManager::getUserStatistics(const std::string& username) const
{
	std::vector<std::string> statistics;
	int userScore = _m_database->getPlayerScore(username);
	statistics.push_back(std::to_string(userScore));
	float time = _m_database->getPlayerAverageAnswerTime(username);
	statistics.push_back(std::to_string(time));
	int correctAns = _m_database->getNumOfCorrectAnswers(username);
	statistics.push_back(std::to_string(correctAns));
	int totalAns = _m_database->getNumOfTotalAnswers(username);
	statistics.push_back(std::to_string(totalAns));
	int games = _m_database->getNumOfPlayerGames(username);
	statistics.push_back(std::to_string(games));

	return statistics;
}

