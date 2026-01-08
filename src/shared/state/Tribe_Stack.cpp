#include "Tribe_Stack.h"

#include <algorithm>
#include <array>
#include <random>
#include <stdexcept>
#include <vector>

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

    if ((position < 0) | (position > 5))
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
    for (size_t i = 0; i < std::min(static_cast<size_t>(6), stack.size()); ++i)
    {
        seeable_Tribes.push_back(&stack[i]);
    }
    return seeable_Tribes;
}

}  // namespace state