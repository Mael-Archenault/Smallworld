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

#include "engine/Choose_Species_Command.h"

using namespace ai;

Ai_Advanced::Ai_Advanced(state::Game_State* state,int player_id) : Ai_Interface(state, player_id), engine(state, player_id) {
    command_queue = {};
}

std::unique_ptr<engine::Command> Ai_Advanced::give_command (state::Turn_Phase phase) {
    if (command_queue.empty()) {
        command_queue = calcul_queue();
    }
    auto ret = std::move(command_queue.front());
    command_queue.pop();
    return ret;
}


std::queue<std::unique_ptr<engine::Command>> Ai_Advanced::calcul_queue() {

    for (std::pair<int,int> area : state->get_conquest_prices(id)) {
        int required_units = area.second;
        int available_units = state->get_free_units_number(id);
        std::make_unique<engine::Conquer_Command>(id,area.first,
                                                    std::min(required_units,available_units),
                                                    (required_units - available_units > 0));
    }

}
