#include "server.h"

namespace server
{
Room::Room(int room_id, Player creator) : owner(creator), id(room_id)
{
    room_players.push_back(owner);
}
}  // namespace server