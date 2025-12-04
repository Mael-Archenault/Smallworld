//
// Created by julien on 11/20/25.
//

#include <memory>
#include <stdexcept>
#include <engine/Conquer_Command.h>
#include <engine/Decline_Command.h>
#include <engine/End_Conquer_Command.h>
#include <engine/Redeploy_Command.h>
#include <engine/Start_Conquest_Command.h>

#include "random"
#include "Ai_Interface.h"

#include "engine/Choose_Species_Command.h"

ai::Ai_Interface::Ai_Interface(state::Game_State* state,int player_id) : id(player_id), state(state){
}

std::unique_ptr<engine::Command> ai::Ai_Interface::give_command(state::Turn_Phase phase)
{
    switch (phase)
    {
        case state::Turn_Phase::START:
            return give_command_Start ();
        case state::Turn_Phase::CONQUER:
            return give_command_Conquer ();
        case state::Turn_Phase::REDEPLOY:
            return give_command_Redeploy ();
        default: throw std::runtime_error ("Unknown turn phase");
    }
}

std::unique_ptr<engine::Command> ai::Ai_Interface::give_command_Start ()
{
    return nullptr;
}

std::unique_ptr<engine::Command> ai::Ai_Interface::give_command_Conquer ()
{
    return nullptr;
}


std::unique_ptr<engine::Command> ai::Ai_Interface::give_command_Redeploy ()
{
    return nullptr;
}

void ai::Ai_Interface::update_state(state::Game_State * new_state) {
    this->state = new_state;
}
