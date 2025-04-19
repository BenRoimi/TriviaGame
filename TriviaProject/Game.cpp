#include "Game.h"
#include <stdexcept>

Question Game::getQuestionForUser(const LoggedUser& user)
{
	return m_players[user.getUsername()].currentQuestion;
}

void Game::submitAnswer(const LoggedUser& user, const unsigned int answerIndex)
{
	if (m_players[user.getUsername()].currentQuestion.getCorrectAnswerIndex() == answerIndex) {
		m_players[user.getUsername()].correctAnswerCount++;
	}
	else {
		m_players[user.getUsername()].wrongAnswerCount++;
	}
	// change average answer time later
}

void Game::removePlayer(const LoggedUser& user)
{
	int wasErased = m_players.erase(user.getUsername());

	if (wasErased == 0) {
		throw std::runtime_error("Couldn't remove player " + user.getUsername() + ", not found");
	}
}

unsigned int Game::getGameID() const
{
	return m_gameId;
}
