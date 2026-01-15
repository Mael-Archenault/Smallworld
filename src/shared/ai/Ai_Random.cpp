//
// Created by julien on 11/20/25.
//
#include <limits.h>

#include <memory>
#include <random>
#include <stdexcept>

#include "ai.h"
#include "engine.h"

namespace ai
{

Ai_Random::Ai_Random(state::Game_State state, int player_id) : Ai_Interface(state, player_id) {}

std::shared_ptr<engine::Command> Ai_Random::give_command_Start()
{
    std::random_device rd;
    if (state.get_current_player().get_tribes().first == nullptr)
    {
        std::mt19937 rng(rd());
        int random_tribe_position = rng() % std::min(state.get_current_player().get_money(), 5);
        return std::make_unique<engine::Choose_Species_Command>(id, random_tribe_position);
    }
    std::mt19937 rng(rd());
    long         random_command = rng() % 2;
    if (random_command == 0)
    {
        return std::make_unique<engine::Decline_Command>(id);
    }
    else
    {
        return std::make_unique<engine::Start_Conquest_Command>(id);
    }
}

std::shared_ptr<engine::Command> Ai_Random::give_command_Conquer()
{
    std::random_device rd;
    std::mt19937       rng(rd());
    long               random_command = rng() % 4;
    if (random_command == 0)
    {
        return std::make_unique<engine::End_Conquer_Command>(id);
    }
    else
    {
        auto attackable_area = state.get_conquest_prices(id);
        int  random_area_id  = attackable_area.at(rng() % attackable_area.size()).first;
        for (auto area : attackable_area)
        {
            if (area.first == random_area_id)
            {
                int required_units  = area.second;
                int available_units = state.get_free_units_number(id);

                return std::make_unique<engine::Conquer_Command>(
                    id, random_area_id,
                    std::min(required_units, available_units),
                    required_units > available_units);
            }
        }
    }
}

std::shared_ptr<engine::Command> Ai_Random::give_command_Redeploy()
{
    std::random_device rd;
    std::mt19937       rng(rd());
    int                available_units = state.get_free_units_number(id);
    if (available_units == 0)
    {
        available_units =
            INT_MAX;  // Putting an absurd value, as we must use the redeploy method to end the
                      // turn. the value is never used as the number is detected too high.
    }

    int  random_available_units = rng() % available_units + 1;
    auto Areas                  = state.get_current_player().get_redeployable_areas();
    int  random_area_id         = Areas.at(rng() % Areas.size());

    return std::make_unique<engine::Redeploy_Command>(id, random_area_id, random_available_units);
}
}  // namespace ai