#pragma once
#include "IDatabase.h"
#include "IRequestHandler.h"
#include "LoginManager.h"
#include "StatisticsManager.h"
#include <memory>
#include "RoomManager.h"
#include "GameManager.h"

class RequestHandlerFactory
{
public:
	RequestHandlerFactory(std::shared_ptr<IDatabase> db);
	std::unique_ptr<IRequestHandler> createLoginRequestHandler() ;
	LoginManager& getLoginManager();
	std::unique_ptr<IRequestHandler> createMenuRequestHandler(const LoggedUser& user) ;
	RoomManager& getRoomManager();
	StatisticsManager& getStatisticsManager();
	std::unique_ptr<IRequestHandler> createRoomAdminRequestHandler(const LoggedUser& user, const unsigned int& rooId) ;
	std::unique_ptr<IRequestHandler> createRoomMemberRequestHandler(const LoggedUser& user, const unsigned int& rooId) ;
	std::unique_ptr<IRequestHandler> createGameRequestHandler(const LoggedUser& user,  Game& game) ;
	GameManager& getGameManager();

private:
	LoginManager m_loginManager;
	RoomManager m_roomManager;
	StatisticsManager m_statisticsManager;
	std::shared_ptr<IDatabase> m_database;
	GameManager m_gameManager;
};
