#include "Map.h"
#include <iostream>
#include <fstream>
#include <json/json.h>
#include <algorithm>
#include "resources_dir.h"


namespace state {

Map::Map(std::string name): name(name) {
    load_from_json(std::string(RESOURCE_DIR) + "/maps/" + name + "/data.json");
}

Area* Map::get_area(int area_id) {
    for (int i=0; i<areas.size(); i++){
        if (area_id==areas[i].id)
        return &areas[i];
    }
    throw std::invalid_argument("get_area:there is no area_id matching for this map");
}


void Map::load_from_json (std::string file_name){

    std::ifstream file(file_name, std::ifstream::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + file_name);
    }

    Json::Value root;
    file >> root;
    file.close();

    const Json::Value& biomes = root["biomes"];
    const Json::Value& specs  = root["specializations"];
    const Json::Value& units  = root["units"];
    const Json::Value& rels   = root["relations"];

    this->areas.clear();
    this->areas.reserve(biomes.size());
    
    std::vector<std::pair<int, std::string>> id_keys;
    for (const auto& key : biomes.getMemberNames()) {
        id_keys.emplace_back(std::stoi(key), key);
    }

    std::sort(id_keys.begin(), id_keys.end(), [](const auto& a, const auto& b){ return a.first < b.first; });

    for (const auto& ik : id_keys) {
        int id = ik.first;
        const std::string& key = ik.second;

        // biome
        std::string biome_str = biomes[key][0].asString();
        Area_Biome biome;
        if (biome_str == "Hill") biome = Area_Biome::HILL;
        else if (biome_str == "Mountains") biome = Area_Biome::MOUNTAINS;
        else if (biome_str == "Forest") biome = Area_Biome::FOREST;
        else if (biome_str == "Swamp") biome = Area_Biome::SWAMP;
        else if (biome_str == "Farm") biome = Area_Biome::FARM;
        else if (biome_str == "Water") biome = Area_Biome::WATER;
        else throw std::runtime_error("Unknown biome type: " + biome_str);

        // specializations
        std::vector<Area_Specialization> specializations;
        for (const auto& s : specs[key]) {
            std::string s_str = s.asString();
            if (s_str == "Magic Region")
                specializations.push_back(Area_Specialization::MAGIC_REGION);
            else if (s_str == "Cavern")
                specializations.push_back(Area_Specialization::CAVERN);
            else if (s_str == "Mine")
                specializations.push_back(Area_Specialization::MINE);
            else
                throw std::runtime_error("Unknown specialization type: " + s_str);
        }

        // unit count
        int unit_count = units[key].asInt();

        // create the area in-place (avoids using deleted assignment)
        this->areas.emplace_back(id, unit_count, biome, specializations);

        // relations (neighbors)
        for (const auto& neighbor_id_val : rels[key]) {
            int neighbor_id = neighbor_id_val.asInt();
            this->areas.back().add_neighbor(&this->areas[neighbor_id]);
        }
    }
}

std::string Map::get_name () {
    return this->name;
}

std::vector<Area>& Map::get_areas () {
    return this->areas;
}

}