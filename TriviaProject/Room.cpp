#include "Room.h"

void Room::addUser(const LoggedUser& user)
{
	m_users.push_back(user);
}

void Room::removeUser(const LoggedUser& user)
{
	m_users.erase(std::remove(m_users.begin(), m_users.end(), user), m_users.end());
}

std::vector<std::string> Room::getAllUsers() const
{
	std::vector<std::string> users;

	for (const auto& user : m_users) {
		users.push_back(user.getUsername());
	}

	return users;
}
