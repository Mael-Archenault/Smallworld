#include <unistd.h>

#include <SFML/Network.hpp>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>

#include "client.h"
namespace client
{

void lobby_update_process(Online_Lobby_State& client)
{
    bool running = true;
    while (running)
    {
        {
            std::lock_guard<std::mutex>(client.get_mutex());
            client.request_lobby_state();
        }
        usleep(1000000);

        running = client.get_running_flag("lobby_update");
    }
}

Online_Lobby_State::Online_Lobby_State(int room_id, std::string session_token,
                                       sf::RenderWindow& window)
    : room_id(room_id), session_token(session_token), renderer(window, std::to_string(room_id))
{
    std::cout << "Online Lobby State" << std::endl;

    register_thread("lobby_update",
                    std::thread([](client::Online_Lobby_State& client)
                                { lobby_update_process(client); }, std::ref(*this)));
    start_thread("lobby_update");
}

Online_Lobby_State::~Online_Lobby_State()
{
    stop_thread("lobby_update");
}

void Online_Lobby_State::handle_input(sf::Event event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2i mouse_pos = sf::Mouse::getPosition(context->get_window());
        std::unordered_map<std::string, sf::FloatRect> layout_infos = renderer.get_layout_infos();

        if (layout_infos["exit_button"].contains(static_cast<sf::Vector2f>(mouse_pos)))
        {
            send_exit_request();
            std::cout << "Switching to Online Menu State" << std::endl;
            stop_thread("lobby_update");
            Online_Menu_State* new_state =
                new Online_Menu_State(context->get_window(), context->get_name());
            this->context->change_state(new_state);
        }
        if (layout_infos["start_button"].contains(static_cast<sf::Vector2f>(mouse_pos)))
        {
            std::cout << "Switching to Online Game State" << std::endl;
            stop_thread("lobby_update");
            send_start_request();
            Online_Game_State* new_state =
                new Online_Game_State(this->context->get_window(), room_id, session_token);
            this->context->change_state(new_state);
        }
    }
    // Handle input events specific to the online lobby state
}

void Online_Lobby_State::render(sf::RenderWindow& window)
{
    std::lock_guard<std::mutex> lock(get_mutex());
    renderer.render(player_names);
}

void Online_Lobby_State::request_lobby_state()
{
    sf::Http http_client("http://localhost", 8888);
    // Connect to localhost server on port 8888
    sf::Http::Request request("/rooms/state/" + std::to_string(room_id));
    request.setMethod(sf::Http::Request::Get);
    request.setField("Session-Token", session_token);

    // Send request
    sf::Http::Response response = http_client.sendRequest(request);

    {
        std::lock_guard<std::mutex> lock(get_mutex());
        player_names.clear();
        std::stringstream ss(response.getBody());
        std::string       t;
        char              del = ',';
        while (getline(ss, t, del))
        {
            player_names.push_back(t);
        }
    }
}
void Online_Lobby_State::send_start_request()
{
    sf::Http http_client("http://localhost", 8888);

    sf::Http::Request request("/game/start/" + std::to_string(room_id));
    request.setMethod(sf::Http::Request::Post);
    request.setField("Session-Token", session_token);

    sf::Http::Response response = http_client.sendRequest(request);
}

void Online_Lobby_State::send_exit_request()
{
    sf::Http http_client("http://localhost", 8888);

    sf::Http::Request request("/rooms/exit/" + std::to_string(room_id));
    request.setMethod(sf::Http::Request::Post);
    request.setField("Session-Token", session_token);

    sf::Http::Response response = http_client.sendRequest(request);
}
}  // namespace client
