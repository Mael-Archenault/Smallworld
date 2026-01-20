#include "client.h"

namespace client
{

Client_State_Interface::Client_State_Interface() : context(nullptr) {}
void Client_State_Interface::handle_input(sf::Event event) {}
void Client_State_Interface::render(sf::RenderWindow& window) {}

void Client_State_Interface::set_context(Client* context)
{
    this->context = context;
}

Client_State_Interface::~Client_State_Interface() {}

std::string Client_State_Interface::get_server_ip()
{
    if (context == nullptr)
    {
        return "localhost";
    }
    return context->get_server_ip();
}

}  // namespace client
