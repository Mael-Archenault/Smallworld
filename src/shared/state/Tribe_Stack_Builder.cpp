#include "Tribe_Stack_Builder.h"

#include <json/json.h>

#include <fstream>
#include <functional>
#include <random>
#include <unordered_map>

#include "resources_dir.h"
#include "state.h"

namespace state
{
std::unordered_map<std::string, std::function<Species_Description()>> str_to_species = {
    {"Amazons", []() { return Dwarves_Species(); }},
    {"Dwarves", []() { return Dwarves_Species(); }},
    {"Elves", []() { return Dwarves_Species(); }},
    {"Ghouls", []() { return Dwarves_Species(); }},
    {"Giants", []() { return Giants_Species(); }},
    {"Halflings", []() { return Halflings_Species(); }},
    {"Humans", []() { return Humans_Species(); }},
    {"Orcs", []() { return Dwarves_Species(); }},
    {"Ratmen", []() { return Ratmen_Species(); }},
    {"Skeletons", []() { return Skeletons_Species(); }},
    {"Sorcerers", []() { return Dwarves_Species(); }},
    {"Tritons", []() { return Tritons_Species(); }},
    {"Trolls", []() { return Trolls_Species(); }},
    {"Wizards", []() { return Wizards_Species(); }}};

std::unordered_map<std::string, std::function<Power_Description()>> str_to_power = {
    {"Alchemist", []() { return Power_Description("Alchemist", 4); }},
    {"Berserk", []() { return Power_Description("Berserk", 4); }},
    {"Bivouacking", []() { return Power_Description("Bivouacking", 5); }},
    {"Commando", []() { return Power_Description("Commando", 4); }},
    {"Heroic", []() { return Power_Description("Heroic", 5); }},
    {"Hill", []() { return Power_Description("Hill", 4); }},
    {"Merchant", []() { return Power_Description("Merchant", 2); }},
    {"Mounted", []() { return Power_Description("Mounted", 5); }},
    {"Pillaging", []() { return Power_Description("Pillaging", 5); }},
    {"Seafaring", []() { return Power_Description("Seafaring", 5); }},
    {"Spirit", []() { return Power_Description("Spirit", 5); }},
    {"Stout", []() { return Power_Description("Stout", 4); }},
    {"Swamp", []() { return Power_Description("Swamp", 4); }},
    {"Underworld", []() { return Power_Description("Underworld", 5); }},
    {"Wealthy", []() { return Power_Description("Wealthy", 4); }},
    {"Diplomat", []() { return Power_Description("Diplomat", 5); }},
    {"Dragon Master", []() { return Power_Description("Dragon Master", 5); }},
    {"Flying", []() { return Power_Description("Flying", 5); }},
    {"Forest", []() { return Power_Description("Forest", 4); }},
    {"Fortified", []() { return Power_Description("Fortified", 3); }},
    {"Hill", []() { return Power_Description("Hill", 4); }}};

Tribe_Stack_Builder::Tribe_Stack_Builder()
{
    load_species_and_powers();
}

std::vector<Tribe> Tribe_Stack_Builder::get_tribe_stack()
{
    std::vector<Tribe> stack;
    // build of the stack: pair each species with a unique power (no duplicates)
    std::random_device rd;
    std::mt19937       rng(rd());

    // shuffle species and powers independently
    std::shuffle(species.begin(), species.end(), rng);
    std::shuffle(powers.begin(), powers.end(), rng);

    // Pair the first 'number_of_species' species with the first same number of powers
    for (size_t i = 0; i < species.size(); i++)
    {
        Tribe tribe(i, &species.at(i), &powers.at(i));
        stack.push_back(tribe);
    }
    return stack;
}

void Tribe_Stack_Builder::load_species_and_powers()
{
    std::string   file_name = std::string(RESOURCE_DIR) + "/config/effects_catalog.json";
    std::ifstream file(file_name, std::ifstream::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file: " + file_name);
    }

    Json::Value root;
    file >> root;
    file.close();

    for (const auto& species_name : root["Species"])
    {
        Species_Description species = str_to_species[species_name.asString()]();
        this->species.push_back(species);
    }
    for (const auto& power_name : root["Powers"])
    {
        Power_Description power = str_to_power[power_name.asString()]();
        this->powers.push_back(power);
    }
}
}  // namespace state