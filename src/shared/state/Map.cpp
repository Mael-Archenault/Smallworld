#include "Map.h"
#include <iostream>
#include <fstream>
#include <json/json.h>


namespace state {

Map::Map() {
}

Area& Map::get_area(int area_id) {
    for (auto area:areas){
        if (area_id==area.id)
        return area;
    }
}


void Map::load_from_json (std::string file_name){
    std::ifstream file(file_name, std::ifstream::binary);
    Json::Value root;
    file >> root;  

    std::cout << ": " << root["biomes"].asString() << std::endl;
}

}