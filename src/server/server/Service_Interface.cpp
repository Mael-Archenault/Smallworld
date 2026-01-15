#include "server.h"

namespace server
{
Service_Interface::Service_Interface(std::string root_prefix) : root_prefix(root_prefix) {}

Service_Interface::~Service_Interface() {}

void Service_Interface::get(std::string url) {}
void Service_Interface::post(std::string url) {}
}  // namespace server