#include <iostream>

#include "ai.h"
#include "client.h"

namespace client
{

bool clicked_on(sf::FloatRect rect, sf::Vector2i mouse_pos)
{
    return rect.contains(static_cast<sf::Vector2f>(mouse_pos));
}

Local_Lobby_State::Local_Lobby_State(sf::RenderWindow& window)
    : renderer(window),
      player_adder_window_opened(false),
      modifying_name(false),
      selected_player_type(state::Player_Type::Human)
{
}

void Local_Lobby_State::handle_input(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::BackSpace && !added_player_name.empty())
        {
            added_player_name.pop_back();
            return;
        }
        if (event.key.code == sf::Keyboard::Enter)
        {
            modifying_name = false;
            return;
        }

        if (event.key.code == sf::Keyboard::Space && added_player_name.size() < 20)
        {
            added_player_name += ' ';
            return;
        }
    }
    if (event.type == sf::Event::TextEntered)

    {
        sf::Uint32 unicode = event.text.unicode;

        if (modifying_name)
        {
            if (unicode >= 32 && unicode < 128 &&
                added_player_name.size() < 20)  // basic ASCII and limit to 20 chars
            {
                added_player_name += static_cast<char>(unicode);
            }
        }
    }
    if (event.type == sf::Event::MouseButtonPressed)
    {
        register_click(sf::Mouse::getPosition(context->get_window()));
        register_layout(renderer.get_layout_infos());

        if (player_adder_window_opened)
        {
            if (!clicked_on("adder_window") || clicked_on("adder_window_close_button"))
            {
                player_adder_window_opened = false;
                return;
            }

            if (modifying_name && !clicked_on("adder_window_name_box"))
            {
                modifying_name = false;
                return;
            }
            if (clicked_on("adder_window_name_box"))
            {
                modifying_name = true;
                return;
            }

            if (clicked_on("adder_window_real_player_button"))
            {
                selected_player_type = state::Player_Type::Human;
            }
            if (clicked_on("adder_window_random_ai_button"))
            {
                selected_player_type = state::Player_Type::Random_AI;
            }

            if (clicked_on("adder_window_heuristic_ai_button"))
            {
                selected_player_type = state::Player_Type::Heuristic_AI;
            }
            if (clicked_on("adder_window_advanced_ai_button"))
            {
                selected_player_type = state::Player_Type::Advanced_AI;
            }

            if (clicked_on("adder_window_add_button"))
            {
                if (added_player_name.empty())
                {
                    added_player_name = "Player" + std::to_string(player_names.size() + 1);
                }

                player_names.push_back(added_player_name);
                player_types.push_back(selected_player_type);
                added_player_name.clear();
                selected_player_type       = state::Player_Type::Human;
                modifying_name             = false;
                player_adder_window_opened = false;
                return;
            }
        }
        if (clicked_on("exit_button"))
        {
            std::cout << "Exiting to Menu State" << std::endl;
            Menu_State* new_state = new Menu_State(this->context->get_window());
            this->context->change_state(new_state);
            return;
        }
        if (clicked_on("start_button"))
        {
            std::cout << "Starting Local Game" << std::endl;
            engine::Engine*   engine = new engine::Engine(player_names);
            Local_Game_State* new_state =
                new Local_Game_State(this->context->get_window(), *engine, player_types);
            this->context->change_state(new_state);
            return;
        }
        if (clicked_on("add_button"))
        {
            std::cout << "Opening Player Adder Window" << std::endl;
            player_adder_window_opened = true;
            return;
        }
        if (clicked_on("remove_player_button"))
        {
            if (!player_names.empty())
            {
                player_names.pop_back();
                player_types.pop_back();
            }
            return;
        }
    }
    // Handle input events specific to the local lobby state
}

void Local_Lobby_State::render(sf::RenderWindow& window)
{
    renderer.render(player_names, player_types, player_adder_window_opened, modifying_name,
                    added_player_name, selected_player_type);
}

}  // namespace client
