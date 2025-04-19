#pragma once
#include "Room.h"
#include <map>
#include <exception>

class RoomManager
{
public:
    void createRoom(const LoggedUser& user, const RoomData& data);
    void deleteRoom(const int roomID);
    ROOM_STATUS getRoomState(const int roomID);
    std::vector<RoomData> getRooms() const;
    Room& getRoom(const int roomID);
private:
    std::map<int, Room> m_rooms; // roomID, Room
};

class RoomNotFoundException : public std::exception {
public:
    const char* what() const noexcept override {
        return "RoomID not found in rooms map";
    }
};