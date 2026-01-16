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
    std::unique_ptr<engine::Command> choose_species_command =
        engine::Command::create_from_json(choose_species_command_json);

    Json::Value conquer_command_json;
    conquer_command_json["command_name"] = "Conquer_Command";
    std::unique_ptr<engine::Command> conquer_command =
        engine::Command::create_from_json(conquer_command_json);

    Json::Value decline_command_json;
    decline_command_json["command_name"] = "Decline_Command";
    std::unique_ptr<engine::Command> decline_command =
        engine::Command::create_from_json(decline_command_json);

    Json::Value end_conquer_command_json;
    end_conquer_command_json["command_name"] = "End_Conquer_Command";
    std::unique_ptr<engine::Command> end_conquer_command =
        engine::Command::create_from_json(end_conquer_command_json);

    Json::Value redeploy_command_json;
    redeploy_command_json["command_name"] = "Redeploy_Command";
    std::unique_ptr<engine::Command> redeploy_command =
        engine::Command::create_from_json(redeploy_command_json);

    Json::Value start_conquest_command_json;
    start_conquest_command_json["command_name"] = "Start_Conquest_Command";
    std::unique_ptr<engine::Command> start_conquest_command =
        engine::Command::create_from_json(start_conquest_command_json);
}

BOOST_AUTO_TEST_CASE(Test_Conquer_Command)
{
    {
        std::unique_ptr<engine::Command> command =
            std::make_unique<engine::Conquer_Command>(0, 5, 10, -1);

        Json::Value command_json;
        command->to_json(command_json);
        command->from_json(command_json);
    }
}

BOOST_AUTO_TEST_CASE(Test_Choose_Species_Command)
{
    {
        std::unique_ptr<engine::Command> command =
            std::make_unique<engine::Choose_Species_Command>(0, 5);

        Json::Value command_json;
        command->to_json(command_json);
        command->from_json(command_json);
    }
}

BOOST_AUTO_TEST_CASE(Test_Decline_Command)
{
    {
        std::unique_ptr<engine::Command> command = std::make_unique<engine::Decline_Command>(0);

        Json::Value command_json;
        command->to_json(command_json);
        command->from_json(command_json);
    }
}

BOOST_AUTO_TEST_CASE(Test_End_Conquer_Command)
{
    {
        std::unique_ptr<engine::Command> command = std::make_unique<engine::End_Conquer_Command>(0);

        Json::Value command_json;
        command->to_json(command_json);
        command->from_json(command_json);
    }
}

BOOST_AUTO_TEST_CASE(Test_Redeploy_Command)
{
    {
        std::unique_ptr<engine::Command> command =
            std::make_unique<engine::Redeploy_Command>(0, 5, 10);

        Json::Value command_json;
        command->to_json(command_json);
        command->from_json(command_json);
    }
}

BOOST_AUTO_TEST_CASE(Test_Start_Conquest_Command)
{
    {
        std::unique_ptr<engine::Command> command =
            std::make_unique<engine::Start_Conquest_Command>(0);

        Json::Value command_json;
        command->to_json(command_json);
        command->from_json(command_json);
    }
}