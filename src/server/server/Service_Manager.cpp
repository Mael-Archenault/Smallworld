#include <iostream>

#include "server.h"

namespace server
{

Service_Manager::Service_Manager(Server_Manager& server_manager) : server_manager(server_manager) {}

Service_Interface* Service_Manager::find_responsible_service(std::string url)
{
    for (auto& service : services)
    {
        if (url.find(service->root_prefix) == 0)
        {
            return service.get();
        }
    }
    return nullptr;
}

void Service_Manager::register_service(std::unique_ptr<Service_Interface> service)
{
    services.push_back(std::move(service));
}

void Service_Manager::handle_request(std::string& in, std::string& out, std::string url,
                                     std::string method, std::string session_token)
{
    // first request

    if (url == "/connect" && method == "POST")
    {
        std::string new_session_token = server_manager.create_player();
        out                           = new_session_token;
        return;
    }

    // else

    bool is_authenticated = server_manager.verify_session_token(session_token);

    if (!is_authenticated)
    {
        return;
    }

    Player requesting_player = server_manager.get_player(session_token);

    Service_Interface* service = find_responsible_service(url);
    if (service)
    {
        if (method == "GET")
        {
            service->get(url);
            out = "in response to GET " + url;
        }
        else if (method == "POST")
        {
            service->post(url, session_token);
            out = "in response to POST " + url;
        }
    }
}
}  // namespace server