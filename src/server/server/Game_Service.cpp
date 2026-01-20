#include <unistd.h>

#include <thread>

#include "server.h"
#include "state/Player_Type.h"
#include "ai.h"
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


    void ai_process_online(Game_Service& service,
                      std::vector<state::Player_Type> player_types,std::string engine_key)
{
    // TODO add enum name of ais for switch

    bool              running = true;
    std::vector<std::shared_ptr<ai::Ai_Interface>> ais;
    for (int i = 0; i < player_types.size(); i++)
    switch (player_types.at(i))
    {
        case state::Random_AI:
            ais.emplace_back(std::make_shared<ai::Ai_Random>(service.get_engines().at(engine_key)->get_state(), i));
            break;
        case state::Heuristic_AI:
            ais.emplace_back(std::make_shared<ai::Ai_Heuristic>(service.get_engines().at(engine_key)->get_state(), i));
            break;
        case state::Advanced_AI:
            ais.emplace_back(std::make_shared<ai::Ai_Advanced>(service.get_engines().at(engine_key)->get_state(), i));
            break;
        default:
            break;
    }

    while (running)
    {
        for (auto ai : ais) {
            if (!running) {break;}      // if one ai ends turn and next one would want to play
            bool is_my_turn = false;
            {
                std::lock_guard<std::mutex> lock(service.get_mutex());
                // update state
                auto engine = service.get_engines().at(engine_key);

                if (engine->get_state_version_id() != ai->get_state().get_version_id())
                {
                    ai->update_state(engine->get_state());
                }
            }
            is_my_turn = (ai->get_state().get_current_player().id == ai->id);
            // give command if needed

            if (is_my_turn)
            {
                std::shared_ptr<engine::Command> command =
                    ai->give_command(ai->get_state().get_current_turn_phase());
                {
                    std::lock_guard<std::mutex> lock(service.get_mutex());
                    service.get_engines().at(engine_key)->add_command(command);
                }
            }
            {
                std::lock_guard<std::mutex> lock(service.get_mutex());
                running = ! ai->get_state().is_game_finished();
            }
        }
        usleep(1000000);
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
        std::pair<std::vector<std::string>, std::vector<state::Player_Type>> start_infos =
            room_service.get_room_start_infos(std::stoi(room_id_str));
        launch_game(std::stoi(room_id_str), start_infos.first, start_infos.second);
        room_service.set_room_state(std::stoi(room_id_str), Room_State::IN_GAME);
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
void Game_Service::launch_game(int room_id, std::vector<std::string> player_names, std::vector<state::Player_Type>  player_types)
{
    {
        std::lock_guard<std::mutex> lock(mtx);
        engines.emplace(std::to_string(room_id), std::make_unique<engine::Engine>(player_names));
    }


    ais[std::to_string(room_id)] = std::thread(
        [](Game_Service& service, std::vector<state::Player_Type> player_types, std::string engine_key)
                    { ai_process_online(service, player_types, engine_key); },
                    std::ref(*this), player_types, std::to_string(room_id));


}
void Game_Service::stop_game(int room_id)
{
    std::lock_guard<std::mutex> lock(mtx);
    engines.erase(std::to_string(room_id));
}
std::unordered_map<std::string, std::shared_ptr<engine::Engine>>& Game_Service::get_engines()
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