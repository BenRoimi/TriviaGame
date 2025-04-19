#include "RequestHandlerFactory.h"
#include "LoginRequestHandler.h"
#include "GameRequestHandler.h"
#include "RoomMemberRequestHandler.h"
#include "RoomAdminRequestHandler.h"
#include "MenuRequestHandler.h"

RequestHandlerFactory::RequestHandlerFactory(std::shared_ptr<IDatabase> db) : m_database(db), m_loginManager(db), m_statisticsManager(db), m_gameManager(db)
{
}

std::unique_ptr<IRequestHandler> RequestHandlerFactory::createLoginRequestHandler() 
{
	return std::make_unique<LoginRequestHandler>(*this);
}

LoginManager& RequestHandlerFactory::getLoginManager()
{
	return m_loginManager;
}

std::unique_ptr<IRequestHandler> RequestHandlerFactory::createMenuRequestHandler(const LoggedUser& user) 
{
	return std::make_unique<MenuRequestHandler>(user, *this);
}

RoomManager& RequestHandlerFactory::getRoomManager()
{
	return m_roomManager;
}

StatisticsManager& RequestHandlerFactory::getStatisticsManager()
{
	return m_statisticsManager;
}

std::unique_ptr<IRequestHandler> RequestHandlerFactory::createRoomAdminRequestHandler(const LoggedUser& user, const unsigned int& rooId) 
{
	return std::make_unique<RoomAdminRequestHandler>(rooId, user, m_roomManager, *this);
}

std::unique_ptr<IRequestHandler> RequestHandlerFactory::createRoomMemberRequestHandler(const LoggedUser& user, const unsigned int& rooId) 
{
	return std::make_unique<RoomMemberRequestHandler>(rooId, user, m_roomManager, *this);
}

std::unique_ptr<IRequestHandler> RequestHandlerFactory::createGameRequestHandler(const LoggedUser& user,  Game& game) 
{
	return std::make_unique<GameRequestHandler>(game, user, m_gameManager, *this);
}

GameManager& RequestHandlerFactory::getGameManager()
{
	return m_gameManager;
}
