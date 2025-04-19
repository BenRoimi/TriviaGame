#include "LoggedUser.h"

LoggedUser::LoggedUser(const std::string& userName)
{
    m_username = userName;
}

std::string LoggedUser::getUsername() const
{
    return m_username;
}

bool LoggedUser::operator==(LoggedUser const& rhs) const
{
    return m_username == rhs.m_username;
}
