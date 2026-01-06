#include <json/json.h>
#include <renderer/Map_Renderer.h>

#include <cmath>
#include <fstream>
#include <iostream>

#include "resources_dir.h"

namespace renderer
{
std::vector<std::string> special_tokens_names = {
    "Troll Lair", "Fortress", "Mountain", "Encampment", "Hole In The Ground", "Hero", "Dragon"};

std::vector<std::string> area_specializations_names = {"Mine", "Magic Region", "Cavern"};

Map_Renderer::Map_Renderer(sf::RenderWindow& window)
    : window(window),
      units_renderer("pawn"),
      special_tokens_renderer("special_tokens"),
      area_specialization_renderer("area_specialization")
{
    loaded_map_name = "None";
}

void Map_Renderer::load_map_resources(std::string map_name)
{
    // loading the image
    std::string file_path = std::string(RESOURCE_DIR) + "/maps/" + map_name + "/map.png";
    if (!this->texture.loadFromFile(file_path))
    {
        throw std::runtime_error("Map_Renderer constructor: Failed to load texture :" + file_path);
    }
    this->sprite.setTexture(this->texture);

    // loading the positions
    std::string positions_file =
        std::string(RESOURCE_DIR) + "/maps/" + map_name + "/positions.json";
    std::ifstream file(positions_file, std::ifstream::binary);

    if (!file.is_open())
    {
        throw std::runtime_error("Map_Renderer constructor: Failed to open positions file: " +
                                 positions_file);
    }
    Json::Value root;
    file >> root;
    file.close();

    area_positions.resize(root.getMemberNames().size());
    for (const auto& area_id_str : root.getMemberNames())
    {
        int   area_id              = std::stoi(area_id_str);
        float x                    = root[area_id_str][0].asFloat();
        float y                    = root[area_id_str][1].asFloat();
        area_positions.at(area_id) = sf::Vector2f(x, y);
    }
    loaded_map_name = map_name;
}

void Map_Renderer::render(state::Map& map)
{
    if (loaded_map_name != map.get_name())
    {
        load_map_resources(map.get_name());
    }

    sf::Vector2u window_size = window.getSize();

    // map
    float scaling_factor = std::min(((float) window_size.x * 5) / (6 * this->texture.getSize().x),
                                    ((float) window_size.y * 5) / (6 * this->texture.getSize().y));
    this->sprite.setScale(scaling_factor, scaling_factor);
    sf::Vector2f map_position =
        sf::Vector2f(((float) window_size.x - this->sprite.getGlobalBounds().width) / 2, 0.f);
    this->sprite.setPosition(map_position);
    window.draw(this->sprite);

    std::vector<state::Area> areas = map.get_areas();

    for (size_t i = 0; i < areas.size(); i++)
    {
        sf::Vector2f area_position = map_position + area_positions[areas.at(i).id] * scaling_factor;

        // area specialisation

        std::vector<state::Area_Specialization> specializations =
            areas.at(i).get_area_specialization();
        if (specializations.size() != 0)
        {
            for (size_t j = 0; j < specializations.size(); j++)
            {
                area_specialization_renderer.scale(scaling_factor, scaling_factor);
                area_specialization_renderer.set_sprite(

                    area_specializations_names.at(specializations.at(j)));
                sf::Vector2f position =
                    area_position -
                    sf::Vector2f(
                        (area_specialization_renderer.get_sprite_width() * scaling_factor) / 2,
                        (area_specialization_renderer.get_sprite_height() * scaling_factor) / 2);
                // rotated around the center
                position += 100.f * scaling_factor *
                            sf::Vector2f(std::cos(j * 90.f * M_PI / 180.f),
                                         std::sin(j * 90.f * M_PI / 180.f));
                area_specialization_renderer.render(window, position, false);
            }
        }

        // special tokens

        std::vector<state::Area_Special_Token> tokens = areas.at(i).get_special_tokens();
        if (tokens.size() != 0)
        {
            for (size_t j = 0; j < tokens.size(); j++)
            {
                special_tokens_renderer.scale(scaling_factor, scaling_factor);
                special_tokens_renderer.set_sprite(special_tokens_names.at(tokens.at(j)));

                sf::Vector2f position =
                    (tokens.at(j) == state::Area_Special_Token::MOUNTAIN)
                        ? area_position -
                              sf::Vector2f(
                                  (special_tokens_renderer.get_sprite_width() * scaling_factor) / 2,
                                  (special_tokens_renderer.get_sprite_height() * scaling_factor) /
                                      2)
                        : area_position -
                              sf::Vector2f(
                                  (special_tokens_renderer.get_sprite_width() * scaling_factor) / 2,
                                  (special_tokens_renderer.get_sprite_height() * scaling_factor) /
                                      2) +
                              sf::Vector2f(75.f, 75.f) * scaling_factor * (float) j;

                special_tokens_renderer.render(window, position, false);
            }
        }
        // units pawns
        if (areas.at(i).get_units_number() != 0)
        {
            state::Tribe* owner_tribe = areas.at(i).get_owner_tribe();

            units_renderer.set_number(areas.at(i).get_units_number());
            units_renderer.scale(scaling_factor, scaling_factor);
            std::string name =
                (owner_tribe == nullptr) ? "Lost Tribe" : owner_tribe->get_species_name();
            units_renderer.set_sprite(name);

            if (owner_tribe != nullptr && owner_tribe->get_is_in_decline())
            {
                units_renderer.color(sf::Color(39, 224, 245));
            }
            else
            {
                units_renderer.color(sf::Color(255, 255, 255));
            }
            units_renderer.render(window, area_position, true);
        }
    }
}

sf::FloatRect Map_Renderer::get_layout()
{
    return this->sprite.getGlobalBounds();
}

std::vector<sf::Vector2f> Map_Renderer::get_on_screen_area_positions()
{
    sf::Vector2u window_size = window.getSize();
    float scaling_factor = std::min(((float) window_size.x * 5) / (6 * this->texture.getSize().x),
                                    ((float) window_size.y * 5) / (6 * this->texture.getSize().y));
    sf::Vector2f map_position =
        sf::Vector2f(((float) window_size.x - this->sprite.getGlobalBounds().width) / 2, 0.f);

    std::vector<sf::Vector2f> result(area_positions.size());
    for (size_t i = 0; i < area_positions.size(); i++)
    {
        result.at(i) = map_position + area_positions.at(i) * scaling_factor;
    }
    return result;
}

};  // namespace renderer