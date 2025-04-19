#pragma once
#include <vector>
#include <string>

class Question {
public:
	Question() = default;
	Question(const std::string& question, const std::vector<std::string>& possibleAnswers, int correctAnswerIndex);

	std::string getQuestion() const { return m_question; }
	std::vector<std::string> getPossibleAnswers() const { return m_possibleAnswers; }
	int getCorrectAnswerIndex() const { return m_correctAnswerIndex; }

private:
	std::string m_question;
	std::vector<std::string> m_possibleAnswers;
	int m_correctAnswerIndex;
};