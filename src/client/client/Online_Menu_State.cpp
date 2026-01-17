#include <SFML/Network.hpp>
#include <iostream>

#include "client.h"
namespace client
{

Online_Menu_State::Online_Menu_State(sf::RenderWindow& window) : renderer(window)
{
    std::cout << "Online Menu State" << std::endl;
    room_id_str       = "";
    modifying_room_id = false;
}

void Online_Menu_State::handle_input(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::M)
        {
            std::cout << "Switching to Menu State" << std::endl;
            Menu_State* new_state = new Menu_State(this->context->get_window());
            this->context->change_state(new_state);
        }

        if (modifying_room_id)
        {
            if (event.key.code == sf::Keyboard::Backspace && !room_id_str.empty())
            {
                room_id_str.pop_back();
            }
            else if (event.key.code >= sf::Keyboard::Num0 && event.key.code <= sf::Keyboard::Num9 &&
                     room_id_str.size() < 6)
            {
                room_id_str += static_cast<char>(event.key.code - sf::Keyboard::Num0 + '0');
            }
            else if (event.key.code >= sf::Keyboard::Numpad0 &&
                     event.key.code <= sf::Keyboard::Numpad9 && room_id_str.size() < 6)
            {
                room_id_str += static_cast<char>(event.key.code - sf::Keyboard::Numpad0 + '0');
            }
        }
    }

    if (event.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2i mouse_pos = sf::Mouse::getPosition(context->get_window());
        std::unordered_map<std::string, sf::FloatRect> layout_infos = renderer.get_layout_infos();

        if (!modifying_room_id)
        {
            if (layout_infos["create_button"].contains(static_cast<sf::Vector2f>(mouse_pos)))
            {
                int room_id = send_create_request();
                std::cout << "Created room with id: " << room_id << std::endl;

                Online_Lobby_State* new_state = new Online_Lobby_State(room_id, session_token);
                this->context->change_state(new_state);
                return;
            }

            if (layout_infos["join_button"].contains(static_cast<sf::Vector2f>(mouse_pos)))
            {
                int room_id = stoi(room_id_str);
                send_join_request(room_id);
                std::cout << "Sent join request for room id: " << room_id_str << std::endl;

                Online_Lobby_State* new_state = new Online_Lobby_State(room_id, session_token);
                this->context->change_state(new_state);
                return;
            }
            if (layout_infos["room_id_box"].contains(static_cast<sf::Vector2f>(mouse_pos)))
            {
                modifying_room_id = true;
                return;
            }
        }

        if (!layout_infos["room_id_box"].contains(static_cast<sf::Vector2f>(mouse_pos)))
        {
            modifying_room_id = false;
        }
    }

    // Handle input events specific to the online menu state
}

void Online_Menu_State::render(sf::RenderWindow& window)
{
    renderer.render(modifying_room_id, room_id_str);
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
