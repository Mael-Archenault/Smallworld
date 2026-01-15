#include <functional>
#include <unordered_map>

#include "engine.h"

namespace engine
{

std::unordered_map<std::string, std::function<std::unique_ptr<Command>()>> name_to_command{
    {"Choose_Species_Command", []() { return std::make_unique<Choose_Species_Command>(0, 0); }},
    {"Conquer_Command", []() { return std::make_unique<Conquer_Command>(0, 0, 0, 0); }},
    {"Decline_Command", []() { return std::make_unique<Decline_Command>(0); }},
    {"End_Conquer_Command", []() { return std::make_unique<End_Conquer_Command>(0); }},
    {"Redeploy_Command", []() { return std::make_unique<Redeploy_Command>(0, 0, 0); }},
    {"Start_Conquest_Command", []() { return std::make_unique<Start_Conquest_Command>(0); }}};

Command::Command(int player_id) : player_id(player_id) {}

void Command::set_player_id(int new_player_id)
{
    player_id = new_player_id;
}

std::unique_ptr<Command> Command::create_from_json(Json::Value& root)
{
    std::unique_ptr<Command> command_ptr = name_to_command[root["command_name"].asString()]();
    command_ptr->from_json(root);
    return command_ptr;
}

}  // namespace engine