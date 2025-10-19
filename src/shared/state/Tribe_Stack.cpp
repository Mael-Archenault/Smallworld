#include "Tribe_Stack.h"

namespace state {

Tribe_Stack::Tribe_Stack()
    {
    Tribe test_tribe(
        0,
      new Species_Description("TestSpecies", 5, 10, Effects_Bundle()),
      new Power_Description("TestPower", 3, Effects_Bundle())
    );
    stack.push_back(std::move(test_tribe));
}


Tribe* Tribe_Stack::take_tribe_at_position(int position) {
    if (position>=0 && position<(int)stack.size()) {
        Tribe* tribe = &stack.at(position);
        stack.erase(stack.begin()+position);
        return tribe;
    }
    return nullptr;
}

std::vector<Tribe*> Tribe_Stack::get_tribes_on_top() {
    return {};
}
}