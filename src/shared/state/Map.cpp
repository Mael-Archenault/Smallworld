#include "Map.h"

#include <json/json.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include "resources_dir.h"

namespace state
{

std::unordered_map<std::string, Area_Biome> str_to_biome = {
    {"Hill", Area_Biome::HILL},     {"Mountains", Area_Biome::MOUNTAINS},
    {"Forest", Area_Biome::FOREST}, {"Swamp", Area_Biome::SWAMP},
    {"Farm", Area_Biome::FARM},     {"Water", Area_Biome::WATER}};

std::unordered_map<std::string, Area_Specialization> str_to_specialization = {
    {"Magic Region", Area_Specialization::MAGIC_REGION},
    {"Cavern", Area_Specialization::CAVERN},
    {"Mine", Area_Specialization::MINE}};

Map::Map(std::string name) : name(name)
{
    load_from_json(std::string(RESOURCE_DIR) + "/maps/" + name + "/data.json");
}

Area& Map::get_area(int area_id)
{
    for (size_t i = 0; i < areas.size(); i++)
    {
        if (area_id == areas[i].id) return areas[i];
    }
    throw std::invalid_argument("get_area:there is no area_id matching for this map");
}

void Map::load_from_json(std::string file_name)
{
    std::ifstream file(file_name, std::ifstream::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file: " + file_name);
    }

    Json::Value root;
    file >> root;
    file.close();

    // pre-preparing the ids
    std::vector<int> ids;
    ids.reserve(root.getMemberNames().size());
    for (const auto& s : root.getMemberNames()) ids.push_back(std::stoi(s));
    if (ids.empty()) return;
    int max_id = *std::max_element(ids.begin(), ids.end());

    // resetting current values
    this->areas.clear();
    this->area_connections.clear();

    // create each area in ids order
    for (int id = 0; id <= max_id; ++id)
    {
        const std::string sid = std::to_string(id);

        Json::Value area_infos = root[sid];

        Area_Biome                       biome = str_to_biome[area_infos["biome"].asString()];
        std::vector<Area_Specialization> specializations;
        for (auto s : area_infos["specializations"])
        {
            specializations.push_back(str_to_specialization[s.asString()]);
        }
        int  unit_count = area_infos["units"].asInt();
        bool is_border  = area_infos["borders"].asBool();

        // emplace area so its position in the vector matches its id
        this->areas.emplace_back(id, unit_count, biome, specializations, is_border);
    }

    // setting areas relations
    for (int id = 0; id <= max_id; ++id)
    {
        const std::string sid       = std::to_string(id);
        Json::Value       relations = root[sid]["relations"];
        std::vector<int>  connections;
        for (const auto& neighbor_id_val : relations)
        {
            int neighbor_id = neighbor_id_val.asInt();
            areas.at(id).add_neighbor(&areas.at(neighbor_id));
            connections.push_back(neighbor_id);
        }
        this->area_connections.push_back(connections);
    }
}

std::string Map::get_name()
{
    return this->name;
}

std::vector<Area>& Map::get_areas()
{
    return this->areas;
}

std::vector<std::pair<int, int>> Map::get_starting_points_prices(Tribe& tribe,
                                                                 bool   can_start_anywhere)
{
    std::vector<std::pair<int, int>> ret = std::vector<std::pair<int, int>>();

    for (Area& area : this->areas)
    {
        if (area.is_border or can_start_anywhere)
        {
            std::pair<int, int> pair = std::make_pair(area.id, area.get_conquest_price(tribe));
            ret.push_back(pair);
        }
    }
    return ret;
}

}  // namespace state