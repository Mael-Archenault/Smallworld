#include <iostream>

#include "client.h"
#include "resources_dir.h"

namespace client
{

Menu_State::Menu_State(sf::RenderWindow& window) : renderer(window)
{
    std::cout << "Menu State" << std::endl;
}

void Menu_State::handle_input(sf::Event event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2i mouse_pos = sf::Mouse::getPosition(context->get_window());
        std::unordered_map<std::string, sf::FloatRect> layout_infos = renderer.get_layout_infos();

        if (layout_infos["online_button"].contains(static_cast<sf::Vector2f>(mouse_pos)))
        {
            std::cout << "Switching to Online Menu State" << std::endl;
            Online_Menu_State* new_state =
                new Online_Menu_State(context->get_window(), context->get_name());
            this->context->change_state(new_state);
            return;
        }

        if (layout_infos["local_button"].contains(static_cast<sf::Vector2f>(mouse_pos)))
        {
            std::cout << "Switching to Local Lobby State" << std::endl;
            Local_Lobby_State* new_state = new Local_Lobby_State(context->get_window());
            this->context->change_state(new_state);
            return;
        }
    }
}

void Menu_State::render(sf::RenderWindow& window)
{
    renderer.render();
}
}  // namespace client
