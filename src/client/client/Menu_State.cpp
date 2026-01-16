#include <iostream>

#include "client.h"

namespace client
{

Menu_State::Menu_State()
{
    std::cout << "Menu State" << std::endl;
}

void Menu_State::handle_input(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::O)
        {
            std::cout << "Switching to Online Menu State" << std::endl;
            Online_Menu_State* new_state = new Online_Menu_State();
            this->context->change_state(new_state);
        }

        if (event.key.code == sf::Keyboard::L)
        {
            std::cout << "Switching to Local Lobby State" << std::endl;
            Local_Lobby_State* new_state = new Local_Lobby_State();
            this->context->change_state(new_state);
        }
    }
}

void Menu_State::render(sf::RenderWindow& window)
{
    window.clear(sf::Color::Black);
    // two buttons to choose between online and local
    // for now just text
    sf::Font font;
}
}  // namespace client
