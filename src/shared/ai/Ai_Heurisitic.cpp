//
// Created by julien on 12/4/25.
//

#include <memory>

#include "Ai_Heuristic.h"

#include <engine/Conquer_Command.h>
#include <engine/Decline_Command.h>
#include <engine/End_Conquer_Command.h>
#include <engine/Redeploy_Command.h>
#include <engine/Start_Conquest_Command.h>
#include "engine/Choose_Species_Command.h"

#include "random"

#define MIN_UNIT_FOR_DECLINE 3

ai::Ai_Heuristic::Ai_Heuristic(state::Game_State* state,int player_id) : Ai_Interface(state, player_id) {
}


std::unique_ptr<engine::Command> ai::Ai_Heuristic::give_command_Start ()
{
    if (state->get_current_player().get_tribes().first == nullptr) {
        return std::make_unique<engine::Choose_Species_Command>(id, 0);
    }

    if (state->get_free_units_number(id) <= MIN_UNIT_FOR_DECLINE)
        {
        return std::make_unique<engine::Decline_Command>(id);
    }

    return std::make_unique<engine::Start_Conquest_Command>(id);
}


std::unique_ptr<engine::Command> ai::Ai_Heuristic::give_command_Conquer ()
{
    if (state->get_free_units_number(id) == 0) {
        return std::make_unique<engine::End_Conquer_Command>(id);
    }

    auto attackable_area = state->get_conquest_prices(id);
    std::pair<int,int> area_id_cost_to_attack = {attackable_area.at(0).first, attackable_area.at(0).second};
    for (auto area_id_cost : attackable_area) {
        if (area_id_cost.second <= area_id_cost_to_attack.second) {
            area_id_cost_to_attack = area_id_cost;
        }
    }
    int required_units = area_id_cost_to_attack.second;
    int available_units = state->get_free_units_number(id);
    return std::make_unique<engine::Conquer_Command>(id,area_id_cost_to_attack.first,
                                                                 std::min(required_units,available_units),
                                                                 (required_units - available_units > 0));

}


std::unique_ptr<engine::Command> ai::Ai_Heuristic::give_command_Redeploy ()
{
    int free_units_number = state->get_free_units_number(id);
    std::vector<int> areas = state->get_redeployable_areas(id);
    state::Area current_area = state->get_map().get_area(areas.at(0));
    std::pair<int,int> area_id_units = {state->get_map().get_area(areas.at(0)).id,
                                        state->get_map().get_area(areas.at(0)).get_units_number()};

    for (auto area_id : areas) {
        if (state->get_map().get_area(area_id).get_units_number() < area_id_units.second) {
            area_id_units = {state->get_map().get_area(areas.at(0)).id,
                                state->get_map().get_area(areas.at(0)).get_units_number()};
        }
    }
    return std::make_unique<engine::Redeploy_Command>(id,area_id_units.first,1);
}