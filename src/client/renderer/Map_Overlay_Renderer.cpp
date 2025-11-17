#include <json/json.h>

#include <fstream>
#include <iostream>

#include "renderer.h"
#include "resources_dir.h"
#include "state.h"

namespace renderer
{
Map_Overlay_Renderer::Map_Overlay_Renderer(sf::RenderWindow& window) : window(window)
{
    loaded_map_name = "None";
    set_selected_area_id(-1);
}

void Map_Overlay_Renderer::set_selected_area_id(int area_id)
{
    selected_area_id = area_id;

    if (selected_area_id == -1)
    {  // no area selected
        return;
    }

    if (!selected_area_border_texture.loadFromFile(std::string(RESOURCE_DIR) + "/maps/" +
                                                   loaded_map_name + "/borders/" +
                                                   std::to_string(selected_area_id) + ".png"))
    {
        throw std::runtime_error("Map_Overlay_Renderer: Failed to load border for selected area");
    }
    selected_area_border_sprite.setTexture(selected_area_border_texture);
}

void Map_Overlay_Renderer::load_map_positions(std::string map_name)
{
    std::string positions_file =
        std::string(RESOURCE_DIR) + "/maps/" + map_name + "/positions.json";
    std::ifstream file(positions_file, std::ifstream::binary);

    if (!file.is_open())
    {
        throw std::runtime_error("Map_Overlay: Failed to open positions file: " + positions_file);
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

void Map_Overlay_Renderer::render(state::Game_State& state)
{
    if (state.get_map().get_name() != loaded_map_name)
    {
        load_map_positions(state.get_map().get_name());
    }

    if (selected_area_id == -1)
    {  // no area selected
        return;
    }

    float scaling_factor =
        std::min(((float) window.getSize().x * 5) / (6 * selected_area_border_texture.getSize().x),
                 ((float) window.getSize().y * 5) / (6 * selected_area_border_texture.getSize().y));

    selected_area_border_sprite.setColor(sf::Color(255, 0, 0));
    selected_area_border_sprite.setScale(scaling_factor, scaling_factor);
    selected_area_border_sprite.setPosition(sf::Vector2f(
        ((float) window.getSize().x - selected_area_border_sprite.getGlobalBounds().width) / 2,
        0.f));
    window.draw(selected_area_border_sprite);
}
}  // namespace renderer