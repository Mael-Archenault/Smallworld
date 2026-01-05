#include "Tribe_Stack_Builder.h"

#include <json/json.h>

#include <fstream>
#include <functional>
#include <random>
#include <unordered_map>

#include "effects.h"
#include "resources_dir.h"
#include "state.h"

namespace state
{
std::unordered_map<std::string, std::function<effects::Species_Description*()>> str_to_species = {
    {"Amazons", []() { return new effects::Amazons_Species(); }},
    {"Dwarves", []() { return new effects::Dwarves_Species(); }},
    {"Elves", []() { return new effects::Elves_Species(); }},
    {"Ghouls", []() { return new effects::Dwarves_Species(); }},
    {"Giants", []() { return new effects::Giants_Species(); }},
    {"Halflings", []() { return new effects::Halflings_Species(); }},
    {"Humans", []() { return new effects::Humans_Species(); }},
    {"Orcs", []() { return new effects::Orcs_Species(); }},
    {"Ratmen", []() { return new effects::Ratmen_Species(); }},
    {"Skeletons", []() { return new effects::Skeletons_Species(); }},
    {"Sorcerers", []() { return new effects::Dwarves_Species(); }},
    {"Tritons", []() { return new effects::Tritons_Species(); }},
    {"Trolls", []() { return new effects::Trolls_Species(); }},
    {"Wizards", []() { return new effects::Wizards_Species(); }}};

std::unordered_map<std::string, std::function<effects::Power_Description*()>> str_to_power = {
    {"Alchemist", []() { return new effects::Power_Description("Alchemist", 4); }},
    {"Berserk", []() { return new effects::Power_Description("Berserk", 4); }},
    {"Bivouacking", []() { return new effects::Power_Description("Bivouacking", 5); }},
    {"Commando", []() { return new effects::Power_Description("Commando", 4); }},
    {"Heroic", []() { return new effects::Power_Description("Heroic", 5); }},
    {"Hill", []() { return new effects::Power_Description("Hill", 4); }},
    {"Merchant", []() { return new effects::Power_Description("Merchant", 2); }},
    {"Mounted", []() { return new effects::Power_Description("Mounted", 5); }},
    {"Pillaging", []() { return new effects::Power_Description("Pillaging", 5); }},
    {"Seafaring", []() { return new effects::Power_Description("Seafaring", 5); }},
    {"Spirit", []() { return new effects::Power_Description("Spirit", 5); }},
    {"Stout", []() { return new effects::Power_Description("Stout", 4); }},
    {"Swamp", []() { return new effects::Power_Description("Swamp", 4); }},
    {"Underworld", []() { return new effects::Power_Description("Underworld", 5); }},
    {"Wealthy", []() { return new effects::Power_Description("Wealthy", 4); }},
    {"Diplomat", []() { return new effects::Power_Description("Diplomat", 5); }},
    {"Dragon Master", []() { return new effects::Power_Description("Dragon Master", 5); }},
    {"Flying", []() { return new effects::Power_Description("Flying", 5); }},
    {"Forest", []() { return new effects::Power_Description("Forest", 4); }},
    {"Fortified", []() { return new effects::Power_Description("Fortified", 3); }}};

Tribe_Stack_Builder::Tribe_Stack_Builder()
{
    load_species_and_powers();

    available_species = species;
    available_powers  = powers;

    // build of the stack: pair each species with a unique power (no duplicates)
    std::random_device rd;
    std::mt19937       rng(rd());

    // shuffle species and powers independently
    std::shuffle(available_species.begin(), available_species.end(), rng);
    std::shuffle(available_powers.begin(), available_powers.end(), rng);
}

Tribe Tribe_Stack_Builder::get_next_tribe()
{
    std::vector<Tribe> stack;

    if (available_species.empty() || available_powers.empty())
    {
        throw std::out_of_range("No more tribes available in the stack builder");
    }

    Tribe result(n_created_tribes, available_species.at(0), available_powers.at(0));
    available_species.erase(available_species.begin());
    available_powers.erase(available_powers.begin());
    n_created_tribes += 1;
    return result;
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
        effects::Species_Description* s = str_to_species[species_name.asString()]();
        this->species.push_back(s);
    }
    for (const auto& power_name : root["Powers"])
    {
        effects::Power_Description* p = str_to_power[power_name.asString()]();
        this->powers.push_back(p);
    }
}

void Tribe_Stack_Builder::return_tribe(Tribe* returned_tribe)
{
    available_species.push_back(returned_tribe->get_species_description());
    available_powers.push_back(returned_tribe->get_power_description());
}

}  // namespace state