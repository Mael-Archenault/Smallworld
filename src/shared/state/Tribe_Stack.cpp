#include "Tribe_Stack.h"
#include <array>
#include <random>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include "Dwarves_Species.h"
#include "Power_Description.h"

#define number_of_species 14
#define number_of_powers 20
#define seeable_stack_number 6


namespace state {

Tribe_Stack::Tribe_Stack() {
    stack.clear();

    std::vector<Species_Description*> species_lists;
    static Dwarves_Species Dwarf_effect;
    static Species_Description Amazons("Amazons",6,8);
    species_lists.push_back(&Amazons);
    static Species_Description Dwarves("Dwarves",3,8);
    species_lists.push_back(&Dwarves);
    static Species_Description Elves("Elves",6,8);
    species_lists.push_back(&Elves);
    static Species_Description Ghouls("Ghouls",5,8);
    species_lists.push_back(&Ghouls);
    static Species_Description Giants("Giants",6,8);
    species_lists.push_back(&Giants);
    static Species_Description Halflings("Halflings",6,8);
    species_lists.push_back(&Halflings);
    static Species_Description Humans("Humans",5,8);
    species_lists.push_back(&Humans);
    static Species_Description Orcs("Orcs",5,8);
    species_lists.push_back(&Orcs);
    static Species_Description Ratmen("Ratmen",8,8);
    species_lists.push_back(&Ratmen);
    static Species_Description Skeletons("Skeletons",6,8);
    species_lists.push_back(&Skeletons);
    static Species_Description Sorcerers("Sorcerers",5,8);
    species_lists.push_back(&Sorcerers);
    static Species_Description Tritons("Tritons",6,8);
    species_lists.push_back(&Tritons);
    static Species_Description Trolls("Trolls",5,8);
    species_lists.push_back(&Trolls);
    static Species_Description Wizards("Wizards",5,8);
    species_lists.push_back(&Wizards);

    //list of Power
    std::vector<Power_Description*> power_lists;
    static Power_Description Alchemist("Alchemist",4);
    power_lists.push_back(&Alchemist);
    static Power_Description Berserk("Berserk",4);
    power_lists.push_back(&Berserk);
    static Power_Description Bivouacking("Bivouacking",5);
    power_lists.push_back(&Bivouacking);
    static Power_Description Commando("Commando",4);
    power_lists.push_back(&Commando);
    static Power_Description Diplomat("Diplomat",5);
    power_lists.push_back(&Diplomat);
    static Power_Description Dragon_Master("Dragon Master",5);
    power_lists.push_back(&Dragon_Master);
    static Power_Description Fiying("Flying",5);
    power_lists.push_back(&Fiying);
    static Power_Description Forest("Forest",4);
    power_lists.push_back(&Forest);
    static Power_Description Fortified("Fortified",3);
    power_lists.push_back(&Fortified);
    static Power_Description Heroic("Heroic",5);
    power_lists.push_back(&Heroic);
    static Power_Description Hill("Hill",4);
    power_lists.push_back(&Hill);
    static Power_Description Merchant("Merchant",2);
    power_lists.push_back(&Merchant);
    static Power_Description Mounted("Mounted",5);
    power_lists.push_back(&Mounted);
    static Power_Description Pillaging("Pillaging",5);
    power_lists.push_back(&Pillaging);
    static Power_Description Seafaring("Seafaring",5);
    power_lists.push_back(&Seafaring);
    static Power_Description Spirit("Spirit",5);
    power_lists.push_back(&Spirit);
    static Power_Description Stout("Stout",4);
    power_lists.push_back(&Stout);
    static Power_Description Swamp("Swamp",4);
    power_lists.push_back(&Swamp);
    static Power_Description Underworld("Underworld",5);
    power_lists.push_back(&Underworld);
    static Power_Description Wealthy("Wealthy",4);
    power_lists.push_back(&Wealthy);

    // build of the stack: pair each species with a unique power (no duplicates)
    std::random_device rd;
    std::mt19937 rng(rd());

    // shuffle species and powers independently
    std::shuffle(species_lists.begin(), species_lists.end(), rng);
    std::shuffle(power_lists.begin(), power_lists.end(), rng);

    // Pair the first 'number_of_species' species with the first same number of powers
    for (size_t i=0; i < number_of_species; i++) {
        Tribe tribe(i, species_lists[i], power_lists[i]);
        stack.push_back(tribe);
    }
}

Tribe* Tribe_Stack::take_tribe_at_position(int position){
    if (stack.empty()) {
        throw std::out_of_range("Tribe Stack: No tribes left in the stack");
    }

    if(position>=static_cast<int>(stack.size()) || position<0 || position>=seeable_stack_number){
        throw std::out_of_range("Tribe Stack: Invalid tribe position");
    }
    Tribe tribe_copy = stack[position];
    stack.erase(stack.begin() + position);
    Tribe* tribe_out = new Tribe(tribe_copy);
    return tribe_out;
}

std::vector<Tribe*> Tribe_Stack::get_tribes_on_top(){
    std::vector<Tribe*> seeable_Tribes;
    for(size_t i = 0; i < std::min(static_cast<size_t>(seeable_stack_number), stack.size()); ++i){
        seeable_Tribes.push_back(&stack[i]);
    }
    return seeable_Tribes;
}

}