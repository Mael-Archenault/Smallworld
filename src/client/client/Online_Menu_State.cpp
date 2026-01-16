#include <SFML/Network.hpp>
#include <iostream>

#include "client.h"
namespace client
{

Online_Menu_State::Online_Menu_State()
{
    std::cout << "Online Menu State" << std::endl;
}

void Online_Menu_State::handle_input(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::M)
        {
            std::cout << "Switching to Menu State" << std::endl;
            Menu_State* new_state = new Menu_State();
            this->context->change_state(new_state);
        }

        if (event.key.code == sf::Keyboard::C)
        {
            int room_id = send_create_request();
            std::cout << "Created room with id: " << room_id << std::endl;

            Online_Lobby_State* new_state = new Online_Lobby_State(room_id, session_token);
            this->context->change_state(new_state);
        }

        if (event.key.code == sf::Keyboard::J)
        {
            int room_id;
            std::cout << "Enter room id to join: ";
            std::cin >> room_id;
            send_join_request(room_id);
            std::cout << "Sent join request for room id: " << room_id << std::endl;

            Online_Lobby_State* new_state = new Online_Lobby_State(room_id, session_token);
            this->context->change_state(new_state);
        }
    }
    // Handle input events specific to the online menu state
}

void Online_Menu_State::render(sf::RenderWindow& window)
{
    window.clear(sf::Color::Black);
}

void Online_Menu_State::request_session_token()
{
    sf::Http http_client("http://localhost", 8888);
    // Connect to localhost server on port 8888
    sf::Http::Request request("/connect");
    request.setMethod(sf::Http::Request::Post);

    // Send request
    sf::Http::Response response = http_client.sendRequest(request);

    std::cout << "Session token : " << response.getBody() << "\n";

    session_token = response.getBody();
}
int Online_Menu_State::send_create_request()
{
    sf::Http http_client("http://localhost", 8888);
    if (session_token.empty())
    {
        request_session_token();
    }
    sf::Http::Request request("/rooms/create");
    request.setMethod(sf::Http::Request::Post);
    request.setField("Session-Token", session_token);

    // Send request
    sf::Http::Response response = http_client.sendRequest(request);

    int room_id = std::stoi(response.getBody());
    return room_id;
}

void Online_Menu_State::send_join_request(int room_id)
{
    sf::Http http_client("http://localhost", 8888);
    if (session_token.empty())
    {
        request_session_token();
    }

    sf::Http::Request request("/rooms/join/" + std::to_string(room_id));
    request.setMethod(sf::Http::Request::Post);
    request.setField("Session-Token", session_token);

    // Send request
    sf::Http::Response response = http_client.sendRequest(request);
}
}  // namespace client
