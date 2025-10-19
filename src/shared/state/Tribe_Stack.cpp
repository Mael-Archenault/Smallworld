#include "Tribe_Stack.h"
#include <array>
#include <random>
#include <vector>
#include <stdexcept>
#include "Dwarf_Effects_Bundle.h"

#define number_of_species 14
#define number_of_powers 20
#define seeable_stack_number 6


namespace state {

Tribe_Stack::Tribe_Stack() {
    stack = {};

    // list of Species
    static std::vector<Species_Description*> species_lists;
    static Dwarf_Effects_Bundle Dwarf_effect;
    static Species_Description Amazons("Amazons",6,8,Dwarf_effect);
    species_lists.push_back(&Amazons);
    static Species_Description Dwarves("Dwarves",3,8,Dwarf_effect);
    species_lists.push_back(&Dwarves);
    static Species_Description Elves("Elves",6,8,Dwarf_effect);
    species_lists.push_back(&Elves);
    static Species_Description Ghouls("Ghouls",5,8,Dwarf_effect);
    species_lists.push_back(&Ghouls);
    static Species_Description Giants("Giants",6,8,Dwarf_effect);
    species_lists.push_back(&Giants);
    static Species_Description Halflings("Halflings",6,8,Dwarf_effect);
    species_lists.push_back(&Halflings);
    static Species_Description Humans("Humans",5,8,Dwarf_effect);
    species_lists.push_back(&Humans);
    static Species_Description Orcs("Orcs",5,8,Dwarf_effect);
    species_lists.push_back(&Orcs);
    static Species_Description Ratmen("Ratmen",8,8,Dwarf_effect);
    species_lists.push_back(&Ratmen);
    static Species_Description Skeletons("Skeletons",6,8,Dwarf_effect);
    species_lists.push_back(&Skeletons);
    static Species_Description Sorcerers("Sorcerers",5,8,Dwarf_effect);
    species_lists.push_back(&Sorcerers);
    static Species_Description Tritons("Tritons",6,8,Dwarf_effect);
    species_lists.push_back(&Tritons);
    static Species_Description Trolls("Trolls",5,8,Dwarf_effect);
    species_lists.push_back(&Trolls);
    static Species_Description Wizards("Wizards",5,8,Dwarf_effect);
    species_lists.push_back(&Wizards);

    //list of Power
    static std::vector<Power_Description*> power_lists;
    static Power_Description Alchemist("Alchemist",4,Dwarf_effect);
    power_lists.push_back(&Alchemist);
    static Power_Description Berserk("Berserk",4,Dwarf_effect);
    power_lists.push_back(&Berserk);
    static Power_Description Bivouacking("Bivouacking",5,Dwarf_effect);
    power_lists.push_back(&Bivouacking);
    static Power_Description Commando("Commando",4,Dwarf_effect);
    power_lists.push_back(&Commando);
    static Power_Description Diplomat("Diplomat",5,Dwarf_effect);
    power_lists.push_back(&Diplomat);
    static Power_Description Dragon_Master("Dragon_Master",5,Dwarf_effect);
    power_lists.push_back(&Dragon_Master);
    static Power_Description Fiying("Fiying",5,Dwarf_effect);
    power_lists.push_back(&Fiying);
    static Power_Description Forest("Forest",4,Dwarf_effect);
    power_lists.push_back(&Forest);
    static Power_Description Fortified("Fortified",3,Dwarf_effect);
    power_lists.push_back(&Fortified);
    static Power_Description Heroic("Heroic",5,Dwarf_effect);
    power_lists.push_back(&Heroic);
    static Power_Description Hill("Hill",4,Dwarf_effect);
    power_lists.push_back(&Hill);
    static Power_Description Merchant("Merchant",2,Dwarf_effect);
    power_lists.push_back(&Merchant);
    static Power_Description Mounted("Mounted",5,Dwarf_effect);
    power_lists.push_back(&Mounted);
    static Power_Description Pillaging("Pillaging",5,Dwarf_effect);
    power_lists.push_back(&Pillaging);
    static Power_Description Seafaring("Seafaring",5,Dwarf_effect);
    power_lists.push_back(&Seafaring);
    static Power_Description Spirit("Spirit",5,Dwarf_effect);
    power_lists.push_back(&Spirit);
    static Power_Description Stout("Stout",4,Dwarf_effect);
    power_lists.push_back(&Stout);
    static Power_Description Swamp("Swamp",4,Dwarf_effect);
    power_lists.push_back(&Swamp);
    static Power_Description Underworld("Underworld",5,Dwarf_effect);
    power_lists.push_back(&Underworld);
    static Power_Description Wealthy("Wealthy",4,Dwarf_effect);
    power_lists.push_back(&Wealthy);

    //build of the stack
    Species_Description* switch_species = nullptr;
    Power_Description*   switch_power  = nullptr;
    std::random_device rd_species;
    std::random_device rd_power;
    int random_value_species = 0;
    int random_value_power = 0;
    for(int species_to_stack=number_of_species;species_to_stack>0;species_to_stack--){
        random_value_species = rd_species() % number_of_species;
        random_value_power = rd_power() % number_of_powers;
        int id = number_of_species - species_to_stack;
        Tribe tribe(id,species_lists[random_value_species], power_lists[random_value_power]);
        switch_species = species_lists[random_value_species];
        switch_power = power_lists[random_value_power];
        species_lists[random_value_species] = species_lists[species_to_stack];
        power_lists[random_value_species] = power_lists[species_to_stack];
        stack.push_back(&tribe);
    }
}

Tribe* Tribe_Stack::take_tribe_at_position(int position){
    if(position>seeable_stack_number){
        throw std::out_of_range("Invalid tribe position");
    }
    Tribe* tribe_out = stack[position];
    stack.erase(stack.begin() + position);
    return tribe_out;
}

std::vector<Tribe*> Tribe_Stack::get_tribes_on_top(){
    std::vector<Tribe*> seeable_Tribes;
    for(int i=0;i<=seeable_stack_number;i++){
        seeable_Tribes.push_back(stack[i]);
    }
    return seeable_Tribes;
}

}