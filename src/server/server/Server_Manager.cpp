#include <iostream>

#include "server.h"

namespace server
{
Server_Manager::Server_Manager() {}

std::string Server_Manager::create_player()
{
    std::string session_token = generate_session_token();
    std::string name          = "Player" + std::to_string(connected_players.size());
    connected_players.push_back(Player(name, session_token));
    std::cout << "Created player: " << connected_players.back().get_name() << std::endl;
    return session_token;
}

Player Server_Manager::get_player(std::string session_token)
{
    for (auto& player : connected_players)
    {
        if (player.get_session_token() == session_token)
        {
            return player;
        }
    }
    throw std::runtime_error("Player with session token not found");
}

std::string Server_Manager::generate_session_token()
{
    static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::string       id;
    for (int i = 0; i < 16; ++i)
    {
        id += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return id;
}
int Server_Manager::create_room(std::string creator_session_token)
{
    std::cout << "Creating room" << std::endl;
    rooms.emplace_back(rooms.size(), get_player(creator_session_token));
    return 0;
}

void Server_Manager::join_room(int room_id, std::string player_session_token)
{
    // find room
    for (auto& room : rooms)
    {
        if (room.id == room_id)
        {
            room.room_players.push_back(get_player(player_session_token));
            return;
        }
    }
    throw std::runtime_error("Room with id not found");
}

void Server_Manager::exit_room(int room_id, std::string player_session_token)
{
    // find room
    for (auto& room : rooms)
    {
        if (room.id == room_id)
        {
            if (room.owner.get_session_token() == player_session_token)
            {
                throw std::runtime_error("Owner cannot exit the room");
            }
            for (auto it = room.room_players.begin(); it != room.room_players.end(); ++it)
            {
                if (it->get_session_token() == player_session_token)
                {
                    room.room_players.erase(it);
                    return;
                }
            }
        }
    }
    throw std::runtime_error("Room with id not found");
}

bool Server_Manager::verify_session_token(std::string session_token)
{
    for (auto& player : connected_players)
    {
        if (player.get_session_token() == session_token)
        {
            return true;
        }
    }
    return false;
}
}  // namespace server