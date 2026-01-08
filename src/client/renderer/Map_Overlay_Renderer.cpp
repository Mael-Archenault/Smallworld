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
}

void Map_Overlay_Renderer::load_map_borders(state::Map& map)
{
    int n_areas = map.get_areas().size();
    border_textures.resize(n_areas);

    for (int area_id = 0; area_id < n_areas; ++area_id)
    {
        if (!border_textures.at(area_id).loadFromFile(std::string(RESOURCE_DIR) + "/maps/" +
                                                      map.get_name() + "/borders/" +
                                                      std::to_string(area_id) + ".png"))
        {
            throw std::runtime_error("Map_Overlay_Renderer: Failed to load border for area :" +
                                     std::to_string(area_id));
        }

        border_sprites.emplace_back();
        border_sprites.at(area_id).setTexture(border_textures.at(area_id));
    }
}
void Map_Overlay_Renderer::render(state::Game_State& state, int rendering_player_id)
{
    state::Player& rendering_player = state.get_players().at(rendering_player_id);
    if (state.get_map().get_name() != loaded_map_name)
    {
        load_map_positions(state.get_map().get_name());
        load_map_borders(state.get_map());
        loaded_map_name = state.get_map().get_name();
    }

    float scaling_factor =
        std::min(((float) window.getSize().x * 5) / (6 * border_textures.at(0).getSize().x),
                 ((float) window.getSize().y * 5) / (6 * border_textures.at(0).getSize().y));
    if (rendering_player.id == state.get_current_player().id)
    {
        if (state.get_current_turn_phase() == state::Turn_Phase::CONQUER)
        {
            std::vector<std::pair<int, int>> conquest_prices =
                state.get_conquest_prices(rendering_player.id);

            for (auto& price_pair : conquest_prices)
            {
                int area_id = price_pair.first;

                sf::Color border_color =
                    (price_pair.second <= state.get_free_units_number(rendering_player.id))
                        ? sf::Color(0, 255, 0)
                        : sf::Color(255, 255, 0);
                border_sprites.at(area_id).setColor(border_color);
                border_sprites.at(area_id).setScale(scaling_factor, scaling_factor);
                border_sprites.at(area_id).setPosition(
                    sf::Vector2f(((float) window.getSize().x -
                                  border_sprites.at(area_id).getGlobalBounds().width) /
                                     2,
                                 0.f));
                window.draw(border_sprites.at(area_id));
            }
        }

        if (state.get_current_turn_phase() == state::Turn_Phase::REDEPLOY)
        {
            std::vector<int> redeployable_areas = rendering_player.get_redeployable_areas();

            for (auto& area_id : redeployable_areas)
            {
                sf::Color border_color = sf::Color(0, 0, 255);
                border_sprites.at(area_id).setColor(border_color);
                border_sprites.at(area_id).setScale(scaling_factor, scaling_factor);
                border_sprites.at(area_id).setPosition(
                    sf::Vector2f(((float) window.getSize().x -
                                  border_sprites.at(area_id).getGlobalBounds().width) /
                                     2,
                                 0.f));
                window.draw(border_sprites.at(area_id));
            }
        }
    }
    if (selected_area_id == -1)
    {  // no area selected
        return;
    }

    border_sprites.at(selected_area_id).setColor(sf::Color(255, 0, 0));
    border_sprites.at(selected_area_id).setScale(scaling_factor, scaling_factor);
    border_sprites.at(selected_area_id)
        .setPosition(sf::Vector2f(((float) window.getSize().x -
                                   border_sprites.at(selected_area_id).getGlobalBounds().width) /
                                      2,
                                  0.f));
    window.draw(border_sprites.at(selected_area_id));
}
}  // namespace renderer