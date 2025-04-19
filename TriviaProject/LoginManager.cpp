#include "LoginManager.h"
#include <iostream>
#include <algorithm>
#include <regex>

LoginManager::LoginManager(std::shared_ptr<IDatabase> db)
{
	_m_database = db;
}

void LoginManager::logout(const std::string& userName)
{
	auto it = std::find_if(_m_loggedUsers.begin(), _m_loggedUsers.end(), [&userName](LoggedUser& user)
		{
			return user.getUsername() == userName;
		});

	if (it != _m_loggedUsers.end())
	{
		_m_loggedUsers.erase(std::remove_if(_m_loggedUsers.begin(), _m_loggedUsers.end(), [&userName](LoggedUser& user)
			{
				return user.getUsername() == userName;
			}));
		std::cout << "User " + userName + " was logged out." << std::endl;
	}
	else
	{
		std::cout << "User " + userName + " was not found." << std::endl;
	}
	
}


bool LoginManager::login(const std::string& userName, const std::string& password) 
{
	auto it = std::find_if(_m_loggedUsers.begin(), _m_loggedUsers.end(), [&userName](LoggedUser& user)
		{
			return user.getUsername() == userName;
		});

	if (it == _m_loggedUsers.end())
	{
		if (_m_database->doesUserExist(userName)  && _m_database->doesPasswordMatch(userName, password))
		{
			_m_loggedUsers.emplace_back(userName);
			std::cout << "user " + userName + " login" << std::endl;
			return true;
		}
		else
		{
			std::cout << "user " + userName + " could not login" << std::endl;
			return false;
		}
	}
	else
	{
		std::cout << "user " + userName + " could not login" << std::endl;
		return false;
	}

}

bool LoginManager::signup(const std::string& userName, const std::string& password, const std::string& email) 
{
	if (_m_database->doesUserExist(userName) == true && _m_database->doesPasswordMatch(userName, password) == true)
	{
		std::cout << "user " + userName + " is already registered" << std::endl;
		return false;
	}
	else
	{
		if ((_m_database->addNewUser(userName, password, email) == true) && checkSignupInfo(userName, password, email))
		{
			_m_loggedUsers.emplace_back(userName);
			std::cout << "user " + userName + " was signup" << std::endl;
			return true;
		}
		else
		{
			std::cout << "user " + userName + " could not signup" << std::endl;
			return false;
		}
	}
	
}

bool LoginManager::checkSignupInfo(const std::string& username, const std::string& password, const std::string& email)
{
	std::regex emailPattern("[a-zA-Z0-9]+[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}");
	std::regex usernamePattern(".{4,}");
	// std::regex passwordPattern("((?=.*[A-Z])(?=.*[a-z])(?=.*\d)(?=.*[\*\&\^\%\$\#\@\!])[A-Za-z\d\*\&\^\%\$\#\@\!]{8,})"); // at least 1 - capital, small, special char and number

	//return (std::regex_match(email, emailPattern) &&
	//	std::regex_match(username, usernamePattern) &&
	//	std::regex_match(password, passwordPattern));

	return (std::regex_match(email, emailPattern) &&
		std::regex_match(username, usernamePattern));
}
