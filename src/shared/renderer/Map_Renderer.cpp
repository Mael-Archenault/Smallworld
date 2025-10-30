#include <renderer/Map_Renderer.h>
#include "resources_dir.h"
#include <iostream>
#include <fstream>
#include <json/json.h>

namespace renderer {

    Map_Renderer::Map_Renderer(state::Map& map, sf::Vector2u window_size): map(map) {
      std::string file_path = std::string(RESOURCE_DIR) + "/maps/"+map.get_name()+"/map.png";
      if (!this->texture.loadFromFile(file_path)) {
        throw std::runtime_error("Map_Renderer constructor: Failed to load texture");
      }
      this->sprite.setTexture(this->texture);
      float border_size = ((float)window_size.x)/4;

      // loading the positions

      std::string positions_file = std::string(RESOURCE_DIR) + "/maps/"+map.get_name()+"/positions.json";
      std::ifstream file(positions_file, std::ifstream::binary);
      if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + positions_file);
      }
      Json::Value root;
      file >> root;
      file.close();

      for (const auto& area_id_str : root.getMemberNames()) {
        int area_id = std::stoi(area_id_str);
        float x = root[area_id_str][0].asFloat();
        float y = root[area_id_str][1].asFloat();
        area_positions.push_back(sf::Vector2f(x, y));
      }

    }

  void Map_Renderer::render (sf::RenderWindow& window) {

    sf::Vector2u window_size = window.getSize();

    // map
    float scaling_factor = std::min(
        ((float)window_size.x*5) / (6*this->texture.getSize().x),
        ((float)window_size.y*5) / (6*this->texture.getSize().y)
    );
    this->sprite.setScale(scaling_factor, scaling_factor);
    sf::Vector2f map_position = sf::Vector2f(((float)window_size.x - this->sprite.getGlobalBounds().width) / 2, 0.f);
    this->sprite.setPosition(map_position);
    window.draw(this->sprite);

    // units on map 

    std::vector<std::tuple<int,std::string, int>> display_infos = map.get_display_infos();

    for (int i = 0; i < display_infos.size(); i++) {
      if (std::get<2>(display_infos[i]) > 0) {
          int area_id = std::get<0>(display_infos[i]);
          sf::Vector2f position = area_positions[area_id];
          units_renderer.set_units_number(std::get<2>(display_infos[i]));
          units_renderer.scale(scaling_factor, scaling_factor);
          units_renderer.set_sprite(std::get<1>(display_infos[i]));
          units_renderer.render(window, map_position + scaling_factor * position);
        }
    }
  }

};