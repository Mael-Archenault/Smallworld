#include "Tribe_Stack_Builder.h"

#include <json/json.h>

#include <fstream>
#include <functional>
#include <memory>
#include <random>
#include <unordered_map>

#include "effects.h"
#include "resources_dir.h"
#include "state.h"

namespace state
{
std::unordered_map<std::string, std::function<std::shared_ptr<effects::Species_Description>()>>
    str_to_species = {
        {"Amazons", []() { return std::make_shared<effects::Amazons_Species>(); }},
        {"Dwarves", []() { return std::make_shared<effects::Dwarves_Species>(); }},
        {"Elves", []() { return std::make_shared<effects::Elves_Species>(); }},
        //{"Ghouls", []() { return std::make_shared<effects::Dwarves_Species>(); }},
        {"Giants", []() { return std::make_shared<effects::Giants_Species>(); }},
        {"Halflings", []() { return std::make_shared<effects::Halflings_Species>(); }},
        {"Humans", []() { return std::make_shared<effects::Humans_Species>(); }},
        {"Orcs", []() { return std::make_shared<effects::Orcs_Species>(); }},
        {"Ratmen", []() { return std::make_shared<effects::Ratmen_Species>(); }},
        {"Skeletons", []() { return std::make_shared<effects::Skeletons_Species>(); }},
        //{"Sorcerers", []() { return std::make_shared<effects::Dwarves_Species>(); }},
        {"Tritons", []() { return std::make_shared<effects::Tritons_Species>(); }},
        {"Trolls", []() { return std::make_shared<effects::Trolls_Species>(); }},
        {"Wizards", []() { return std::make_shared<effects::Wizards_Species>(); }}};

std::unordered_map<std::string, std::function<std::shared_ptr<effects::Power_Description>()>>
    str_to_power = {
        {"Alchemist",
         []() { return std::make_shared<effects::Alchemist_Power>(); }},
        //{"Berserk", []() { return std::make_shared<effects::Power_Description>("Berserk", 4); }},
        // {"Bivouacking",
        //  []() { return std::make_shared<effects::Power_Description>("Bivouacking", 5); }},
        {"Commando", []() { return std::make_shared<effects::Commando_Power>(); }},
        //{"Heroic", []() { return std::make_shared<effects::Power_Description>("Heroic", 5); }},
        {"Hill", []() { return std::make_shared<effects::Hill_Power>(); }},
        {"Merchant", []() { return std::make_shared<effects::Merchant_Power>(); }},
        {"Mounted", []() { return std::make_shared<effects::Mounted_Power>(); }},
        {"Pillaging",
         []() { return std::make_shared<effects::Pillaging_Power>(); }},
        {"Seafaring",
         []() { return std::make_shared<effects::Seafaring_Power>(); }},
        //{"Spirit", []() { return std::make_shared<effects::Power_Description>("Spirit", 5); }},
        //{"Stout", []() { return std::make_shared<effects::Power_Description>("Stout", 4); }},
        {"Swamp", []() { return std::make_shared<effects::Swamp_Power>(); }},
        {"Underworld",
         []() { return std::make_shared<effects::Underworld_Power>(); }},
        {"Wealthy", []() { return std::make_shared<effects::Wealthy_Power>(); }},

        //{"Diplomat", []() { return std::make_shared<effects::Power_Description>("Diplomat", 5); }},
        //{"Dragon Master",
         //[]() { return std::make_shared<effects::Power_Description>("Dragon Master", 5); }},
        {"Flying", []() { return std::make_shared<effects::Flying_Power>(); }},
        {"Forest", []() { return std::make_shared<effects::Forest_Power>(); }},
        // {"Fortified",
        //  []() { return std::make_shared<effects::Power_Description>("Fortified", 3); }}
        };

Tribe_Stack_Builder::Tribe_Stack_Builder()
{
    load_species_and_powers();

    for (auto& species_desc : species)
    {
        available_species.push_back(species_desc.get());
    }
    for (auto& power_desc : powers)
    {
        available_powers.push_back(power_desc.get());
    }

    // build of the stack: pair each species with a unique power (no duplicates)
    std::random_device rd;
    std::mt19937       rng(rd());

    // shuffle species and powers independently
    std::shuffle(available_species.begin(), available_species.end(), rng);
    std::shuffle(available_powers.begin(), available_powers.end(), rng);
    n_created_tribes = 0;
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
        this->species.push_back(str_to_species[species_name.asString()]());
    }
    for (const auto& power_name : root["Powers"])
    {
        this->powers.push_back(str_to_power[power_name.asString()]());
    }
}

void Tribe_Stack_Builder::return_tribe(Tribe* returned_tribe)
{
    available_species.push_back(returned_tribe->get_species_description());
    available_powers.push_back(returned_tribe->get_power_description());
}

std::vector<std::shared_ptr<effects::Species_Description>>& Tribe_Stack_Builder::get_species()
{
    return species;
}

std::vector<std::shared_ptr<effects::Power_Description>>& Tribe_Stack_Builder::get_powers()
{
    return powers;
}

Tribe_Stack_Builder Tribe_Stack_Builder::deep_copy()
{
    Tribe_Stack_Builder copy;

    copy.n_created_tribes = n_created_tribes;
    // recreating powers and species in the heap
    for (auto& species_desc : species)
    {
        copy.species.push_back(str_to_species[species_desc->get_name()]());
    }
    for (auto& power_desc : powers)
    {
        copy.powers.push_back(str_to_power[power_desc->get_name()]());
    }

    for (auto available_species_desc : available_species)
    {
        for (auto& species_copy : copy.species)
        {
            if (available_species_desc->get_name() == species_copy->get_name())
            {
                copy.available_species.push_back(species_copy.get());
                break;
            }
        }
    }

    for (auto available_power_desc : available_powers)
    {
        for (auto& power_copy : copy.powers)
        {
            if (available_power_desc->get_name() == power_copy->get_name())
            {
                copy.available_powers.push_back(power_copy.get());
                break;
            }
        }
    }
    // restoring available powers and species
    return copy;
}

void Tribe_Stack_Builder::to_json(Json::Value& root)
{
    root["n_created_tribes"] = n_created_tribes;

    Json::Value available_species_json(Json::arrayValue);
    for (auto& species_desc : available_species)
    {
        available_species_json.append(species_desc->get_name());
    }
    root["available_species"] = available_species_json;

    Json::Value available_powers_json(Json::arrayValue);
    for (auto& power_desc : available_powers)
    {
        available_powers_json.append(power_desc->get_name());
    }
    root["available_powers"] = available_powers_json;
}

void Tribe_Stack_Builder::from_json(Json::Value& root)
{
    n_created_tribes = root["n_created_tribes"].asInt();

    available_species.clear();
    for (const auto& species_name_json : root["available_species"])
    {
        std::string species_name = species_name_json.asString();
        for (auto& species_desc : species)
        {
            if (species_desc->get_name() == species_name)
            {
                available_species.push_back(species_desc.get());
                break;
            }
        }
    }

    available_powers.clear();
    for (const auto& power_name_json : root["available_powers"])
    {
        std::string power_name = power_name_json.asString();
        for (auto& power_desc : powers)
        {
            if (power_desc->get_name() == power_name)
            {
                available_powers.push_back(power_desc.get());
                break;
            }
        }
    }
}

}  // namespace state