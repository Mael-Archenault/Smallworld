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
    return 0;
}
}  // namespace server