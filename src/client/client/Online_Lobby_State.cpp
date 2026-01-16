#include <iostream>

#include "client.h"
namespace client
{

Online_Lobby_State::Online_Lobby_State()
{
    std::cout << "Online Lobby State" << std::endl;
}

void Online_Lobby_State::handle_input(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::O)
        {
            std::cout << "Switching to Online Menu State" << std::endl;
            Online_Menu_State* new_state = new Online_Menu_State();
            this->context->change_state(new_state);
        }
        if (event.key.code == sf::Keyboard::M)
        {
            std::cout << "Switching to Menu State" << std::endl;
            Menu_State* new_state = new Menu_State();
            this->context->change_state(new_state);
        }
        if (event.key.code == sf::Keyboard::G)
        {
            std::cout << "Switching to Online Game State" << std::endl;
            Online_Game_State* new_state = new Online_Game_State();
            this->context->change_state(new_state);
        }
    }
    // Handle input events specific to the online lobby state
}
}  // namespace client
