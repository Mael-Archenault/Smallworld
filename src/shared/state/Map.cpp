#include "Map.h"
#include <iostream>
#include <fstream>
#include <json/json.h>
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

    // assume Map has std::vector<Area> areas;
    this->areas.clear();

    // iterate through keys (0, 1, 2, ...)
    for (const auto& key : biomes.getMemberNames()) {
        int id = std::stoi(key);

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
            if (s_str == "MAGIC_REGION")
                specializations.push_back(Area_Specialization::MAGIC_REGION);
            // add others as needed
        }

        // unit count
        int unit_count = units[key].asInt();


        // create the area
        Area area(id,unit_count, biome, specializations);

        this->areas.push_back(area);
    }

    // now handle relations
    for (const auto& key : rels.getMemberNames()) {
        int id = std::stoi(key);
        std::vector<int> connections;
        for (const auto& rel_id_json : rels[key]) {
            int rel_id = rel_id_json.asInt();
            this->areas[id].add_neighbor(&this->areas[rel_id]);
            connections.push_back(rel_id);
        }
        this->area_connections.push_back(connections);
    }
}

std::string Map::get_name () {
    return this->name;
}

std::vector<Area>& Map::get_areas () {
    return this->areas;
}

}