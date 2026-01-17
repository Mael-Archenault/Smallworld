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

        running = client.get_running_flag("lobby_update");
    }
}

Online_Lobby_State::Online_Lobby_State(int room_id, std::string session_token)
    : room_id(room_id), session_token(session_token)
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
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::E)
        {
            send_exit_request();
            std::cout << "Switching to Online Menu State" << std::endl;
            stop_thread("lobby_update");
            Online_Menu_State* new_state = new Online_Menu_State(this->context->get_window());
            this->context->change_state(new_state);
        }
        if (event.key.code == sf::Keyboard::M)
        {
            std::cout << "Switching to Menu State" << std::endl;
            stop_thread("lobby_update");
            Menu_State* new_state = new Menu_State(this->context->get_window());
            this->context->change_state(new_state);
        }
        if (event.key.code == sf::Keyboard::G)
        {
            std::cout << "Switching to Online Game State" << std::endl;
            stop_thread("lobby_update");
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
}  // namespace client
