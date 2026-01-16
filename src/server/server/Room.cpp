#include <stdexcept>

#include "server.h"

namespace server
{
Room::Room(int room_id, Player creator) : owner(creator), id(room_id)
{
    room_players.push_back(owner);
}

Room::~Room()
{
    for (auto& player : room_players)
    {
        player.set_room(-1);
    }
}

void Room::add_player(Player player)
{
    for (auto& player_in_room : room_players)
    {
        if (player_in_room.get_session_token() == player.get_session_token())
        {
            throw std::runtime_error("Player already in the room");
        }
    }

    room_players.push_back(player);
}
void Room::remove_player(Player player)
{
    // transfering ownership if needed
    if (owner.get_session_token() == player.get_session_token())
    {
        if (room_players.size() > 1)
        {
            owner = room_players.at(1);
        }
    }

    // removing the player from the list
    for (auto it = room_players.begin(); it != room_players.end(); ++it)
    {
        if (it->get_session_token() == player.get_session_token())
        {
            room_players.erase(it);
            return;
        }
    }
    throw std::runtime_error("Player not found in the room");
}

Player Room::get_owner()
{
    return owner;
}

bool Room::is_empty()
{
    return room_players.empty();
}

std::string Room::get_state()
{
    std::string state =
        "Room " + std::to_string(id) + " owned by " + owner.get_name() + " with players: ";
    for (auto& player : room_players)
    {
        state += player.get_name() + " ";
    }
    return state;
}

}  // namespace server