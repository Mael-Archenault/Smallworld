#include <unistd.h>

#include <SFML/Network.hpp>
#include <iostream>
#include <mutex>
#include <thread>

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
        {
            std::lock_guard<std::mutex>(client.get_mutex());
            running = client.get_thread_state();
        }
    }
}

Online_Lobby_State::Online_Lobby_State(int room_id, std::string session_token)
    : room_id(room_id), session_token(session_token)
{
    std::cout << "Online Lobby State" << std::endl;
    lobby_update_thread = std::thread(
        [](client::Online_Lobby_State& client) { lobby_update_process(client); }, std::ref(*this));
    lobby_update_running = true;
}

Online_Lobby_State::~Online_Lobby_State()
{
    stop_lobby_update_thread();
}

void Online_Lobby_State::stop_lobby_update_thread()
{
    {
        std::lock_guard<std::mutex> lock(mtx);
        lobby_update_running = false;
    }
    if (lobby_update_thread.joinable())
    {
        lobby_update_thread.join();
    }
}

void Online_Lobby_State::handle_input(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::E)
        {
            send_exit_request();
            std::cout << "Switching to Online Menu State" << std::endl;
            stop_lobby_update_thread();
            Online_Menu_State* new_state = new Online_Menu_State();
            this->context->change_state(new_state);
        }
        if (event.key.code == sf::Keyboard::M)
        {
            std::cout << "Switching to Menu State" << std::endl;
            stop_lobby_update_thread();
            Menu_State* new_state = new Menu_State();
            this->context->change_state(new_state);
        }
        if (event.key.code == sf::Keyboard::G)
        {
            std::cout << "Switching to Online Game State" << std::endl;
            stop_lobby_update_thread();
            Online_Game_State* new_state = new Online_Game_State();
            this->context->change_state(new_state);
        }
    }
    // Handle input events specific to the online lobby state
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

    std::cout << "Status : " << response.getStatus() << "\n";
    std::cout << response.getBody() << "\n";
}

void Online_Lobby_State::send_exit_request()
{
    sf::Http http_client("http://localhost", 8888);

    sf::Http::Request request("/rooms/exit/" + std::to_string(room_id));
    request.setMethod(sf::Http::Request::Post);
    request.setField("Session-Token", session_token);

    sf::Http::Response response = http_client.sendRequest(request);
}
bool Online_Lobby_State::get_thread_state()
{
    return lobby_update_running;
}
std::mutex& Online_Lobby_State::get_mutex()
{
    return mtx;
}
}  // namespace client
