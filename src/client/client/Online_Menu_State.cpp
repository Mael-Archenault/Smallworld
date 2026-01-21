#include <SFML/Network.hpp>
#include <iostream>

#include "client.h"
namespace client
{

Online_Menu_State::Online_Menu_State(sf::RenderWindow& window, std::string name) : renderer(window)
{
    room_id_str       = "";
    modifying_room_id = false;
    modifying_name    = false;
    input_name        = name;
}

void Online_Menu_State::handle_input(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Escape)
        {
            Menu_State* new_state = new Menu_State(this->context->get_window());
            this->context->change_state(new_state);
        }

        if (modifying_room_id)
        {
            if (event.key.code == sf::Keyboard::Backspace && !room_id_str.empty())
            {
                room_id_str.pop_back();
            }
            else if (event.key.code == sf::Keyboard::Enter)
            {
                modifying_room_id = false;
            }
        }
        if (modifying_name)
        {
            if (event.key.code == sf::Keyboard::Backspace && !input_name.empty())
            {
                input_name.pop_back();
            }
            else if (event.key.code == sf::Keyboard::Space && input_name.size() < 20)
            {
                input_name += ' ';
            }
            else if (event.key.code == sf::Keyboard::Enter)
            {
                modifying_name = false;
            }
            this->context->set_name(input_name);
        }
    }
    if (event.type == sf::Event::TextEntered)

    {
        sf::Uint32 unicode = event.text.unicode;

        if (modifying_room_id)
        {
            if (unicode >= '0' && unicode <= '9' && room_id_str.size() < 6)  // limit to 6 digits
            {
                room_id_str += static_cast<char>(unicode);
            }
        }
        if (modifying_name)
        {
            if (unicode >= 32 && unicode < 128 &&
                input_name.size() < 20)  // basic ASCII and limit to 20 chars
            {
                input_name += static_cast<char>(unicode);
            }
            this->context->set_name(input_name);
        }
    }

    if (event.type == sf::Event::MouseButtonPressed)
    {
        register_click(sf::Mouse::getPosition(context->get_window()));
        register_layout(renderer.get_layout_infos());

        if (!modifying_room_id && !modifying_name)
        {
            if (clicked_on("create_button"))
            {
                try
                {
                    int room_id = send_create_request();

                    Online_Lobby_State* new_state =
                        new Online_Lobby_State(room_id, session_token, context->get_window());
                    this->context->change_state(new_state);
                    return;
                }
                catch (std::exception& e)
                {
                    std::cout << "Failed to create room: " << e.what() << std::endl;
                    return;
                }
            }

            if (clicked_on("join_button"))
            {
                if (room_id_str.empty())
                {
                    std::cout << "Room ID is empty, cannot join room." << std::endl;
                    return;
                }
                int room_id = stoi(room_id_str);
                try
                {
                    send_join_request(room_id);

                    Online_Lobby_State* new_state =
                        new Online_Lobby_State(room_id, session_token, context->get_window());
                    this->context->change_state(new_state);
                    return;
                }
                catch (std::exception& e)
                {
                    std::cout << "Failed to join room: " << e.what() << std::endl;
                    return;
                }
            }
            if (clicked_on("room_id_box"))
            {
                modifying_room_id = true;
                return;
            }

            if (clicked_on("name_box"))
            {
                modifying_name = true;
                return;
            }
        }

        if (modifying_name)
        {
            if (!clicked_on("name_box"))
            {
                modifying_name = false;
            }
        }
        if (modifying_room_id)
        {
            if (!clicked_on("room_id_box"))
            {
                modifying_room_id = false;
            }
        }
    }

    // Handle input events specific to the online menu state
}

void Online_Menu_State::render(sf::RenderWindow& window)
{
    renderer.render(modifying_room_id, room_id_str, modifying_name, input_name);
}

void Online_Menu_State::request_session_token()
{
    sf::Http          http_client("http://" + get_server_ip(), 8888);
    sf::Http::Request request("/connect");
    request.setMethod(sf::Http::Request::Post);
    request.setBody(this->context->get_name());

    // Send request
    sf::Http::Response response = http_client.sendRequest(request);

    if (response.getStatus() != sf::Http::Response::Ok)
    {
        throw std::runtime_error("Failed to get session token: " + response.getBody());
    }

    session_token = response.getBody();
}
int Online_Menu_State::send_create_request()
{
    sf::Http http_client("http://" + get_server_ip(), 8888);
    if (session_token.empty())
    {
        try
        {
            request_session_token();
        }
        catch (std::exception& e)
        {
            throw std::runtime_error("Cannot create room without session token: " +
                                     std::string(e.what()));
        }
    }
    sf::Http::Request request("/rooms/create");
    request.setMethod(sf::Http::Request::Post);
    request.setField("Session-Token", session_token);

    // Send request
    sf::Http::Response response = http_client.sendRequest(request);

    if (response.getStatus() != sf::Http::Response::Created)
    {
        throw std::runtime_error("Failed to create room: " + response.getBody());
    }

    int room_id = std::stoi(response.getBody());
    return room_id;
}

void Online_Menu_State::send_join_request(int room_id)
{
    sf::Http http_client("http://" + get_server_ip(), 8888);
    if (session_token.empty())
    {
        request_session_token();
    }

    sf::Http::Request request("/rooms/join/" + std::to_string(room_id));
    request.setMethod(sf::Http::Request::Post);
    request.setField("Session-Token", session_token);

    // Send request
    sf::Http::Response response = http_client.sendRequest(request);

    if (response.getStatus() != sf::Http::Response::Ok)
    {
        throw std::runtime_error("Failed to join room: " + response.getBody());
    }
}
}  // namespace client
