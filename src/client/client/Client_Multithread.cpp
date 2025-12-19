
#include <unistd.h>

#include <iostream>
#include <thread>

#include "client.h"
#include "engine.h"

namespace client
{

Client_Multithread::Client_Multithread(engine::Engine& engine)
    : window(sf::VideoMode(1720, 820), "Smallworld"),
      state(engine.get_state()),
      renderer(state, window),
      engine(engine),
      click_handler(*this)
{
    selected_area_id         = 0;
    tribe_info_window_opened = false;
    renderer.set_selected_area(selected_area_id);
}

int Client_Multithread::run()
{
    std::cout << "\nRunning Client_Multithread..." << std::endl;
    // first rendering
    sf::Event e;
    while (window.pollEvent(e))
    {
    }  // purge early events

    window.clear(sf::Color::Black);
    renderer.render(state);
    window.display();

    sf::View view          = window.getDefaultView();  // store your base view
    bool     mouse_clicked = false;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();

            // When window is resized:
            if (event.type == sf::Event::Resized)
            {
                // Reset the view to match the new window size
                view.setSize(event.size.width, event.size.height);
                view.setCenter(event.size.width / 2.f, event.size.height / 2.f);
                window.setView(view);
            }

            // handle click

            if (event.type == sf::Event::MouseButtonPressed && mouse_clicked == false)
            {
                mouse_clicked          = true;
                sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                std::cout << "Mouse clicked at position: " << mouse_pos.x << ", " << mouse_pos.y
                          << std::endl;
                // handle_mouse_click(mouse_pos);
                click_handler.handle_click(mouse_pos);
            }
            if (event.type == sf::Event::MouseButtonReleased)
            {
                mouse_clicked = false;
            }
        }
        renderer_process();
    }
}

int Client_Multithread::get_selected_area_id()
{
    return selected_area_id;
}

void Client_Multithread::set_selected_area_id(int selected_area_id)
{
    this->selected_area_id = selected_area_id;
}
int Client_Multithread::get_selected_position_in_stack()
{
    return selected_position_in_stack;
}
void Client_Multithread::set_selected_position_in_stack(int position)
{
    selected_position_in_stack = position;
}

void Client_Multithread::set_tribe_info_window_state(bool is_window_opened)
{
    tribe_info_window_opened = is_window_opened;
}

bool Client_Multithread::get_tribe_info_window_state()
{
    return tribe_info_window_opened;
}

state::Game_State& Client_Multithread::get_state()
{
    return state;
}
renderer::Renderer& Client_Multithread::get_renderer()
{
    return renderer;
}
engine::Engine& Client_Multithread::get_engine()
{
    return engine;
}

void Client_Multithread::renderer_process()
{
    window.clear(sf::Color::Black);
    renderer.render(state);
    window.display();
}

void Client_Multithread::update_process()
{
    try
    {
        engine.update();
        state = state::Game_State(engine.get_state());

        // if (state.is_game_finished())
        // {
        //     std::cout << "Maximum rounds reached." << std::endl;
        //     std::pair<int, int> victorious_player_money = {0, -1};
        //     for (std::pair<int, int> player_id_money : state.get_all_player_id_money())
        //     {
        //         std::cout << "Player " << player_id_money.first << " has " <<
        //         player_id_money.second
        //                   << " money." << std::endl;
        //         if (player_id_money.second >= victorious_player_money.second)
        //         {
        //             victorious_player_money = player_id_money;
        //         }
        //     }
        //     std::cout << "\nVictory goes to " << victorious_player_money.first << ", with "
        //               << victorious_player_money.second << " money !" << std::endl;
        //     window.close();
        //     return victorious_player_money.first;
        // }
    }
    catch (const std::exception& e)
    {
        engine.remove_last_command();
        std::cerr << "Error executing command: " << e.what() << std::endl;
    }
}
}  // namespace client
