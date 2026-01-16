//
// Created by julien on 12/5/25.
//


#include <memory>
#include <engine/Conquer_Command.h>
#include <engine/Decline_Command.h>
#include <engine/End_Conquer_Command.h>
#include <engine/Redeploy_Command.h>
#include <engine/Start_Conquest_Command.h>

#include "random"
#include "Ai_Advanced.h"

#include <algorithm>
#include <iostream>
#include <stack>

#include "engine/Choose_Species_Command.h"
#include "json/json.h"
#define MIN_UNIT_FOR_DECLINE 5
using namespace ai;

Ai_Advanced::Ai_Advanced(state::Game_State state,int player_id) : Ai_Interface(state, player_id), engine(state, player_id) {
    command_stack = {};
    engine.set_state(state);
}

std::pair<int,std::vector<std::shared_ptr<engine::Command>>> Ai_Advanced::calcul_stack(std::shared_ptr<engine::Command> command, state::Game_State new_state, int depth = 0) {
    if (command != nullptr) {
        engine.add_command(command);
        try {
            engine.update(); // problem with state when going to Engine
            new_state = engine.get_state().deep_copy();
        }
        catch (std::invalid_argument &e) {
            engine.remove_last_command();
            std::cout<<e.what()<<std::endl;
            return {0, std::vector<std::shared_ptr<engine::Command>>()};
        }
    }
    else {
        engine.set_state(new_state);
    }
    std::pair<float,std::vector<std::shared_ptr<engine::Command>>>  current_best_node_value_command = {0,std::vector<std::shared_ptr<engine::Command>>()};

    if (engine.get_state().get_free_units_number(id) == 0 || engine.get_state().get_current_turn_phase() == state::REDEPLOY) {
        engine.get_state().get_rewards(id);
        current_best_node_value_command.first = engine.get_state().get_current_player().get_money();
        current_best_node_value_command.second.emplace_back(command);
        return current_best_node_value_command;
    }

    for (std::pair<int,int> area : engine.get_state().get_conquest_prices(id)) {
        if (area.first == 35) continue;
        int required_units = area.second;
        int available_units = engine.get_state().get_free_units_number(id);
        auto new_command = std::make_shared<engine::Conquer_Command>(id,area.first,
                                                    std::min(required_units,available_units),
                                                    (required_units - available_units > 0));

        std::pair<float,std::vector<std::shared_ptr<engine::Command>>> new_node_value_command = calcul_stack(new_command, engine.get_state().deep_copy(), depth + 1);
        engine.set_state(new_state);    //TO move ?

        // if (required_units - available_units > 0) {     //check if dice was needed, in which case the gain is multiplied by the probability
        //     float current_gain = state.get_current_player().get_money();    //TODO check if the ponderation works properly
        //     new_node_value_command.first = current_gain + (new_node_value_command.first - current_gain) * unit_to_proba(required_units - available_units) ;
        // }

        if (depth == -1) {
            std::string areas_str;
            for (auto cmd : new_node_value_command.second) {
                auto cmd_conquer = (engine::Conquer_Command *) cmd.get();
                areas_str.append(std::to_string(cmd_conquer->get_area_id()));
                areas_str.append(", ");
            }
            std::cout << "New commands at depth " << depth << " : \n\tmoney : " << new_node_value_command.first << std::endl;
            std::cout << "\tareas : " << areas_str << std::endl;
        }
        if (current_best_node_value_command.first < new_node_value_command.first) {
            current_best_node_value_command = {new_node_value_command.first,new_node_value_command.second};

        }
    }
    if (depth != 0) {
    current_best_node_value_command.second.emplace_back(command);
    }
    return current_best_node_value_command;

}

float Ai_Advanced::unit_to_proba(int units) {
    switch (units) {
        case 1 : return 0.5;
        case 2 : return 1/3;
        case 3 : return 1/6;
        default : return 0;
    }
}

std::shared_ptr<engine::Command> Ai_Advanced::give_command_Start (){
    if (state.get_current_player().get_tribes().first == nullptr) {
        return std::make_unique<engine::Choose_Species_Command>(id, 0);
    }
    state.gather_free_units(id);
    if (state.get_free_units_number(id) <= MIN_UNIT_FOR_DECLINE)
    {
        return std::make_unique<engine::Decline_Command>(id);
    }

    return std::make_unique<engine::Start_Conquest_Command>(id);
}
std::shared_ptr<engine::Command> Ai_Advanced::give_command_Conquer () {
    engine.set_state(state);
    if (command_stack.empty()) {
        command_stack = calcul_stack(nullptr,state).second;
    }
    auto ret = command_stack.back();
    command_stack.pop_back();
    return ret;
};


std::shared_ptr<engine::Command> Ai_Advanced::give_command_Redeploy (){
    int free_units_number = state.get_free_units_number(id);
    std::vector<int> areas = state.get_redeployable_areas(id);
    state::Area current_area = state.get_map().get_area(areas.at(0));
    std::pair<int,int> area_id_units = {state.get_map().get_area(areas.at(0)).id,
                                        state.get_map().get_area(areas.at(0)).get_units_number()};

    for (auto area_id : areas) {
        if (state.get_map().get_area(area_id).get_units_number() < area_id_units.second) {
            area_id_units = {state.get_map().get_area(areas.at(0)).id,
                                state.get_map().get_area(areas.at(0)).get_units_number()};
        }
    }
    return std::make_unique<engine::Redeploy_Command>(id,area_id_units.first,1);};


