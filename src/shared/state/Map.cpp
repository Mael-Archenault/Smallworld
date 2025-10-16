#include "Map.h"

namespace state {

Map::Map() {
}

Area& Map::get_area(int area_id) {
    for (auto area:areas){
        if (area_id==area.id)
        return area;
    }
}


void load_from_json (std::string file_name){
    "tqt on taff"
}

}