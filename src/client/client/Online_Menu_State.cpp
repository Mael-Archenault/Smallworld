#include <iostream>

#include "client.h"
namespace client
{

Online_Menu_State::Online_Menu_State()
{
    std::cout << "Online Menu State" << std::endl;
}

void Online_Menu_State::handle_input(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::M)
        {
            std::cout << "Switching to Menu State" << std::endl;
            Menu_State* new_state = new Menu_State();
            this->context->change_state(new_state);
        }
        if (event.key.code == sf::Keyboard::L)
        {
            std::cout << "Switching to Online Lobby State" << std::endl;
            Online_Lobby_State* new_state = new Online_Lobby_State();
            this->context->change_state(new_state);
        }
    }
    // Handle input events specific to the online menu state
}
}  // namespace client
