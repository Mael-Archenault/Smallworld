#include <iostream>

#include "client.h"

namespace client
{

Local_Lobby_State::Local_Lobby_State(sf::RenderWindow& window)
    : renderer(window), player_adder_window_opened(false)
{
    std::cout << "Local Lobby State" << std::endl;
    players = {{"Alice", 0}, {"Bob", 1}};
}

void Local_Lobby_State::handle_input(sf::Event event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2i mouse_pos = sf::Mouse::getPosition(context->get_window());
        std::unordered_map<std::string, sf::FloatRect> layout_infos = renderer.get_layout_infos();

        if (layout_infos["exit_button"].contains(static_cast<sf::Vector2f>(mouse_pos)))
        {
            std::cout << "Exiting to Menu State" << std::endl;
            Menu_State* new_state = new Menu_State(this->context->get_window());
            this->context->change_state(new_state);
            return;
        }
        if (layout_infos["start_button"].contains(static_cast<sf::Vector2f>(mouse_pos)))
        {
            std::cout << "Starting Local Game" << std::endl;
            int                      nb_players = players.size();
            std::vector<std::string> player_names;
            for (const auto& p : players)
            {
                player_names.push_back(p.first);
            }
            engine::Engine*   engine = new engine::Engine(nb_players, player_names);
            Local_Game_State* new_state =
                new Local_Game_State(this->context->get_window(), *engine);
            this->context->change_state(new_state);
            return;
        }
        if (layout_infos["add_button"].contains(static_cast<sf::Vector2f>(mouse_pos)))
        {
            std::cout << "Opening Player Adder Window" << std::endl;
            player_adder_window_opened = true;
        }
    }
    // Handle input events specific to the local lobby state
}

void Local_Lobby_State::render(sf::RenderWindow& window)
{
    renderer.render(players, player_adder_window_opened);
}

}  // namespace client
