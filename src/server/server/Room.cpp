#include <stdexcept>

#include "server.h"

namespace server
{
Room::Room(int room_id, Player creator)
    : id(room_id), owner(creator), state(Room_State::WAITING), max_players(4)
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
    if (player.get_player_type() == state::Human) {
        for (auto& player_in_room : room_players)
        {
            if (player_in_room.get_session_token() == player.get_session_token())
            {
                throw std::runtime_error("Player already in the room");
            }
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

bool Room::is_full()
{
    return room_players.size() == max_players;
}

std::string Room::get_infos(std::string session_token)
{
    std::string state = owner.get_name() + "," + std::to_string(static_cast<int>(owner.get_player_type())) + "/";

    for (auto& player : room_players)
    {
        if (player.get_session_token() == owner.get_session_token())
        {
            continue;
        }
        state += player.get_name() + "," + std::to_string(static_cast<int>(player.get_player_type())) + "/";
    }

    // adding player_id
    for (int i = 0; i < room_players.size(); ++i)
    {
        if (room_players[i].get_session_token() == session_token)
        {
            state += std::to_string(i);
            break;
        }
    }

    return state;
}

std::pair<std::vector<std::string>, std::vector<state::Player_Type>> Room::get_start_infos()
{
    std::vector<std::string> player_names;
    std::vector<state::Player_Type> player_types;
    for (auto& player : room_players)
    {
        player_names.push_back(player.get_name());
        player_types.emplace_back(player.get_player_type());
    }

    return {player_names, player_types};
}

Room_State Room::get_state()
{
    return state;
}

void Room::set_state(Room_State new_state)
{
    state = new_state;
}
}  // namespace server