#include <boost/test/unit_test.hpp>
#include <fstream>
#include <iostream>

#include "engine.h"
#include "renderer.h"
#include "state.h"

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(Test_Engine)
{
    {
        state::Game_State state(2, {"Alice", "Bob"});
        engine::Engine    engine(2, {"Alice", "Bob"});

        // ---------------- error testing ------------------------------

        engine.update();  // empty queue

        BOOST_CHECK_EQUAL(engine.get_state_version_id(), 0);

        engine.add_command(std::make_unique<engine::Start_Conquest_Command>(0));  // no active tribe
        BOOST_CHECK_THROW(engine.update(), std::runtime_error);
        engine.remove_last_command();

        engine.add_command(
            std::make_unique<engine::Choose_Species_Command>(1, 0));  // not the player's turn
        BOOST_CHECK_THROW(engine.update(), std::runtime_error);
        engine.remove_last_command();

        engine.add_command(std::make_unique<engine::Choose_Species_Command>(
            0, 0));  // correct command for taking a tribe
        engine.update();

        engine.add_command(
            std::make_unique<engine::Redeploy_Command>(0, 0, 1));  // not the right turn phase
        BOOST_CHECK_THROW(engine.update(), std::runtime_error);
        engine.remove_last_command();

        // ---------------------- simulate game turns -------------------------

        engine = engine::Engine(2, {"Alice", "Bob"});  // reset engine

        // choose species for player 0

        engine.add_command(
            std::make_unique<engine::Choose_Species_Command>(state.get_current_player().id, 0));
        engine.update();
        state = state::Game_State(engine.get_state());

        // conquer loop for player 0

        while (state.get_current_turn_phase() == state::Turn_Phase::CONQUER)
        {
            std::vector<std::pair<int, int>> prices =
                state.get_conquest_prices(state.get_current_player().id);
            std::vector<std::pair<int, int>> attackable_areas;
            for (const auto& price_info : prices)
            {
                if (price_info.second <= state.get_free_units_number(state.get_current_player().id))
                {
                    attackable_areas.push_back(price_info);
                }
            }

            if (attackable_areas.empty())
            {
                // no more areas can be conquered, end conquest phase
                engine.add_command(
                    std::make_unique<engine::End_Conquer_Command>(state.get_current_player().id));
                engine.update();
                state = state::Game_State(engine.get_state());
                break;
            }

            std::pair<int, int> area_to_attack =
                attackable_areas[std::rand() % attackable_areas.size()];

            engine.add_command(std::make_unique<engine::Conquer_Command>(
                state.get_current_player().id, area_to_attack.first, area_to_attack.second, -1));
            engine.update();
            state = state::Game_State(engine.get_state());
        }

        // redeploy loop for player 0
        while (state.get_current_turn_phase() == state::Turn_Phase::REDEPLOY)
        {
            int              player_id          = state.get_current_player().id;
            std::vector<int> redeployable_areas = state.get_redeployable_areas(player_id);

            if (redeployable_areas.empty())
            {
                throw std::runtime_error("No redeployable areas available for this player.");
            }

            engine.add_command(
                std::make_unique<engine::Redeploy_Command>(player_id, redeployable_areas.at(0), 1));

            engine.update();
            state = state::Game_State(engine.get_state());
        }

        // choose species for player 1

        engine.add_command(
            std::make_unique<engine::Choose_Species_Command>(state.get_current_player().id, 0));
        engine.update();
        state = state::Game_State(engine.get_state());

        // conquer 1 area for player 1

        std::vector<std::pair<int, int>> prices =
            state.get_conquest_prices(state.get_current_player().id);
        std::vector<std::pair<int, int>> attackable_areas;
        for (const auto& price_info : prices)
        {
            if (price_info.second <= state.get_free_units_number(state.get_current_player().id))
            {
                attackable_areas.push_back(price_info);
            }
        }

        std::pair<int, int> area_to_attack =
            attackable_areas[std::rand() % attackable_areas.size()];

        engine.add_command(std::make_unique<engine::Conquer_Command>(
            state.get_current_player().id, area_to_attack.first, area_to_attack.second, -1));
        engine.update();
        state = state::Game_State(engine.get_state());

        // end conquest

        engine.add_command(
            std::make_unique<engine::End_Conquer_Command>(state.get_current_player().id));
        engine.update();
        state = state::Game_State(engine.get_state());

        // redeploy loop for player 1
        while (state.get_current_turn_phase() == state::Turn_Phase::REDEPLOY)
        {
            int              player_id          = state.get_current_player().id;
            std::vector<int> redeployable_areas = state.get_redeployable_areas(player_id);

            if (redeployable_areas.empty())
            {
                throw std::runtime_error("No redeployable areas available for this player.");
            }

            engine.add_command(
                std::make_unique<engine::Redeploy_Command>(player_id, redeployable_areas.at(0), 1));

            engine.update();
            state = state::Game_State(engine.get_state());
        }

        // test the error at turn's beginning
        engine.add_command(
            std::make_unique<engine::Choose_Species_Command>(state.get_current_player().id, 0));
        BOOST_CHECK_THROW(engine.update(), std::runtime_error);
        engine.remove_last_command();

        // go in decline with player 0

        engine.add_command(
            std::make_unique<engine::Decline_Command>(state.get_current_player().id));
        engine.update();
        state = state::Game_State(engine.get_state());

        // start conquests for player 1 (using serial version)

        std::unique_ptr<engine::Command> command =
            std::make_unique<engine::Start_Conquest_Command>(state.get_current_player().id);
        Json::Value command_json;
        command->to_json(command_json);
        engine.add_command(command_json);
        engine.update();
        state = engine.get_state().deep_copy();

        engine.get_state_json();
    }
}
