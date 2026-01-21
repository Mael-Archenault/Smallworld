#include <unistd.h>

#include <SFML/Network.hpp>
#include <iostream>

#include "client.h"
namespace client
{

void online_state_update_process(client::Online_Game_State& client)
{
    bool running = true;
    while (running)
    {
        {
            std::lock_guard<std::mutex> lock(client.get_mutex());
            try
            {
                client.update_state();
            }
            catch (std::exception& e)
            {
                std::cout << "Error updating state: " << e.what() << std::endl;
            }
        }

        usleep(100000);  // 10 updates per second
        running = client.get_running_flag("state_update");
    }
}

Online_Game_State::Online_Game_State(sf::RenderWindow&        window,
                                     std::vector<std::string> player_names, int room_id,
                                     std::string session_token, int player_id)
    : state(player_names, 0),
      renderer(state, window),
      selected_area_id(0),
      tribe_info_window_opened(false),
      selected_position_in_stack(0),
      click_handler(*this),
      player_id(player_id),
      mouse_clicked(false),
      room_id(room_id),
      session_token(session_token)
{
    register_thread("state_update",
                    std::thread([](client::Online_Game_State& client)
                                { online_state_update_process(client); }, std::ref(*this)));

    start_thread("state_update");
}

Online_Game_State::~Online_Game_State()
{
    stop_thread("state_update");
}

void Online_Game_State::handle_input(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::M)
        {
            stop_thread("state_update");
            Menu_State* new_state = new Menu_State(this->context->get_window());
            this->context->change_state(new_state);
        }
    }

    if (event.type == sf::Event::MouseButtonPressed && mouse_clicked == false)
    {
        mouse_clicked          = true;
        sf::Vector2i mouse_pos = sf::Mouse::getPosition(context->get_window());
        {
            std::lock_guard<std::mutex> lock(get_mutex());
            try
            {
                click_handler.handle_click(mouse_pos);
            }
            catch (std::exception& e)
            {
                std::cout << "Error handling click: " << e.what() << std::endl;
            }
        }
    }
    if (event.type == sf::Event::MouseButtonReleased)
    {
        mouse_clicked = false;
    }
    // Handle input events specific to the online game state
}

void Online_Game_State::render(sf::RenderWindow& window)
{
    bool is_game_finished = false;
    {
        std::lock_guard<std::mutex> lock(get_mutex());
        renderer.render(state, player_id);
        is_game_finished = state.is_game_finished();
    }

    if (is_game_finished)
    {
        stop_thread("state_update");
        Endgame_State* new_state =
            new Endgame_State(context->get_window(), state.get_players_money());
        this->context->change_state(new_state);
    }
}

int Online_Game_State::get_selected_area_id()
{
    return selected_area_id;
}

void Online_Game_State::set_selected_area_id(int selected_area_id)
{
    this->selected_area_id = selected_area_id;
}
int Online_Game_State::get_selected_position_in_stack()
{
    return selected_position_in_stack;
}
void Online_Game_State::set_selected_position_in_stack(int position)
{
    selected_position_in_stack = position;
}

void Online_Game_State::set_tribe_info_window_state(bool is_window_opened)
{
    tribe_info_window_opened = is_window_opened;
}

bool Online_Game_State::get_tribe_info_window_state()
{
    return tribe_info_window_opened;
}

state::Game_State& Online_Game_State::get_state()
{
    return state;
}
renderer::Game_Renderer& Online_Game_State::get_renderer()
{
    return renderer;
}

void Online_Game_State::update_state()
{
    if (request_version_id() == state.get_version_id())
    {
        return;
    }
    Json::Value root;

    request_state(root);
    state.from_json(root);
}

int Online_Game_State::get_player_id()
{
    return player_id;
}

int Online_Game_State::request_version_id()
{
    sf::Http          http_client("http://" + get_server_ip(), 8888);
    sf::Http::Request request("/game/version/" + std::to_string(room_id));
    request.setMethod(sf::Http::Request::Get);
    request.setField("Session-Token", session_token);

    // Send request
    sf::Http::Response response = http_client.sendRequest(request);
    if (response.getStatus() != sf::Http::Response::Ok)
    {
        throw std::runtime_error("Failed to get version id from server: " + response.getBody());
    }
    return std::stoi(response.getBody());
}

void Online_Game_State::request_state(Json::Value& root)
{
    sf::Http          http_client("http://" + get_server_ip(), 8888);
    sf::Http::Request request("/game/state/" + std::to_string(room_id));
    request.setMethod(sf::Http::Request::Get);
    request.setField("Session-Token", session_token);

    // Send request
    sf::Http::Response response = http_client.sendRequest(request);

    if (response.getStatus() != sf::Http::Response::Ok)
    {
        throw std::runtime_error("Failed to get state from server: " + response.getBody());
    }

    std::istringstream      iss(response.getBody());
    Json::CharReaderBuilder builder;
    std::string             errors;

    bool ok = Json::parseFromStream(builder, iss, &root, &errors);

    if (!ok)
    {
        throw std::runtime_error("Failed to parse state JSON: " + errors);
    }
}

void Online_Game_State::send_command(Json::Value& command_json)
{
    sf::Http          http_client("http://" + get_server_ip(), 8888);
    sf::Http::Request request("/game/command/" + std::to_string(room_id));
    request.setMethod(sf::Http::Request::Post);
    request.setField("Session-Token", session_token);

    Json::StreamWriterBuilder writer_builder;
    std::string               command_str = Json::writeString(writer_builder, command_json);
    request.setBody(command_str);

    // Send request
    sf::Http::Response response = http_client.sendRequest(request);
    if (response.getStatus() != sf::Http::Response::Ok)
    {
        throw std::runtime_error("Failed to send command to server: " + response.getBody());
    }
}
}  // namespace client
