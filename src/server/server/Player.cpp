#include "server.h"

namespace server
{
Player::Player(std::string name, std::string session_token)
    : name(name), session_token(session_token)
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
}  // namespace server