#include <renderer/Map_Renderer.h>
#include "resources_dir.h"
#include <iostream>
#include <fstream>
#include <json/json.h>
#include <cmath>

namespace renderer
{

  Map_Renderer::Map_Renderer(state::Map &map, sf::Vector2u window_size) : map(map), units_renderer("pawn"), special_tokens_renderer("special_tokens"), area_specialization_renderer("area_specialization")
  {
    // loading the map image
    std::string file_path = std::string(RESOURCE_DIR) + "/maps/" + map.get_name() + "/map.png";
    if (!this->texture.loadFromFile(file_path))
    {
      throw std::runtime_error("Map_Renderer constructor: Failed to load texture :" + file_path);
    }
    this->sprite.setTexture(this->texture);
    float border_size = ((float)window_size.x) / 4;

    // loading the positions
    std::string positions_file = std::string(RESOURCE_DIR) + "/maps/" + map.get_name() + "/positions.json";
    std::ifstream file(positions_file, std::ifstream::binary);
    if (!file.is_open())
    {
      throw std::runtime_error("Map_Renderer constructor: Failed to open positions file: " + positions_file);
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

      // area specialisation

      std::vector<state::Area_Specialization> specializations = areas[i].get_area_specialization();
      if (specializations.size() != 0)
      {
        for (int j = 0; j < specializations.size(); j++)
        {
          area_specialization_renderer.scale(scaling_factor, scaling_factor);
          if (specializations[j] == state::Area_Specialization::CAVERN)
            area_specialization_renderer.set_sprite("Cavern");
          else if (specializations[j] == state::Area_Specialization::MAGIC_REGION)
            area_specialization_renderer.set_sprite("Magic Region");
          else if (specializations[j] == state::Area_Specialization::MINE)
            area_specialization_renderer.set_sprite("Mine");
          sf::Vector2f position = area_position - sf::Vector2f((area_specialization_renderer.get_sprite_width() * scaling_factor) / 2, (area_specialization_renderer.get_sprite_height() * scaling_factor) / 2);
          // rotated around the center
          position += 100.f*scaling_factor* sf::Vector2f(std::cos(j*90.f*M_PI/180.f), std::sin(j*90.f*M_PI/180.f));
          area_specialization_renderer.render(window, position, false);
        }
      }

      
      // special tokens

      std::vector<state::Area_Special_Token> tokens = areas[i].get_special_tokens();
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
        state::Tribe* owner_tribe = areas[i].get_owner_tribe();
        units_renderer.set_number(areas[i].get_units_number());
        units_renderer.scale(scaling_factor, scaling_factor);
        units_renderer.set_sprite(owner_tribe->get_species_name());

        if (owner_tribe->is_in_decline()) {
          units_renderer.color(sf::Color(39, 224, 245));
        } else {
          units_renderer.color(sf::Color(255,255,255));
        }
        units_renderer.render(window, area_position, true);
      }
    }
  }

};