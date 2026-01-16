#include <iostream>

#include "server.h"

namespace server
{
Server_Manager::Server_Manager() : next_room_id(0) {}

std::string Server_Manager::create_player()
{
    std::string session_token = generate_session_token();
    std::string name          = "Player" + std::to_string(connected_players.size());
    connected_players.push_back(Player(name, session_token));
    std::cout << "Created player: " << connected_players.back().get_name() << std::endl;
    return session_token;
}

Player& Server_Manager::get_player(std::string session_token)
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
    Player& creator = get_player(creator_session_token);

    if (creator.get_room() != -1)
    {
        throw std::runtime_error("Player already in a room");
    }
    std::cout << "Creating room : " << next_room_id << std::endl;
    rooms.emplace_back(next_room_id, creator);
    next_room_id++;
    creator.set_room(rooms.back().id);
    return 0;
}

void Server_Manager::join_room(int room_id, std::string player_session_token)
{
    Player& player = get_player(player_session_token);
    if (player.get_room() != -1)
    {
        throw std::runtime_error("Player already in a room");
    }
    // find room
    for (auto& room : rooms)
    {
        if (room.id == room_id)
        {
            room.add_player(player);
            player.set_room(room_id);
            return;
        }
    }
    throw std::runtime_error("Room with id not found");
}

void Server_Manager::exit_room(int room_id, std::string player_session_token)
{
    Player& player = get_player(player_session_token);

    if (player.get_room() == -1)
    {
        throw std::runtime_error("Player not in any room");
    }

    if (player.get_room() != room_id)
    {
        throw std::runtime_error("Player not in the specified room");
    }

    // find room
    for (auto& room : rooms)
    {
        if (room.id == room_id)
        {
            room.remove_player(player);
            get_player(player_session_token).set_room(-1);
            if (room.is_empty())
            {
                // remove room
                std::cout << "Removing empty room with id: " << room_id << std::endl;
                delete_room(room_id);
            }
            return;
        }
    }
    throw std::runtime_error("Room with id not found");
}

std::string Server_Manager::get_room_state(int room_id, std::string session_token)
{
    if (get_player(session_token).get_room() != room_id)
    {
        throw std::runtime_error("Player not in the specified room");
    }
    for (auto& room : rooms)
    {
        if (room.id == room_id)
        {
            return room.get_state();
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

bool Server_Manager::verify_room_id(int room_id)
{
    for (auto& room : rooms)
    {
        if (room.id == room_id)
        {
            return true;
        }
    }
    return false;
}

void Server_Manager::delete_room(int room_id)
{
    for (auto it = rooms.begin(); it != rooms.end(); ++it)
    {
        if (it->id == room_id)
        {
            rooms.erase(it);
            return;
        }
    }
}
}  // namespace server