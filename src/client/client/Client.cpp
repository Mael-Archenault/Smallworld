#include "client.h"

#include <ai/Ai_Heuristic.h>
#include <ai/Ai_Random.h>

#include <cmath>
#include <iostream>

#include "engine.h"

namespace client
{

Client::Client(engine::Engine& engine)
    : window(sf::VideoMode(1720, 820), "Smallworld"),
      state(engine.get_state()),
      renderer(state, window),
      engine(engine),
      click_handler(*this)
{
    selected_area_id         = 0;
    tribe_info_window_opened = false;
    renderer.set_selected_area(selected_area_id);
    // ais = std::vector<ai::Ai_Interface*>({new ai::Ai_Random(&state,1), new
    // ai::Ai_Heuristic(&state,0)});
    ais = std::vector<ai::Ai_Interface*>();
}

int Client::run()
{
    // first rendering
    sf::Event e;
    while (window.pollEvent(e))
    {
    }  // purge early events

    window.clear(sf::Color::Black);
    renderer.render(state, state.get_current_player().id);
    window.display();

    sf::View view           = window.getDefaultView();  // store your base view
    bool     mouse_clicked  = false;
    bool     event_happened = false;
    while (window.isOpen())
    {
        bool is_ai_turn = false;
        for (auto ai : ais)
        {
            if (ai->id == state.get_current_player().id)
            {
                is_ai_turn = true;
                ai->update_state(&state);
                engine.add_command(ai->give_command(state.get_current_turn_phase()));
                event_happened = true;
                // sf::sleep(sf::seconds(1));
            }
        }

        if (!is_ai_turn)
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed) window.close();

                // When window is resized:
                if (event.type == sf::Event::Resized)
                {
                    // Reset the view to match the new window size
                    event_happened = true;
                    view.setSize(event.size.width, event.size.height);
                    view.setCenter(event.size.width / 2.f, event.size.height / 2.f);
                    window.setView(view);
                }

                // handle click

                if (event.type == sf::Event::MouseButtonPressed && mouse_clicked == false)
                {
                    event_happened         = true;
                    mouse_clicked          = true;
                    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                    // handle_mouse_click(mouse_pos);
                    click_handler.handle_click(mouse_pos);
                }
                if (event.type == sf::Event::MouseButtonReleased)
                {
                    mouse_clicked = false;
                }
            }
        }

        if (event_happened)
        {
            try
            {
                engine.update();
                // state = state::Game_State(engine.get_state());
                state = engine.get_state().deep_copy();
                if (state.is_game_finished())
                {
                    std::cout << "Maximum rounds reached." << std::endl;
                    std::pair<int, int> victorious_player_money = {0, -1};
                    for (std::pair<int, int> player_id_money : state.get_all_player_id_money())
                    {
                        std::cout << "Player " << player_id_money.first << " has "
                                  << player_id_money.second << " money." << std::endl;
                        if (player_id_money.second >= victorious_player_money.second)
                        {
                            victorious_player_money = player_id_money;
                        }
                    }
                    std::cout << "\nVictory goes to " << victorious_player_money.first << ", with "
                              << victorious_player_money.second << " money !" << std::endl;
                    window.close();
                    return victorious_player_money.first;
                }
            }
            catch (const std::exception& e)
            {
                engine.remove_last_command();
                std::cerr << "Error executing command: " << e.what() << std::endl;
            }
            window.clear(sf::Color::Black);
            renderer.render(state, state.get_current_player().id);
            window.display();
            event_happened = false;
        }
    }
    return 0;
}

int Client::get_selected_area_id()
{
    return selected_area_id;
}

void Client::set_selected_area_id(int selected_area_id)
{
    this->selected_area_id = selected_area_id;
}
int Client::get_selected_position_in_stack()
{
    return selected_position_in_stack;
}
void Client::set_selected_position_in_stack(int position)
{
    selected_position_in_stack = position;
}

void Client::set_tribe_info_window_state(bool is_window_opened)
{
    tribe_info_window_opened = is_window_opened;
}

bool Client::get_tribe_info_window_state()
{
    return tribe_info_window_opened;
}

state::Game_State& Client::get_state()
{
    return state;
}
renderer::Renderer& Client::get_renderer()
{
    return renderer;
}
engine::Engine& Client::get_engine()
{
    return engine;
}
}  // namespace client
