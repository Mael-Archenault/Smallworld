#include <iostream>

#include "server.h"
namespace server
{
Room_Service::Room_Service(Server_Manager& server_manager)
    : Service_Interface("/rooms"), server_manager(server_manager)
{
}

void Room_Service::post(std::string url, std::string session_token)
{
    // remove prefix from url
    std::string route = url.substr(root_prefix.size());

    // creating a room
    if (route == "/create")
    {
        server_manager.create_room(session_token);
        return;
    }

    // action on a specific room -> /join/{id} or /exit/{id}

    size_t      first_slash_pos = route.find('/', 1);
    std::string action          = route.substr(0, first_slash_pos);
    std::string room_id_str     = route.substr(first_slash_pos + 1);
    int         room_id         = std::stoi(room_id_str);
    std::cout << "room id : " << room_id_str << " action : " << action << std::endl;
    if (action == "/join")
    {
        server_manager.join_room(room_id, session_token);
        return;
    }
    if (action == "/exit")
    {
        server_manager.exit_room(room_id, session_token);
    }
}
}  // namespace server