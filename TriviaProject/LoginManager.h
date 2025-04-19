#pragma once
#include "IDatabase.h"
#include "LoggedUser.h"
#include <vector>
#include <memory>


class LoginManager 
{
public:
	LoginManager(std::shared_ptr<IDatabase> db);
	void logout(const std::string& userName);
	bool login(const std::string& userName, const std::string& password);
	bool signup(const std::string& userName, const std::string& password, const std::string& email);

private:
	std::shared_ptr<IDatabase> _m_database;
	std::vector<LoggedUser> _m_loggedUsers;
		
	bool checkSignupInfo(const std::string& username, const std::string& password, const std::string& email);
};

