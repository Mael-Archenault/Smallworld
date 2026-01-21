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
#define DANGER_COST_LIMIT 10
#define BASE_COST_PRICE 2
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
    if (command_stack.empty()) {
        command_stack = calcul_area_danger(state);
    }
    auto ret = command_stack.back();
    command_stack.pop_back();
    return ret;
};

std::pair<int,bool> Ai_Advanced::dist_to_ennemies(state::Game_State state, int area_id, int cost, std::vector<std::pair<int,bool>> area_status, std::vector<std::pair<char,int>>& visited) //return the cost and if there is an ennemy of cost<=6
{
    int nAreas = (int)state.get_map().get_areas().size();
    if (visited.at(area_id).first) return {visited.at(area_id).second, false};
    visited.at(area_id).first = 1;

    int token_costs = state.get_map().get_area(area_id).get_special_tokens().size();
    int units_costs = state.get_map().get_area(area_id).get_units_number();
    cost += token_costs + units_costs;
    visited.at(area_id).second = cost;
    
    if(cost>=DANGER_COST_LIMIT)
    {
        return {DANGER_COST_LIMIT,false};
    }
    for(auto n_area : state.get_map().get_area(area_id).get_neighbors())
    {
        if (n_area->id < 0 || n_area->id >= nAreas) continue;
        if(area_status.at(n_area->id).first==1)
        {
            return {cost,true};
        }
        else 
        {
            auto visited2 = visited;
            auto dist = dist_to_ennemies(state, n_area->id, cost, area_status, visited2);
            if(dist.second) return dist;
        }
    }
    return {cost,false};
}

std::vector<std::shared_ptr<engine::Command>> Ai_Advanced::calcul_area_danger(state::Game_State state)
{
    int nAreas = (int)state.get_map().get_areas().size();
    //each area has its status, if -1, it's the owner areas, if 0, it's neutral and if 1, it's another player's area
    std::vector<std::pair<int,bool>> area_status(nAreas, {0,false});
    for(int area_id=0; area_id<nAreas; area_id++)
    {
        for(std::pair<int,int> player_id : state.get_all_player_id_money())
        {
            auto* tribe = state.get_map().get_area(area_id).get_owner_tribe();
            if (tribe && tribe->get_owner() && tribe->get_owner()->id == id)
            {
                area_status.at(area_id).first = -1;
                break;
            }
            else if(tribe && tribe->get_owner() && tribe->get_owner()->id == player_id.first)
            {
                area_status.at(area_id).first = 1;
                break;
            }
            else
            {
                area_status.at(area_id).first = 0;
            }
        }
    }
    
    //calcul the cost of each area
    std::vector<int> area_danger(nAreas,DANGER_COST_LIMIT*2);
    for(int area_id : state.get_redeployable_areas(id))
    {
        std::vector<std::pair<char,int>> visited(nAreas, {0,DANGER_COST_LIMIT});
        area_danger.at(area_id) = dist_to_ennemies(state,area_id,0,area_status,visited).first;
    }

    auto redeploy = state.get_redeployable_areas(id);
    int free = state.get_free_units_number(id);

    std::vector<std::shared_ptr<engine::Command>> command_stack;
    command_stack.reserve(free);

    if (redeploy.empty() || nAreas == 0) return command_stack;

    for (int k = 0; k < free; ++k) {
        int best_cost = DANGER_COST_LIMIT;
        int best_area = -1;
        for(int area_id : redeploy){
            if(area_danger.at(area_id)<=best_cost){
                best_area = area_id;
                best_cost = area_danger.at(area_id);
            }
        }
        if (best_area == -1) break;

        command_stack.emplace_back(std::make_shared<engine::Redeploy_Command>(id, best_area, 1));
        area_danger.at(best_area) += 1;
    }
    return command_stack;
}