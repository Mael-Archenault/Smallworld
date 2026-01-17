#include <unistd.h>

#include <iostream>
#include <mutex>
#include <thread>

#include "client.h"
#include "engine.h"
#include "renderer.h"

namespace client
{

void engine_process(engine::Engine& engine, client::Local_Game_State& client)
{
    bool running = true;
    while (running)
    {
        {
            std::lock_guard<std::mutex> lock(client.get_mutex());
            try
            {
                engine.update();
            }
            catch (std::exception& e)
            {
                engine.remove_last_command();
                std::cerr << e.what() << std::endl;
            }
        }

        usleep(50000 / 3);  // 60 updates per secound
        running = client.get_running_flag("engine_update");
    }
}

void state_update_process(client::Local_Game_State& client)
{
    bool running = true;
    while (running)
    {
        {
            std::lock_guard<std::mutex> lock(client.get_mutex());
            client.update_state();
        }

        usleep(100000);  // 10 updates per secound
        running = client.get_running_flag("state_update");
    }
}

int                      nb_players   = 2;
std::vector<std::string> player_names = {"Alice", "Bob"};

Local_Game_State::Local_Game_State(sf::RenderWindow& window, engine::Engine& engine)
    : engine(engine),
      state(engine.get_state()),
      renderer(state, window),
      selected_area_id(0),
      tribe_info_window_opened(false),
      selected_position_in_stack(0),
      click_handler(this),
      mouse_clicked(false)

{
    std::cout << "Local Game State" << std::endl;

    // initiating the threads
    register_thread(
        "engine_update",
        std::thread([](engine::Engine& engine, client::Local_Game_State& client)
                    { engine_process(engine, client); }, std::ref(engine), std::ref(*this)));

    register_thread("state_update",
                    std::thread([](client::Local_Game_State& client)
                                { state_update_process(client); }, std::ref(*this)));

    start_thread("engine_update");
    start_thread("state_update");
}
Local_Game_State::~Local_Game_State()
{
    stop_thread("engine_update");
    stop_thread("state_update");
}

void Local_Game_State::handle_input(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::M)
        {
            std::cout << "Switching to Menu State" << std::endl;
            stop_thread("engine_update");
            stop_thread("state_update");
            Menu_State* new_state = new Menu_State(this->context->get_window());
            this->context->change_state(new_state);
        }
    }

    // handle click

    if (event.type == sf::Event::MouseButtonPressed && mouse_clicked == false)
    {
        mouse_clicked          = true;
        sf::Vector2i mouse_pos = sf::Mouse::getPosition(context->get_window());
        {
            std::lock_guard<std::mutex> lock(get_mutex());
            click_handler.handle_click(mouse_pos);
        }
    }
    if (event.type == sf::Event::MouseButtonReleased)
    {
        mouse_clicked = false;
    }
}

void Local_Game_State::render(sf::RenderWindow& window)
{
    std::lock_guard<std::mutex> lock(get_mutex());
    renderer.render(state, state.get_current_player().id);
}

int Local_Game_State::get_selected_area_id()
{
    return selected_area_id;
}

void Local_Game_State::set_selected_area_id(int selected_area_id)
{
    this->selected_area_id = selected_area_id;
}
int Local_Game_State::get_selected_position_in_stack()
{
    return selected_position_in_stack;
}
void Local_Game_State::set_selected_position_in_stack(int position)
{
    selected_position_in_stack = position;
}

void Local_Game_State::set_tribe_info_window_state(bool is_window_opened)
{
    tribe_info_window_opened = is_window_opened;
}

bool Local_Game_State::get_tribe_info_window_state()
{
    return tribe_info_window_opened;
}

state::Game_State& Local_Game_State::get_state()
{
    return state;
}
renderer::Game_Renderer& Local_Game_State::get_renderer()
{
    return renderer;
}
engine::Engine& Local_Game_State::get_engine()
{
    return engine;
}

void Local_Game_State::update_state()
{
    if (engine.get_state_version_id() == state.get_version_id())
    {
        return;
    }
    state = engine.get_state().deep_copy();
}  // namespace client

}  // namespace client
