#include "Tribe_Stack.h"

#include <algorithm>
#include <array>
#include <random>
#include <stdexcept>
#include <vector>

namespace state
{

Tribe_Stack::Tribe_Stack(int n_players)
{
    stack.clear();
    stack.reserve(6);
    for (int i = 0; i < 6; ++i)
    {
        stack.push_back(stack_builder.get_next_tribe());
    }
    in_game_tribes.reserve(n_players * 2);  // each player can have up to 2 tribes in game
}

Tribe* Tribe_Stack::take_tribe_at_position(int position)
{
    if (stack.empty())
    {
        throw std::out_of_range("Tribe Stack: No tribes left in the stack");
    }

    if ((position < 0) | (position > 5))
    {
        throw std::out_of_range("Tribe Stack: Invalid tribe position");
    }

    in_game_tribes.push_back(stack.at(position));

    stack.erase(stack.begin() + position);
    stack.push_back(stack_builder.get_next_tribe());
    return &in_game_tribes.back();
}

std::vector<Tribe*> Tribe_Stack::get_tribes_on_top()
{
    std::vector<Tribe*> seeable_Tribes;
    for (size_t i = 0; i < std::min(static_cast<size_t>(6), stack.size()); ++i)
    {
        seeable_Tribes.push_back(&stack.at(i));
    }
    return seeable_Tribes;
}

void Tribe_Stack::remove_from_in_game_tribes(int tribe_id)
{
    for (int i = 0; i < in_game_tribes.size(); i++)
    {
        if (in_game_tribes.at(i).id == tribe_id)
        {
            stack_builder.return_tribe(&in_game_tribes.at(i));
            in_game_tribes.erase(in_game_tribes.begin() + i);
            return;
        }
    }
}

Tribe_Stack Tribe_Stack::deep_copy()
{
    Tribe_Stack copy(in_game_tribes.capacity() / 2);
    copy.stack_builder = stack_builder.deep_copy();

    for (auto& tribe : in_game_tribes)
    {
        copy.in_game_tribes.push_back(tribe.deep_copy());  // first making a shallow copy

        // restoring species_description and power_description pointers for each tribe
        for (auto& power : copy.stack_builder.get_powers())
        {
            if (power->get_name() == tribe.get_power_name())
            {
                copy.in_game_tribes.back().set_power_description(power.get());
                break;
            }
        }

        for (auto& species : copy.stack_builder.get_species())
        {
            if (species->get_name() == tribe.get_species_name())
            {
                copy.in_game_tribes.back().set_species_description(species.get());
                break;
            }
        }
    }

    copy.stack.clear();
    for (auto& tribe : stack)
    {
        copy.stack.push_back(tribe.deep_copy());  // first making a shallow copy

        // restoring species_description and power_description pointers for each tribe
        for (auto& power : copy.stack_builder.get_powers())
        {
            if (power->get_name() == tribe.get_power_name())
            {
                copy.stack.back().set_power_description(power.get());
                break;
            }
        }

        for (auto& species : copy.stack_builder.get_species())
        {
            if (species->get_name() == tribe.get_species_name())
            {
                copy.stack.back().set_species_description(species.get());
                break;
            }
        }
    }

    return copy;
}

std::vector<Tribe*> Tribe_Stack::get_in_game_tribes()
{
    std::vector<Tribe*> tribes_ptrs;
    for (auto& tribe : in_game_tribes)
    {
        tribes_ptrs.push_back(&tribe);
    }
    return tribes_ptrs;
}

void Tribe_Stack::to_json(Json::Value& root)
{
    Json::Value stack_builder_json;
    stack_builder.to_json(stack_builder_json);
    Json::Value stack_json(Json::objectValue);
    for (Tribe& tribe : stack)
    {
        Json::Value tribe_json;
        tribe.to_json(tribe_json);
        stack_json[std::to_string(tribe.id)] = tribe_json;
    }
    root["stack"] = stack_json;

    Json::Value in_game_tribes_json(Json::objectValue);
    for (auto& tribe : in_game_tribes)
    {
        Json::Value tribe_json;
        tribe.to_json(tribe_json);
        in_game_tribes_json[std::to_string(tribe.id)] = tribe_json;
    }
    root["in_game_tribes"] = in_game_tribes_json;
}

void Tribe_Stack::from_json(Json::Value& root)
{
    Json::Value stack_builder_json = root["stack_builder"];
    stack_builder.from_json(stack_builder_json);

    stack.clear();
    for (const auto& tribe_json : root["stack"])
    {
        stack.emplace_back(0, new effects::Species_Description("temp", 0, 0),
                           new effects::Power_Description("temp", 0));
        stack.back().from_json(const_cast<Json::Value&>(tribe_json));
        // restore species descriptions and power description
        for (auto& species : stack_builder.get_species())
        {
            if (species->get_name() == tribe_json["species_description_name"].asString())
            {
                stack.back().set_species_description(species.get());
                break;
            }
        }
        for (auto& power : stack_builder.get_powers())
        {
            if (power->get_name() == tribe_json["power_description_name"].asString())
            {
                stack.back().set_power_description(power.get());
                break;
            }
        }
    }

    in_game_tribes.clear();
    for (const auto& tribe_json : root["in_game_tribes"])
    {
        in_game_tribes.emplace_back(0, new effects::Species_Description("temp", 0, 0),
                                    new effects::Power_Description("temp", 0));
        in_game_tribes.back().from_json(const_cast<Json::Value&>(tribe_json));
        // restore species descriptions and power description
        for (auto& species : stack_builder.get_species())
        {
            if (species->get_name() == tribe_json["species_description_name"].asString())
            {
                in_game_tribes.back().set_species_description(species.get());
                break;
            }
        }

        for (auto& power : stack_builder.get_powers())
        {
            if (power->get_name() == tribe_json["power_description_name"].asString())
            {
                in_game_tribes.back().set_power_description(power.get());
                break;
            }
        }
    }
}

}  // namespace state