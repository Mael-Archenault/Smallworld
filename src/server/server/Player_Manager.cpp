#include <iostream>

#include "server.h"

namespace server
{
Player_Manager::Player_Manager() {}

std::string Player_Manager::create_player(std::string name)
{
    std::string session_token = generate_session_token();
    connected_players.push_back(Player(name, session_token));
    std::cout << "Created player: " << connected_players.back().get_name() << std::endl;
    return session_token;
}

Player& Player_Manager::get_player(std::string session_token)
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

std::string Player_Manager::generate_session_token()
{
    static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::string       id;
    for (int i = 0; i < 16; ++i)
    {
        id += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return id;
}

bool Player_Manager::verify_session_token(std::string session_token)
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