#include "client.h"

namespace client
{

void Client_State_Interface::handle_input(sf::Event event) {}
void Client_State_Interface::render(sf::RenderWindow& window) {}

void Client_State_Interface::set_context(Client* context)
{
    this->context = context;
}

Client_State_Interface::~Client_State_Interface() {}

}  // namespace client
