//
// Created by julien on 11/20/25.
//

#include <memory>
#include <stdexcept>
#include <engine/Conquer_Command.h>
#include <engine/Decline_Command.h>
#include <engine/End_Conquer_Command.h>
#include <engine/Start_Conquest_Command.h>

#include "random"
#include "Ai_Interface.h"

ai::Ai_Interface::Ai_Interface(state::Game_State* state) : state(state) {
}

std::unique_ptr<engine::Command> ai::Ai_Interface::give_command(state::Turn_Phase phase) {
    switch (phase) {
        case state::Turn_Phase::START:
            return give_command_Start ();
        case state::Turn_Phase::CONQUER:
            return give_command_Conquer ();
        case state::Turn_Phase::REDEPLOY:
            return give_command_Redeploy ();
        default: throw std::runtime_error ("Unknown turn phase");
    }
}

std::unique_ptr<engine::Command> ai::Ai_Interface::give_command_Start () {
    long random_command = rand() % 2;
    if (random_command == 0) {
        return std::make_unique<engine::Decline_Command>(id);
    }
    else {
        return std::make_unique<engine::Start_Conquest_Command(id);
    }
}

std::unique_ptr<engine::Command> ai::Ai_Interface::give_command_Conquer () {
    long random_command = rand() % 2;
    if (random_command == 0) {
        auto attackable_area = state->get_conquest_prices(id);
        int random_area_id = attackable_area.at(rand() % attackable_area.size()).first;
        return std::make_unique<engine::Conquer_Command>(id,random_area_id,
                                                         std::min(attackable_area.at(random_area_id).second),),0);
    }
    else {
        return std::make_unique<engine::End_Conquer_Command(id);
    }
}
