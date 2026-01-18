#include <unistd.h>

#include <thread>

#include "server.h"

namespace server
{

void engines_process(Game_Service& service)
{
    bool running = true;

    while (running)
    {
        for (auto& games : service.get_engines())
        {
            {
                std::lock_guard<std::mutex> lock(service.get_mutex());
                try
                {
                    games.second->update();
                }
                catch (std::exception& e)
                {
                    games.second->remove_last_command();
                }
            }
        }

        usleep(50000 / 3);  // 60 updates per second
        running = service.get_engines_thread_flag();
    }
}
Game_Service::Game_Service(Room_Service& room_service)
    : Service_Interface("/game"), room_service(room_service)
{
    engines_update_thread =
        std::thread([](Game_Service& service) { engines_process(service); }, std::ref(*this));
    engines_update_running = true;
}

Http_Status Game_Service::get(std::string& in, std::string& out, std::string url,
                              std::string session_token)
{
    size_t      slash_pos   = url.find('/', 1);
    std::string action      = url.substr(0, slash_pos);
    std::string room_id_str = url.substr(slash_pos + 1);

    if (action == "/version")
    {
        std::lock_guard<std::mutex> lock(mtx);
        int                         version_id = engines[room_id_str]->get_state_version_id();
        out                                    = std::to_string(version_id);
        return Http_Status::OK;
    }

    if (action == "/state")
    {
        std::lock_guard<std::mutex> lock(mtx);
        Json::Value                 state_json = engines[room_id_str]->get_state_json();
        Json::StreamWriterBuilder   writer_builder;
        writer_builder["indentation"] = "";  // No pretty print
        std::unique_ptr<Json::StreamWriter> writer(writer_builder.newStreamWriter());
        std::ostringstream                  oss;
        writer->write(state_json, &oss);
        out = oss.str();
        return Http_Status::OK;
    }

    return Http_Status::METHOD_NOT_ALLOWED;
}

Http_Status Game_Service::post(std::string& in, std::string& out, std::string url,
                               std::string session_token)
{
    size_t      slash_pos   = url.find('/', 1);
    std::string action      = url.substr(0, slash_pos);
    std::string room_id_str = url.substr(slash_pos + 1);

    if (action == "/start")
    {
        std::pair<int, std::vector<std::string>> start_infos =
            room_service.get_room_start_infos(std::stoi(room_id_str));
        launch_game(std::stoi(room_id_str), start_infos.first, start_infos.second);
        return Http_Status::OK;
    }

    if (action == "/command")
    {
        int room_id = std::stoi(room_id_str);
        add_command_to_game(room_id, in);
        return Http_Status::OK;
    }

    return Http_Status::METHOD_NOT_ALLOWED;
}
void Game_Service::launch_game(int room_id, int nb_players, std::vector<std::string> player_names)
{
    std::lock_guard<std::mutex> lock(mtx);
    engines.emplace(std::to_string(room_id),
                    std::make_unique<engine::Engine>(nb_players, player_names));
}
void Game_Service::stop_game(int room_id)
{
    std::lock_guard<std::mutex> lock(mtx);
    engines.erase(std::to_string(room_id));
}
std::unordered_map<std::string, std::unique_ptr<engine::Engine>>& Game_Service::get_engines()
{
    return engines;
}

void Game_Service::add_command_to_game(int room_id, std::string command)
{
    Json::Value             root;
    Json::CharReaderBuilder builder;
    std::string             errors;

    std::istringstream iss(command);

    bool ok = Json::parseFromStream(builder, iss, &root, &errors);

    if (!ok)
    {
        throw std::runtime_error("Failed to parse command JSON: " + errors);
    }

    {
        std::lock_guard<std::mutex> lock(mtx);
        engines[std::to_string(room_id)]->add_command(root);
    }
}

std::mutex& Game_Service::get_mutex()
{
    return mtx;
}

bool Game_Service::get_engines_thread_flag()
{
    std::lock_guard<std::mutex> lock(mtx);
    return engines_update_running;
}
}  // namespace server