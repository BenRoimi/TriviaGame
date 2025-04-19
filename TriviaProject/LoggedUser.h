#pragma once
#include <string>

class LoggedUser 
{
public:
	LoggedUser(const std::string& userName);
	~LoggedUser() = default;
	std::string getUsername() const;
	bool operator==(LoggedUser const& rhs) const;

private:
	std::string m_username;
};

