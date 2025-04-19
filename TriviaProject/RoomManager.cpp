#include "RoomManager.h"

void RoomManager::createRoom(const LoggedUser& user, const RoomData& data)
{
	std::vector<LoggedUser> users(1, user);
	Room newRoom(data, users);

	m_rooms[data.id] = newRoom;
}

void RoomManager::deleteRoom(const int roomID)
{
	m_rooms.erase(roomID);
}

ROOM_STATUS RoomManager::getRoomState(const int roomID)
{
	if (m_rooms.find(roomID) == m_rooms.end()) {
		throw RoomNotFoundException();
	}

	return m_rooms[roomID].getRoomData().status;
}

std::vector<RoomData> RoomManager::getRooms() const
{
	std::vector<RoomData> rooms;

	for (const auto& room : m_rooms) {
		rooms.push_back(room.second.getRoomData());
	}

	return rooms;
}

Room& RoomManager::getRoom(const int roomID)
{
	if (m_rooms.find(roomID) == m_rooms.end()) {
		throw RoomNotFoundException();
	}

	return m_rooms[roomID];
}
