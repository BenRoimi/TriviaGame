#pragma once
#include <string>
#include <vector>
#include "LoggedUser.h"

enum ROOM_STATUS {
    waiting = 1,
    active = 2,
    unavailable = 3
};

struct RoomData {
    unsigned int id;
    std::string name;
    unsigned int maxPlayers;
    unsigned int numOfQuestionsInGame;
    unsigned int timePerQuestion;
    ROOM_STATUS status;
};

class Room
{
public:
    Room() = default;
    Room(const RoomData& data, const std::vector<LoggedUser>& users) : m_metadata(data), m_users(users) {};
    void addUser(const LoggedUser& user);
    void removeUser(const LoggedUser& user);
    std::vector<std::string> getAllUsers() const;
    RoomData getRoomData() const { return m_metadata; };
private:
    RoomData m_metadata;
    std::vector<LoggedUser> m_users;
};