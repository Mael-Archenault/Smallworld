#include "server.h"

namespace server
{
Room_Service::Room_Service(Server_Manager& server_manager)
    : Service_Interface("/rooms"), server_manager(server_manager)
{
}

void Room_Service::post(std::string url)
{
    if (url == "/rooms/create")
    {
        server_manager.create_room("temp");
    }
}
}  // namespace server