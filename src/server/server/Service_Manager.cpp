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

Http_Status Service_Manager::handle_request(std::string& in, std::string& out, std::string url,
                                            std::string method, std::string session_token)
{
    // first request

    if (url == "/connect" && method == "POST")
    {
        std::string new_session_token = server_manager.create_player();
        out                           = new_session_token;
        return Http_Status::OK;
    }

    // else

    bool is_authenticated = server_manager.verify_session_token(session_token);

    if (!is_authenticated)
    {
        return Http_Status::UNAUTHORIZED;
    }

    Player requesting_player = server_manager.get_player(session_token);

    Service_Interface* service = find_responsible_service(url);
    if (!service)
    {
        return Http_Status::NOT_FOUND;
    }
    // remove prefix from url
    std::string action = url.substr(service->get_root_prefix().size());
    if (service)
    {
        if (method == "GET")
        {
            Http_Status status = service->get(in, out, action, session_token);
            return status;
        }
        if (method == "POST")
        {
            Http_Status status = service->post(in, out, action, session_token);
            return status;
        }
        return Http_Status::METHOD_NOT_ALLOWED;
    }
}
}  // namespace server