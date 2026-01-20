#include <fcntl.h>
#include <microhttpd.h>
#include <resources_dir.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <string>
#define PORT 8888

#include "server.h"

struct HttpRequest
{
    std::string method;
    std::string path;
    std::string body;
    std::string sessionToken;
};

struct HttpResponse
{
    int         status;
    std::string body;
};

MHD_Result answer_to_connection(void* cls, struct MHD_Connection* connection, const char* url,
                                const char* method, const char* version, const char* upload_data,
                                size_t* upload_data_size, void** req_cls)
{
    server::Service_Manager* service_manager = static_cast<server::Service_Manager*>(cls);

    struct MHD_Response* response;
    MHD_Result           ret;
    std::string          in;
    std::string          out;

    // Accumulate POST body across calls (required by libmicrohttpd)
    std::string* body_ptr = static_cast<std::string*>(*req_cls);
    if (!body_ptr)
    {
        body_ptr = new std::string();
        *req_cls = body_ptr;
        return MHD_YES;  // libmicrohttpd will call again with upload data
    }

    if (std::string(method) == "POST" && *upload_data_size != 0)
    {
        body_ptr->append(upload_data, *upload_data_size);
        *upload_data_size = 0;  // tell libmicrohttpd we've consumed the data
        return MHD_YES;         // it will call us again with upload_data_size == 0
    }

    std::string session_token;
    const char* session_token_cstr =
        MHD_lookup_connection_value(connection, MHD_HEADER_KIND, "Session-Token");
    if (session_token_cstr)
    {
        session_token = std::string(session_token_cstr);
    }

    // All data received; use accumulated body
    in = *body_ptr;
    delete body_ptr;
    *req_cls = nullptr;

    server::Http_Status status =
        service_manager->handle_request(in, out, url, method, session_token);

    response =
        MHD_create_response_from_buffer(out.size(), (void*) out.c_str(), MHD_RESPMEM_MUST_COPY);
    // Queue the response
    ret = MHD_queue_response(connection, static_cast<unsigned int>(status), response);
    MHD_destroy_response(response);
    return ret;
}

int main(int argc, char* argv[])
{
    std::cout << "Starting Server..." << std::endl;

    server::Player_Manager  player_manager;
    server::Service_Manager service_manager(player_manager);

    std::unique_ptr<server::Room_Service> room_service =
        std::make_unique<server::Room_Service>(player_manager);
    std::unique_ptr<server::Game_Service> game_service =
        std::make_unique<server::Game_Service>(*room_service);
    service_manager.register_service(std::move(room_service));
    service_manager.register_service(std::move(game_service));

    struct MHD_Daemon* daemon;
    daemon = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD, PORT, NULL, NULL,
                              &answer_to_connection, &service_manager, MHD_OPTION_END);
    if (NULL == daemon) return 1;
    getchar();
    MHD_stop_daemon(daemon);
    return 0;
}
