#include <state.h>

#include <SFML/Graphics.hpp>
#include <unordered_map>

#include "renderer.h"
#include "resources_dir.h"

namespace renderer
{

Game_Renderer::Game_Renderer(state::Game_State& state, sf::RenderWindow& window)
    : window(window),
      map_renderer(window),
      tribe_stack_renderer(window),
      player_area_renderer(window),
      UI_renderer(window, state)
{
    // load background image
    std::string file_path = std::string(RESOURCE_DIR) + "/assets/general_background_blurred.png";
    if (!background_texture.loadFromFile(file_path))
    {
        throw std::runtime_error("Online_Lobby_Renderer constructor: Failed to load texture :" +
                                 file_path);
    }
    background_sprite.setTexture(background_texture);
};

void Game_Renderer::render(state::Game_State& state, int rendering_player_id)
{
    window.clear(sf::Color::Black);

    sf::Vector2u window_size = window.getSize();

    // map
    float scaling_factor = std::max(((float) window_size.x / background_texture.getSize().x),
                                    ((float) window_size.y / background_texture.getSize().y));
    background_sprite.setScale(scaling_factor, scaling_factor);
    sf::Vector2f map_position =
        sf::Vector2f(((float) window_size.x - background_sprite.getGlobalBounds().width) / 2,
                     ((float) window_size.y - background_sprite.getGlobalBounds().height) / 2);
    background_sprite.setPosition(map_position);
    window.draw(background_sprite);

    sf::RectangleShape background(sf::Vector2f(window_size.x, window_size.y));
    background.setFillColor(sf::Color(50, 50, 50, 100));
    background.setPosition(sf::Vector2f(0.f, 0.f));
    window.draw(background);

    map_renderer.render(state.get_map());

    // Render the tribe stack at a fixed position
    tribe_stack_renderer.set_position(sf::Vector2f(window_size.x * 5 / 6.f, 0.f));
    tribe_stack_renderer.render(state.get_tribe_stack());

    state::Player& rendering_player = state.get_players().at(rendering_player_id);
    player_area_renderer.render(rendering_player);

    UI_renderer.render(state, rendering_player_id);
}

void Game_Renderer::set_selected_area(int area_id)
{
    UI_renderer.set_selected_area(area_id);
}

std::unordered_map<std::string, sf::FloatRect> Game_Renderer::get_layout_infos()
{
    // returns layout infos: map area, tribe stack area, player area

    std::unordered_map<std::string, sf::FloatRect> result;

    result["map"]         = map_renderer.get_layout();
    result["tribe_stack"] = tribe_stack_renderer.get_layout();

    for (const auto& [key, value] : UI_renderer.get_layout())
    {
        result[key] = value;
    }

    for (const auto& [key, value] : player_area_renderer.get_layout())
    {
        result[key] = value;
    }

    return result;
}

std::vector<sf::Vector2f> Game_Renderer::get_on_screen_area_positions()
{
    return map_renderer.get_on_screen_area_positions();
}

std::vector<sf::Vector2f> Game_Renderer::get_on_screen_tribe_positions()
{
    return tribe_stack_renderer.get_on_screen_tribe_positions();
}

void Game_Renderer::open_tribe_info_window(state::Tribe& tribe, bool is_buying_possible)
{
    UI_renderer.open_tribe_info_window(tribe, is_buying_possible);
}

void Game_Renderer::close_tribe_info_window()
{
    UI_renderer.close_tribe_info_window();
}

}  // namespace renderer