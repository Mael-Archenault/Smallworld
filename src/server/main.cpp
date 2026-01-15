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

MHD_Result print_out_key(void* cls, enum MHD_ValueKind kind, const char* key, const char* value)
{
    std::cout << key << ": " << value << "\n";
    return MHD_YES;
}

MHD_Result answer_to_connection(void* cls, struct MHD_Connection* connection, const char* url,
                                const char* method, const char* version, const char* upload_data,
                                size_t* upload_data_size, void** req_cls)
{
    server::Service_Manager* service_manager = static_cast<server::Service_Manager*>(cls);
    std::cout << url << std::endl;

    struct MHD_Response* response;
    MHD_Result           ret;
    std::string          in;
    std::string          out;

    service_manager->handle_request(in, out, url, method);

    response =
        MHD_create_response_from_buffer(out.size(), (void*) out.c_str(), MHD_RESPMEM_MUST_COPY);
    // Queue the response
    ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);
    return ret;
}

int main(int argc, char* argv[])
{
    std::cout << "Hello World" << std::endl;

    server::Server_Manager  server_manager;
    server::Service_Manager service_manager;

    std::unique_ptr<server::Room_Service> room_service =
        std::make_unique<server::Room_Service>(server_manager);
    service_manager.register_service(std::move(room_service));

    struct MHD_Daemon* daemon;
    daemon = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD, PORT, NULL, NULL,
                              &answer_to_connection, &service_manager, MHD_OPTION_END);
    if (NULL == daemon) return 1;
    getchar();
    MHD_stop_daemon(daemon);
    return 0;
}
