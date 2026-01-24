#include <iostream>

#include "client.h"
#include "resources_dir.h"

namespace client
{

Menu_State::Menu_State(sf::RenderWindow& window) : renderer(window) {}

void Menu_State::handle_input(sf::Event event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        register_click(sf::Mouse::getPosition(context->get_window()));
        register_layout(renderer.get_layout_infos());

        if (clicked_on("online_button"))
        {
            Online_Menu_State* new_state =
                new Online_Menu_State(context->get_window(), context->get_name());
            this->context->change_state(new_state);
            delete this;
            return;
        }

        if (clicked_on("local_button"))
        {
            Local_Lobby_State* new_state = new Local_Lobby_State(context->get_window());
            this->context->change_state(new_state);
            delete this;
            return;
        }
    }
}

void Menu_State::render(sf::RenderWindow& window)
{
    renderer.render();
}
}  // namespace client
