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

    Tribe* taken_tribe = new Tribe(stack.at(position));
    in_game_tribes.push_back(taken_tribe);

    stack.erase(stack.begin() + position);
    stack.push_back(stack_builder.get_next_tribe());
    return taken_tribe;
}

std::vector<Tribe*> Tribe_Stack::get_tribes_on_top()
{
    std::vector<Tribe*> seeable_Tribes;
    for (size_t i = 0; i < std::min(static_cast<size_t>(6), stack.size()); ++i)
    {
        seeable_Tribes.push_back(&stack[i]);
    }
    return seeable_Tribes;
}

void Tribe_Stack::remove_from_in_game_tribes(int tribe_id)
{
    for (int i = 0; i < in_game_tribes.size(); i++)
    {
        if (in_game_tribes.at(i)->id == tribe_id)
        {
            stack_builder.return_tribe(in_game_tribes.at(i));
            free(in_game_tribes.at(i));
            in_game_tribes.erase(in_game_tribes.begin() + i);
            return;
        }
    }
}

Tribe_Stack Tribe_Stack::deep_copy()
{
    Tribe_Stack copy(in_game_tribes.capacity() / 2);
    copy.stack_builder = stack_builder.deep_copy();
    for (Tribe* tribe : in_game_tribes)
    {
        Tribe* tribe_copy =
            new Tribe(tribe->id, tribe->get_species_description(), tribe->get_power_description());
        tribe_copy->set_is_in_decline(tribe->get_is_in_decline());
        tribe_copy->set_free_units_number(tribe->get_free_units_number());

        // restoring species_description and power_description pointers
        for (auto power : copy.stack_builder.get_powers())
        {
            if (power->get_name() == tribe->get_power_name())
            {
                tribe_copy->set_power_description(power);
                break;
            }
        }

        for (auto species : copy.stack_builder.get_species())
        {
            if (species->get_name() == tribe->get_species_name())
            {
                tribe_copy->set_species_description(species);
                break;
            }
        }
        copy.in_game_tribes.push_back(tribe_copy);
    }

    copy.stack.clear();
    for (Tribe tribe : stack)
    {
        // restoring species_description and power_description pointers
        Tribe tribe_copy(tribe.id, tribe.get_species_description(), tribe.get_power_description());
        for (auto power : stack_builder.get_powers())
        {
            if (power->get_name() == tribe.get_power_description()->get_name())
            {
                tribe_copy.set_power_description(power);
                break;
            }
        }

        for (auto species : stack_builder.get_species())
        {
            if (species->get_name() == tribe.get_species_description()->get_name())
            {
                tribe_copy.set_species_description(species);
                break;
            }
        }
        copy.stack.push_back(tribe_copy);
    }

    return copy;
}

std::vector<Tribe*> Tribe_Stack::get_in_game_tribes()
{
    return in_game_tribes;
}

}  // namespace state