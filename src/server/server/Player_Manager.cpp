#include <unistd.h>

#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

#include "server.h"

namespace server
{

void sessions_update_process(Player_Manager& manager)
{
    bool running = true;
    while (running)
    {
        manager.remove_inactive_players();
        usleep(5000000);  // checking every 5 seconds
        running = manager.get_sessions_update_thread_running();
    }
}
Player_Manager::Player_Manager() : room_service(nullptr)
{
    sessions_update_thread_running = true;  // mark running before starting the thread
    sessions_update_thread         = std::thread(
        [](Player_Manager& manager) { sessions_update_process(manager); }, std::ref(*this));
}

Player_Manager::~Player_Manager()
{
    {
        std::lock_guard<std::mutex> lock(mtx);
        sessions_update_thread_running = false;
    }

    if (sessions_update_thread.joinable())
    {
        sessions_update_thread.join();
    }
}

std::string Player_Manager::create_player(std::string name)
{
    std::string session_token = generate_session_token();
    connected_players.push_back(Player(name, state::Human, session_token));
    std::cout << "Created player: " << connected_players.back().get_name() << std::endl;
    refresh_last_seen(session_token);
    return session_token;
}

Player& Player_Manager::get_player(std::string session_token)
{
    for (auto& player : connected_players)
    {
        if (player.get_session_token() == session_token)
        {
            return player;
        }
    }
    throw std::runtime_error("Player with session token not found");
}

std::string Player_Manager::generate_session_token()
{
    static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::string       id;
    for (int i = 0; i < 16; ++i)
    {
        id += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return id;
}

bool Player_Manager::verify_session_token(std::string session_token)
{
    for (auto& player : connected_players)
    {
        if (player.get_session_token() == session_token)
        {
            return true;
        }
    }
    return false;
}
void Player_Manager::refresh_last_seen(std::string session_token)
{
    for (auto& player : connected_players)
    {
        if (player.get_session_token() == session_token)
        {
            player.set_last_seen(clock.now());
        }
    }
}

void Player_Manager::remove_inactive_players()
{
    if (room_service == nullptr)
    {  // reference to room_service not set yet, we cannot clean
        return;
    }
    std::lock_guard<std::mutex> lock(mtx);
    auto                        now = clock.now();

    // reverse iterating to safely removing players while iterating
    for (int i = connected_players.size() - 1; i >= 0; i--)
    {
        auto& player = connected_players.at(i);
        auto  duration =
            std::chrono::duration_cast<std::chrono::seconds>(now - player.get_last_seen());
        if (duration.count() >= 5)
        {
            // remove the player from its room (if any)
            try
            {
                room_service->exit_room(player.get_room(), player.get_session_token());
            }
            catch (std::exception& e)
            {
            }

            // Remove the player from the vector
            connected_players.erase(connected_players.begin() + i);
            std::cout << "Removed inactive player: " << player.get_name() << std::endl;
        }
    }
}
bool Player_Manager::get_sessions_update_thread_running()
{
    std::lock_guard<std::mutex> lock(mtx);
    return sessions_update_thread_running;
}

void Player_Manager::set_room_service_reference(Room_Service* room_service_ref)
{
    room_service = room_service_ref;
}
}  // namespace server