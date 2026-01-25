#include "server.h"

namespace server
{
Service_Interface::Service_Interface(std::string root_prefix) : root_prefix(root_prefix) {}

Service_Interface::~Service_Interface() {}

Http_Status Service_Interface::get(std::string& in, std::string& out, std::string url,
                                   std::string session_token)
{
    return Http_Status::NOT_FOUND;
}
Http_Status Service_Interface::post(std::string& in, std::string& out, std::string url,
                                    std::string session_token)
{
    return Http_Status::NOT_FOUND;
}
std::string Service_Interface::get_root_prefix()
{
    return root_prefix;
}
}  // namespace server