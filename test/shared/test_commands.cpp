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

BOOST_AUTO_TEST_CASE(Test_Command_Creation)
{
    Json::Value choose_species_command_json;
    choose_species_command_json["command_name"] = "Choose_Species_Command";
    Json::Value conquer_command_json;
    conquer_command_json["command_name"] = "Conquer_Command";
    Json::Value decline_command_json;
    decline_command_json["command_name"] = "Decline_Command";
    Json::Value end_conquer_command_json;
    end_conquer_command_json["command_name"] = "End_Conquer_Command";
    Json::Value redeploy_command_json;
    redeploy_command_json["command_name"] = "Redeploy_Command";
    Json::Value start_conquest_command_json;
    start_conquest_command_json["command_name"] = "Start_Conquest_Command";

    std::shared_ptr<engine::Command> choose_species_command =
        engine::Command::create_from_json(choose_species_command_json);

    std::shared_ptr<engine::Command> conquer_command =
        engine::Command::create_from_json(conquer_command_json);

    std::shared_ptr<engine::Command> decline_command =
        engine::Command::create_from_json(decline_command_json);

    std::shared_ptr<engine::Command> end_conquer_command =
        engine::Command::create_from_json(end_conquer_command_json);

    std::shared_ptr<engine::Command> redeploy_command =
        engine::Command::create_from_json(redeploy_command_json);

    std::shared_ptr<engine::Command> start_conquest_command =
        engine::Command::create_from_json(start_conquest_command_json);
}

BOOST_AUTO_TEST_CASE(Test_Conquer_Command)
{
    {
        std::shared_ptr<engine::Command> command =
            std::make_shared<engine::Conquer_Command>(0, 0, 1, true);

        Json::Value command_json;
        command->to_json(command_json);
        command->from_json(command_json);
        std::shared_ptr<engine::Conquer_Command> conquer_command =
            (std::static_pointer_cast<engine::Conquer_Command>(command));
        BOOST_CHECK_EQUAL(conquer_command->get_area_id(), 0);

        state::Game_State state({"Alice", "Bob"}, 0);
        state.take_tribe_at_position(0, 0);
        conquer_command->execute(state);
    }
}

BOOST_AUTO_TEST_CASE(Test_Choose_Species_Command)
{
    {
        std::shared_ptr<engine::Command> command =
            std::make_shared<engine::Choose_Species_Command>(0, 5);

        Json::Value command_json;
        command->to_json(command_json);
        command->from_json(command_json);
    }
}

BOOST_AUTO_TEST_CASE(Test_Decline_Command)
{
    {
        std::shared_ptr<engine::Command> command = std::make_shared<engine::Decline_Command>(0);

        Json::Value command_json;
        command->to_json(command_json);
        command->from_json(command_json);
    }
}

BOOST_AUTO_TEST_CASE(Test_End_Conquer_Command)
{
    {
        std::shared_ptr<engine::Command> command = std::make_shared<engine::End_Conquer_Command>(0);

        Json::Value command_json;
        command->to_json(command_json);
        command->from_json(command_json);
    }
}

BOOST_AUTO_TEST_CASE(Test_Redeploy_Command)
{
    {
        std::shared_ptr<engine::Command> command =
            std::make_shared<engine::Redeploy_Command>(0, 5, 10);

        Json::Value command_json;
        command->to_json(command_json);
        command->from_json(command_json);
    }
}

BOOST_AUTO_TEST_CASE(Test_Start_Conquest_Command)
{
    {
        std::shared_ptr<engine::Command> command =
            std::make_shared<engine::Start_Conquest_Command>(0);

        Json::Value command_json;
        command->to_json(command_json);
        command->from_json(command_json);
    }
}