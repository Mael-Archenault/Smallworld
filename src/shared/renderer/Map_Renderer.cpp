#include <renderer/Map_Renderer.h>
#include "resources_dir.h"
#include <iostream>
#include <fstream>
#include <json/json.h>

namespace renderer
{

  Map_Renderer::Map_Renderer(state::Map &map, sf::Vector2u window_size) : map(map), units_renderer("pawn"), special_tokens_renderer("special_tokens")
  {
    std::string file_path = std::string(RESOURCE_DIR) + "/maps/" + map.get_name() + "/map.png";
    if (!this->texture.loadFromFile(file_path))
    {
      throw std::runtime_error("Map_Renderer constructor: Failed to load texture");
    }
    this->sprite.setTexture(this->texture);
    float border_size = ((float)window_size.x) / 4;

    // loading the positions

    std::string positions_file = std::string(RESOURCE_DIR) + "/maps/" + map.get_name() + "/positions.json";
    std::ifstream file(positions_file, std::ifstream::binary);
    if (!file.is_open())
    {
      throw std::runtime_error("Cannot open file: " + positions_file);
    }
    Json::Value root;
    file >> root;
    file.close();

    area_positions.resize(map.get_areas().size());
    for (const auto &area_id_str : root.getMemberNames())
    {
      int area_id = std::stoi(area_id_str);
      float x = root[area_id_str][0].asFloat();
      float y = root[area_id_str][1].asFloat();
      area_positions.at(area_id) = sf::Vector2f(x, y);
    }
  }

  void Map_Renderer::render(sf::RenderWindow &window)
  {

    sf::Vector2u window_size = window.getSize();

    // map
    float scaling_factor = std::min(
        ((float)window_size.x * 5) / (6 * this->texture.getSize().x),
        ((float)window_size.y * 5) / (6 * this->texture.getSize().y));
    this->sprite.setScale(scaling_factor, scaling_factor);
    sf::Vector2f map_position = sf::Vector2f(((float)window_size.x - this->sprite.getGlobalBounds().width) / 2, 0.f);
    this->sprite.setPosition(map_position);
    window.draw(this->sprite);

    std::vector<state::Area> areas = map.get_areas();

    for (int i = 0; i < areas.size(); i++)
    {
      sf::Vector2f area_position = map_position + area_positions[areas[i].id] * scaling_factor;

      
      // special tokens

      std::vector<state::Area_Special_Token> tokens = areas[i].get_special_tokens();
      if (areas[i].id == 4){
        tokens.push_back(state::Area_Special_Token::TROLL_LAIR);
        tokens.push_back(state::Area_Special_Token::HOLE_IN_THE_GROUND);
      }
      if (tokens.size() != 0)
      {
        for (int j = 0; j < tokens.size(); j++)
        {
          special_tokens_renderer.scale(scaling_factor, scaling_factor);
          if (tokens[j] == state::Area_Special_Token::MOUNTAIN)
            special_tokens_renderer.set_sprite("Mountains");
          else if (tokens[j] == state::Area_Special_Token::TROLL_LAIR)
            special_tokens_renderer.set_sprite("Troll_Lair");
          else if (tokens[j] == state::Area_Special_Token::FORTRESS)
            special_tokens_renderer.set_sprite("Fortress");
          else if (tokens[j] == state::Area_Special_Token::ENCAMPMENT)
            special_tokens_renderer.set_sprite("Encampment");
          else if (tokens[j] == state::Area_Special_Token::HOLE_IN_THE_GROUND)
            special_tokens_renderer.set_sprite("Hole In The Ground");
            else if (tokens[j] == state::Area_Special_Token::HERO)
            special_tokens_renderer.set_sprite("Hero");
          else if (tokens[j] == state::Area_Special_Token::DRAGON)
            special_tokens_renderer.set_sprite("Dragon");

          sf::Vector2f position = (tokens[j] == state::Area_Special_Token::MOUNTAIN) ?
            area_position - sf::Vector2f((special_tokens_renderer.get_sprite_width() * scaling_factor) / 2, (special_tokens_renderer.get_sprite_height() * scaling_factor) / 2) :
            area_position - sf::Vector2f((special_tokens_renderer.get_sprite_width() * scaling_factor) / 2, (special_tokens_renderer.get_sprite_height() * scaling_factor) / 2) + sf::Vector2f(75.f, 75.f) * scaling_factor * (float)j;

          special_tokens_renderer.render(window, position, false);

        }
      }
      // units pawns
      if (areas[i].get_units_number() != 0)
      {
        units_renderer.set_number(areas[i].get_units_number());
        units_renderer.scale(scaling_factor, scaling_factor);
        units_renderer.set_sprite(areas[i].get_owner_tribe_name());
        units_renderer.render(window, area_position, true);
      }
    }
  }

};