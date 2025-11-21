#include "Tribe_Stack.h"

#include <algorithm>
#include <array>
#include <random>
#include <stdexcept>
#include <vector>

#include "Dwarves_Species.h"
#include "Power_Description.h"

#define number_of_species 14
#define number_of_powers 20
#define seeable_stack_number 6

namespace state
{

Tribe_Stack::Tribe_Stack()
{
    stack.clear();
    stack = stack_builder.get_tribe_stack();
}

Tribe* Tribe_Stack::take_tribe_at_position(int position)
{
    if (stack.empty())
    {
        throw std::out_of_range("Tribe Stack: No tribes left in the stack");
    }

    if (position >= static_cast<int>(stack.size()) || position < 0 ||
        position >= seeable_stack_number)
    {
        throw std::out_of_range("Tribe Stack: Invalid tribe position");
    }
    Tribe tribe_copy = stack[position];
    stack.erase(stack.begin() + position);
    Tribe* tribe_out = new Tribe(tribe_copy);
    return tribe_out;
}

std::vector<Tribe*> Tribe_Stack::get_tribes_on_top()
{
    std::vector<Tribe*> seeable_Tribes;
    for (size_t i = 0; i < std::min(static_cast<size_t>(seeable_stack_number), stack.size()); ++i)
    {
        seeable_Tribes.push_back(&stack[i]);
    }
    return seeable_Tribes;
}

}  // namespace state