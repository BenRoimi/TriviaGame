#pragma once
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "Game.h"
#include "LoggedUser.h"
#include "GameManager.h"
#include "IRequestHandler.h"
#include <vector>
#include "JsonResponsePacketSerializer.h"

class GameRequestHandler : public IRequestHandler {
public:
    GameRequestHandler(Game& game, const LoggedUser& user, GameManager& gameManager, RequestHandlerFactory& handlerFactory);

    bool isRequestRelevant(const RequestInfo& requestInfo) const override;
    RequestResult handleRequest(const RequestInfo& requestInfo) override;

private:
    Game& m_game;
    LoggedUser m_user;
    GameManager& m_gameManager;
    RequestHandlerFactory& m_handlerFactory;

    RequestResult getQuestion(const RequestInfo& requestInfo) const;
    RequestResult submitAnswer(const RequestInfo& requestInfo) const;
    RequestResult getGameResults(const RequestInfo& requestInfo);
    RequestResult leaveGame(const RequestInfo& requestInfo) const;

    RequestResult createErrorRequestResult() const;
    RequestResult createGetQuestionRequestResult(const std::string& question, const std::map<unsigned int, std::string>& answers) const;
    RequestResult createSubmitAnswerRequestResult(const unsigned int& correctAnswerId) const;
    RequestResult createGetGameResultsRequestResult(std::vector<PlayerResults> results) const;
    RequestResult createLeaveGameRequestResult() const;
};