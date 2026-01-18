#include <iostream>

#include "server.h"
namespace server
{

Room_Service::Room_Service(Player_Manager& player_manager)
    : Service_Interface("/rooms"), player_manager(player_manager), next_room_id(0)
{
}

Http_Status Room_Service::get(std::string& in, std::string& out, std::string url,
                              std::string session_token)
{
    if (url.find("/state/") != 0)
    {
        return Http_Status::BAD_REQUEST;
    }

    size_t      slash_pos   = url.find('/', 1);
    std::string action      = url.substr(0, slash_pos);
    std::string room_id_str = url.substr(slash_pos + 1);

    int room_id = std::stoi(room_id_str);
    std::cout << "Room_id : " << room_id << "Action :" << action << std::endl;
    if (action == "/state")
    {
        try
        {
            std::string room_state = get_room_state(room_id, session_token);
            out                    = room_state;
            std::cout << "Room state: " << room_state << std::endl;
        }
        catch (std::exception& e)
        {
            std::cout << "Error while getting room state: " << e.what() << std::endl;
            return Http_Status::BAD_REQUEST;
        }
        return Http_Status::OK;
    }
    return Http_Status::BAD_REQUEST;
}

Http_Status Room_Service::post(std::string& in, std::string& out, std::string url,
                               std::string session_token)
{
    // creating a room
    if (url == "/create")
    {
        try
        {
            int room_id = create_room(session_token);
            out         = std::to_string(room_id);
        }
        catch (std::exception& e)
        {
            out = "Error while creating room: " + std::string(e.what());
            return Http_Status::BAD_REQUEST;
        }
        return Http_Status::CREATED;
    }

    if (url.find("/join/") != 0 && url.find("/exit/") != 0)
    {
        return Http_Status::BAD_REQUEST;
    }

    // action on a specific room -> /join/{id} or /exit/{id}

    size_t      slash_pos   = url.find('/', 1);
    std::string action      = url.substr(0, slash_pos);
    std::string room_id_str = url.substr(slash_pos + 1);
    int         room_id     = std::stoi(room_id_str);

    if (verify_room_id(room_id) == false)
    {
        return Http_Status::NOT_FOUND;
    }
    std::cout << "room id : " << room_id_str << " action : " << action << std::endl;
    if (action == "/join")
    {
        try
        {
            join_room(room_id, session_token);
        }
        catch (std::exception& e)
        {
            out = "Error while joining room: " + std::string(e.what());
            std::cout << "Error while joining room: " << e.what() << std::endl;
            return Http_Status::BAD_REQUEST;
        }
        return Http_Status::OK;
    }
    if (action == "/exit")
    {
        try
        {
            exit_room(room_id, session_token);
        }
        catch (std::exception& e)
        {
            out = "Error while exiting room: " + std::string(e.what());
            std::cout << "Error while exiting room: " << e.what() << std::endl;
            return Http_Status::BAD_REQUEST;
        }
        return Http_Status::OK;
    }
    return Http_Status::METHOD_NOT_ALLOWED;
}

int Room_Service::create_room(std::string creator_session_token)
{
    Player& creator = player_manager.get_player(creator_session_token);

    if (creator.get_room() != -1)
    {
        throw std::runtime_error("Player already in a room");
    }
    std::cout << "Creating room : " << next_room_id << std::endl;
    rooms.emplace_back(next_room_id, creator);
    next_room_id++;
    creator.set_room(rooms.back().id);
    return rooms.back().id;
}

void Room_Service::join_room(int room_id, std::string player_session_token)
{
    Player& player = player_manager.get_player(player_session_token);
    if (player.get_room() != -1)
    {
        throw std::runtime_error("Player already in a room");
    }
    // find room
    for (auto& room : rooms)
    {
        if (room.id == room_id)
        {
            room.add_player(player);
            player.set_room(room_id);
            return;
        }
    }
    throw std::runtime_error("Room with id not found");
}

void Room_Service::exit_room(int room_id, std::string player_session_token)
{
    Player& player = player_manager.get_player(player_session_token);

    if (player.get_room() == -1)
    {
        throw std::runtime_error("Player not in any room");
    }

    if (player.get_room() != room_id)
    {
        throw std::runtime_error("Player not in the specified room");
    }

    // find room
    for (auto& room : rooms)
    {
        if (room.id == room_id)
        {
            room.remove_player(player);
            player_manager.get_player(player_session_token).set_room(-1);
            if (room.is_empty())
            {
                // remove room
                std::cout << "Removing empty room with id: " << room_id << std::endl;
                delete_room(room_id);
            }
            return;
        }
    }
    throw std::runtime_error("Room with id not found");
}

std::string Room_Service::get_room_state(int room_id, std::string session_token)
{
    if (player_manager.get_player(session_token).get_room() != room_id)
    {
        throw std::runtime_error("Player not in the specified room");
    }
    for (auto& room : rooms)
    {
        if (room.id == room_id)
        {
            return room.get_state();
        }
    }
    throw std::runtime_error("Room with id not found");
}

bool Room_Service::verify_room_id(int room_id)
{
    for (auto& room : rooms)
    {
        if (room.id == room_id)
        {
            return true;
        }
    }
    return false;
}

void Room_Service::delete_room(int room_id)
{
    for (auto it = rooms.begin(); it != rooms.end(); ++it)
    {
        if (it->id == room_id)
        {
            rooms.erase(it);
            return;
        }
    }
}

std::pair<int, std::vector<std::string>> Room_Service::get_room_start_infos(int room_id)
{
    for (auto& room : rooms)
    {
        if (room.id == room_id)
        {
            return room.get_start_infos();
        }
    }
    throw std::runtime_error("Room with id not found");
}
}  // namespace server