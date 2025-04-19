#include "GameRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"

GameRequestHandler::GameRequestHandler(Game& game, const LoggedUser& user, GameManager& gameManager, RequestHandlerFactory& handlerFactory) 
	: m_game(game), m_user(user), m_gameManager(gameManager), m_handlerFactory(handlerFactory)
{
}

bool GameRequestHandler::isRequestRelevant(const RequestInfo& requestInfo) const
{
	int code = requestInfo.RequestId;

	return ((code == getQuestionRequestCode) || (code == submitAnswerRequestCode) ||
		(code == getGameResultsRequestCode) || (code == leaveGameRequestCode));
}

RequestResult GameRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
    int code = requestInfo.RequestId;

    switch (code)
    {
    case getQuestionRequestCode:
        return getQuestion(requestInfo);
    case submitAnswerRequestCode:
        return submitAnswer(requestInfo);
    case getGameResultsRequestCode:
        return getGameResults(requestInfo);
    case leaveGameRequestCode:
        return leaveGame(requestInfo);
    default:
        return createErrorRequestResult();
    }
}

RequestResult GameRequestHandler::getQuestion(const RequestInfo& requestInfo) const
{
    std::string question = m_game.getQuestionForUser(m_user).getQuestion();
    std::map<unsigned int, std::string> answers;

    const auto& possibleAnswers = m_game.getQuestionForUser(m_user).getPossibleAnswers();
    unsigned int counter = 1;
    for (const std::string& answer : possibleAnswers) {
        answers[counter] = answer;
        counter++;
    }

	return createGetQuestionRequestResult(question, answers);
}

RequestResult GameRequestHandler::submitAnswer(const RequestInfo& requestInfo) const
{
    unsigned int correctAnswerId = m_game.getQuestionForUser(m_user).getCorrectAnswerIndex();

	return createSubmitAnswerRequestResult(correctAnswerId);
}

RequestResult GameRequestHandler::getGameResults(const RequestInfo& requestInfo)
{
    std::vector<PlayerResults> results;
    std::vector<std::string> users = m_handlerFactory.getRoomManager().getRoom(m_game.getGameID()).getAllUsers();

    for (const auto& user : users) {
        std::vector<std::string> userStatistics = m_handlerFactory.getStatisticsManager().getUserStatistics(user);
        unsigned int correctAnsCount = std::stoi(userStatistics[2]);
        unsigned int wrongAnsCount = std::stoi(userStatistics[3]) - correctAnsCount;
        unsigned int AvgAnswerTime = std::stoi(userStatistics[1]);
        results.push_back(PlayerResults{ user, correctAnsCount, wrongAnsCount, AvgAnswerTime });
    }

	return createGetGameResultsRequestResult(results);
}

RequestResult GameRequestHandler::leaveGame(const RequestInfo& requestInfo) const
{
    m_game.removePlayer(m_user);

	return createLeaveGameRequestResult();
}

RequestResult GameRequestHandler::createErrorRequestResult() const
{
    return RequestResult{ JsonResponsePacketSerializer::serializeResponse
        (ErrorResponse{ "No relevant response" }), nullptr };
}

RequestResult GameRequestHandler::createGetQuestionRequestResult(const std::string& question, const std::map<unsigned int, std::string>& answers) const
{
    return RequestResult{ JsonResponsePacketSerializer::serializeResponse
        (GetQuestionResponse{ SUCCESS, question, answers }), m_handlerFactory.createGameRequestHandler(m_user, m_game)};
}

RequestResult GameRequestHandler::createSubmitAnswerRequestResult(const unsigned int& correctAnswerId) const
{
    return RequestResult{ JsonResponsePacketSerializer::serializeResponse
        (SubmitAnswerResponse{ SUCCESS, correctAnswerId }), m_handlerFactory.createGameRequestHandler(m_user, m_game) };
}

RequestResult GameRequestHandler::createGetGameResultsRequestResult(std::vector<PlayerResults> results) const
{
    return RequestResult{ JsonResponsePacketSerializer::serializeResponse
    (GetGameResultsResponse{ SUCCESS, results }), m_handlerFactory.createGameRequestHandler(m_user, m_game) };
}

RequestResult GameRequestHandler::createLeaveGameRequestResult() const
{
    return RequestResult{ JsonResponsePacketSerializer::serializeResponse
        (LeaveGameResponse{ SUCCESS }), m_handlerFactory.createMenuRequestHandler(m_user)};
}
