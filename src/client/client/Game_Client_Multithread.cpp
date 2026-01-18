
#include <unistd.h>

#include <iostream>
#include <mutex>
#include <thread>

#include "client.h"
#include "engine.h"

extern std::mutex mtx;

namespace client
{

// Game_Client_Multithread::Game_Client_Multithread(engine::Engine& engine, int player_id)
//     : window(sf::VideoMode(1720, 820), "Smallworld"),
//       state(engine.get_state().deep_copy()),
//       renderer(state, window),
//       engine(engine),
//       player_id(player_id),
//       click_handler(Online_Game_State(window, 0, ""))  // dummy Online_Game_State
// {
//     selected_area_id         = 0;
//     tribe_info_window_opened = false;
//     renderer.set_selected_area(selected_area_id);
// }

// int Game_Client_Multithread::run()
// {
//     std::cout << "\nRunning Game_Client_Multithread..." << std::endl;
//     // first rendering
//     sf::Event e;
//     while (window.pollEvent(e))
//     {
//     }  // purge early events

//     window.clear(sf::Color::Black);
//     {
//         std::lock_guard<std::mutex> lock(mtx);
//         renderer.render(state, player_id);
//     }
//     window.display();

//     sf::View view          = window.getDefaultView();  // store your base view
//     bool     mouse_clicked = false;
//     while (window.isOpen())
//     {
//         sf::Event event;
//         while (window.pollEvent(event))
//         {
//             if (event.type == sf::Event::Closed)
//             {
//                 window.close();
//             }

//             // When window is resized:
//             if (event.type == sf::Event::Resized)
//             {
//                 // Reset the view to match the new window size
//                 view.setSize(event.size.width, event.size.height);
//                 view.setCenter(event.size.width / 2.f, event.size.height / 2.f);
//                 window.setView(view);
//             }

//             // handle click

//             if (event.type == sf::Event::MouseButtonPressed && mouse_clicked == false)
//             {
//                 mouse_clicked          = true;
//                 sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
//                 {
//                     std::lock_guard<std::mutex> lock(mtx);
//                     click_handler.handle_click(mouse_pos);
//                 }
//             }
//             if (event.type == sf::Event::MouseButtonReleased)
//             {
//                 mouse_clicked = false;
//             }
//         }

//         window.clear(sf::Color::Black);
//         {
//             std::lock_guard<std::mutex> lock(mtx);
//             renderer.render(state, player_id);
//         }

//         window.display();
//         usleep(1000000 / 60);  // limit to 60 fps
//     }

//     return 0;
// }

// int Game_Client_Multithread::get_selected_area_id()
// {
//     return selected_area_id;
// }

// void Game_Client_Multithread::set_selected_area_id(int selected_area_id)
// {
//     this->selected_area_id = selected_area_id;
// }
// int Game_Client_Multithread::get_selected_position_in_stack()
// {
//     return selected_position_in_stack;
// }
// void Game_Client_Multithread::set_selected_position_in_stack(int position)
// {
//     selected_position_in_stack = position;
// }

// void Game_Client_Multithread::set_tribe_info_window_state(bool is_window_opened)
// {
//     tribe_info_window_opened = is_window_opened;
// }

// bool Game_Client_Multithread::get_tribe_info_window_state()
// {
//     return tribe_info_window_opened;
// }

// state::Game_State& Game_Client_Multithread::get_state()
// {
//     return state;
// }
// renderer::Game_Renderer& Game_Client_Multithread::get_renderer()
// {
//     return renderer;
// }
// engine::Engine& Game_Client_Multithread::get_engine()
// {
//     return engine;
// }

// void Game_Client_Multithread::update_state()
// {
//     if (engine.get_state_version_id() == state.get_version_id())
//     {
//         return;
//     }
//     Json::Value root;
//     root = engine.get_state_json();
//     state.from_json(root);
// }

// int Game_Client_Multithread::get_player_id()
// {
//     return player_id;
// }
}  // namespace client
