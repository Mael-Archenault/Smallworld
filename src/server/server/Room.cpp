#include "server.h"

namespace server
{
Room::Room(Player creator, int room_id) : owner(creator), id(room_id)
{
    room_players.push_back(owner);
}
}  // namespace server