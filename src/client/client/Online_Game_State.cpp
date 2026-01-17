#include <iostream>

#include "client.h"
namespace client
{

Online_Game_State::Online_Game_State()
{
    std::cout << "Online Game State" << std::endl;
}

void Online_Game_State::handle_input(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::M)
        {
            std::cout << "Switching to Menu State" << std::endl;
            Menu_State* new_state = new Menu_State(this->context->get_window());
            this->context->change_state(new_state);
        }
    }
    // Handle input events specific to the online game state
}
}  // namespace client
