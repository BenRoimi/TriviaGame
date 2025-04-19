#include "Question.h"

Question::Question(const std::string& question, const std::vector<std::string>& possibleAnswers, int correctAnswerIndex)
{
	m_question = question;
	m_possibleAnswers = possibleAnswers;
	m_correctAnswerIndex = correctAnswerIndex;
}
