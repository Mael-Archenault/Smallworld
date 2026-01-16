#include "server.h"

namespace server
{
Player::Player(std::string name, std::string session_token)
    : name(name), session_token(session_token), in_room(-1)
{
}

std::string Player::get_name()
{
    return name;
}

std::string Player::get_session_token()
{
    return session_token;
}

void Player::set_room(int room_id)
{
    in_room = room_id;
}

int Player::get_room()
{
    return in_room;
}
}  // namespace server