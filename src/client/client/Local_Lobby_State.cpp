#include <iostream>

#include "client.h"

namespace client
{

Local_Lobby_State::Local_Lobby_State()
{
    std::cout << "Local Lobby State" << std::endl;
}

void Local_Lobby_State::handle_input(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::M)
        {
            std::cout << "Switching to Menu State" << std::endl;
            Menu_State* new_state = new Menu_State(this->context->get_window());
            this->context->change_state(new_state);
        }

        if (event.key.code == sf::Keyboard::G)
        {
            std::cout << "Switching to Local Game State" << std::endl;
            int                      nb_players   = 2;
            std::vector<std::string> player_names = {"Alice", "Bob"};
            engine::Engine*          engine       = new engine::Engine(nb_players, player_names);
            Local_Game_State*        new_state =
                new Local_Game_State(this->context->get_window(), *engine);
            this->context->change_state(new_state);
        }
    }
    // Handle input events specific to the local lobby state
}

}  // namespace client
