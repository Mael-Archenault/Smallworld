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
            try
            {
                client.request_lobby_state();
            }
            catch (const std::runtime_error& e)
            {
                std::cout << "Failed to request lobby state: " << e.what() << std::endl;
                // switching back to menu

                client.stop_thread("lobby_update");
                Online_Menu_State* new_state =
                    new Online_Menu_State(client.context->get_window(), client.context->get_name());
                client.context->change_state(new_state);
                delete &client;
                return;
            }
        }
        usleep(1000000);

        running = client.get_running_flag("lobby_update");
    }
}

Online_Lobby_State::Online_Lobby_State(int room_id, std::string session_token,
                                       sf::RenderWindow& window)
    : room_id(room_id),
      session_token(session_token),
      renderer(window, std::to_string(room_id)),
      game_remotely_launched(false),
      player_id(0),
      is_owner(false),
      modifying_name(false),
      ai_adder_window_opened(false),
      selected_player_type(state::Player_Type::Random_AI)
{
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
        if (event.key.code == sf::Keyboard::BackSpace && !added_ai_name.empty())
        {
            added_ai_name.pop_back();
            return;
        }
        if (event.key.code == sf::Keyboard::Enter)
        {
            modifying_name = false;
            return;
        }

        if (event.key.code == sf::Keyboard::Space && added_ai_name.size() < 20)
        {
            added_ai_name += ' ';
            return;
        }
    }
    if (event.type == sf::Event::TextEntered)

    {
        sf::Uint32 unicode = event.text.unicode;

        if (modifying_name)
        {
            if (unicode >= 32 && unicode < 128 &&
                added_ai_name.size() < 20)  // basic ASCII and limit to 20 chars
            {
                added_ai_name += static_cast<char>(unicode);
            }
        }
    }
    if (event.type == sf::Event::MouseButtonPressed)
    {
        register_click(sf::Mouse::getPosition(context->get_window()));
        register_layout(renderer.get_layout_infos());

        if (ai_adder_window_opened)
        {
            if (!clicked_on("adder_window") || clicked_on("adder_window_close_button"))
            {
                ai_adder_window_opened = false;
                return;
            }

            if (modifying_name && !clicked_on("adder_window_name_box"))
            {
                modifying_name = false;
                return;
            }
            if (clicked_on("adder_window_name_box"))
            {
                modifying_name = true;
                return;
            }

            if (clicked_on("adder_window_random_ai_button"))
            {
                selected_player_type = state::Player_Type::Random_AI;
            }

            if (clicked_on("adder_window_heuristic_ai_button"))
            {
                selected_player_type = state::Player_Type::Heuristic_AI;
            }
            if (clicked_on("adder_window_advanced_ai_button"))
            {
                selected_player_type = state::Player_Type::Advanced_AI;
            }

            if (clicked_on("adder_window_add_button"))
            {
                if (added_ai_name.empty())
                {
                    added_ai_name = "Player" + std::to_string(player_names.size() + 1);
                }

                try
                {
                    send_ai_creation_request(added_ai_name, selected_player_type);
                }
                catch (const std::runtime_error& e)
                {
                    std::cout << "Failed to add AI: " << e.what() << std::endl;
                    ai_adder_window_opened = false;
                }
                ai_adder_window_opened = false;
                added_ai_name.clear();
                return;
            }
        }

        if (clicked_on("exit_button"))
        {
            try
            {
                send_exit_request();
            }
            catch (const std::runtime_error& e)
            {
                std::cout << "Failed to exit lobby: " << e.what() << std::endl;
                return;
            }
            stop_thread("lobby_update");
            Online_Menu_State* new_state =
                new Online_Menu_State(context->get_window(), context->get_name());
            this->context->change_state(new_state);
            delete this;
            return;
        }
        if (clicked_on("start_button"))
        {
            try
            {
                send_start_request();
            }
            catch (const std::runtime_error& e)
            {
                std::cout << "Failed to start game: " << e.what() << std::endl;
                return;
            }
            stop_thread("lobby_update");
            Online_Game_State* new_state = new Online_Game_State(
                this->context->get_window(), player_names, room_id, session_token, player_id);
            this->context->change_state(new_state);
            delete this;
            return;
        }

        if (clicked_on("add_ai_button"))
        {
            ai_adder_window_opened = true;
            return;
        }

        if (clicked_on("remove_ai_button"))
        {
            try
            {
                send_ai_removal_request();
            }
            catch (const std::runtime_error& e)
            {
                std::cout << "Failed to remove AI: " << e.what() << std::endl;
                return;
            }
            return;
        }
    }
}

void Online_Lobby_State::render(sf::RenderWindow& window)
{
    {
        std::lock_guard<std::mutex> lock(get_mutex());
        renderer.render(player_names, player_types, ai_adder_window_opened, modifying_name,
                        added_ai_name, selected_player_type, is_owner);
    }
    bool flag;
    {
        std::lock_guard<std::mutex> lock(get_mutex());
        flag = game_remotely_launched;
    }
    if (flag)
    {
        stop_thread("lobby_update");
        Online_Game_State* new_state = new Online_Game_State(
            this->context->get_window(), player_names, room_id, session_token, player_id);
        this->context->change_state(new_state);
        delete this;
    }
}

void Online_Lobby_State::request_lobby_state()
{
    // getting names of the players in the lobby
    sf::Http          http_client("http://" + get_server_ip(), 8888);
    sf::Http::Request request("/rooms/state/" + std::to_string(room_id));
    request.setMethod(sf::Http::Request::Get);
    request.setField("Session-Token", session_token);

    sf::Http::Response response = http_client.sendRequest(request);

    if (response.getStatus() != sf::Http::Response::Ok)
    {
        throw std::runtime_error("Failed to get lobby state from server");
        return;
    }

    if (response.getBody() == "Game Launched")
    {
        std::lock_guard<std::mutex> lock(get_mutex());
        game_remotely_launched = true;
        return;
    }
    {
        std::lock_guard<std::mutex> lock(get_mutex());
        player_names.clear();
        player_types.clear();

        std::string temp = response.getBody();

        std::vector<std::string> players_str;
        int                      slash_pos = temp.find("/", 0);
        while (slash_pos != std::string::npos)
        {
            players_str.push_back(temp.substr(0, slash_pos));
            temp      = temp.substr(slash_pos + 1);
            slash_pos = temp.find("/", 0);
        }
        // last info are the player id and the owning info

        int         coma_pos      = temp.find(",", 0);
        std::string player_id_str = temp.substr(0, coma_pos);
        std::string is_owner_str  = temp.substr(coma_pos + 1);

        player_id = std::stoi(player_id_str);
        is_owner  = std::stoi(is_owner_str) == 1;

        for (int i = 0; i < players_str.size(); i++)
        {
            std::string player_infos = players_str.at(i);
            coma_pos                 = player_infos.find(",", 0);
            player_names.push_back(player_infos.substr(0, coma_pos));
            std::string player_type_str = player_infos.substr(coma_pos + 1);
            player_types.push_back(static_cast<state::Player_Type>(std::stoi(player_type_str)));
        }
    }
}
void Online_Lobby_State::send_start_request()
{
    sf::Http http_client("http://" + get_server_ip(), 8888);

    sf::Http::Request request("/game/start/" + std::to_string(room_id));
    request.setMethod(sf::Http::Request::Post);
    request.setField("Session-Token", session_token);

    sf::Http::Response response = http_client.sendRequest(request);
    if (response.getStatus() != sf::Http::Response::Ok)
    {
        throw std::runtime_error("Failed to send start request to server");
    }
}

void Online_Lobby_State::send_exit_request()
{
    sf::Http http_client("http://" + get_server_ip(), 8888);

    sf::Http::Request request("/rooms/exit/" + std::to_string(room_id));
    request.setMethod(sf::Http::Request::Post);
    request.setField("Session-Token", session_token);

    sf::Http::Response response = http_client.sendRequest(request);
    if (response.getStatus() != sf::Http::Response::Ok)
    {
        throw std::runtime_error("Failed to send exit request to server");
    }
}

void Online_Lobby_State::send_ai_creation_request(std::string name, state::Player_Type type)
{
    sf::Http http_client("http://" + get_server_ip(), 8888);

    sf::Http::Request request("/rooms/add_ai/" + std::to_string(room_id));
    request.setMethod(sf::Http::Request::Post);
    request.setField("Session-Token", session_token);
    request.setBody(std::to_string(static_cast<int>(type)) + "," + name);

    sf::Http::Response response = http_client.sendRequest(request);
    if (response.getStatus() != sf::Http::Response::Ok)
    {
        throw std::runtime_error("Failed to send AI creation request to server");
    }
}

void Online_Lobby_State::send_ai_removal_request()
{
    sf::Http http_client("http://" + get_server_ip(), 8888);

    sf::Http::Request request("/rooms/delete_ai/" + std::to_string(room_id));
    request.setMethod(sf::Http::Request::Post);
    request.setField("Session-Token", session_token);

    sf::Http::Response response = http_client.sendRequest(request);
    if (response.getStatus() != sf::Http::Response::Ok)
    {
        throw std::runtime_error("Failed to send AI removal request to server");
    }
}
}  // namespace client
