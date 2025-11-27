#include "Renderer.h"

#include <state.h>

#include <SFML/Graphics.hpp>
#include <unordered_map>

namespace renderer
{

Renderer::Renderer(state::Game_State& state, sf::RenderWindow& window)
    : window(window),
      map_renderer(window),
      tribe_stack_renderer(window),
      player_area_renderer(window),
      UI_renderer(window) {};

void Renderer::render(state::Game_State& state)
{
    sf::Vector2u window_size = window.getSize();
    map_renderer.render(state.get_map());

    // Render the tribe stack at a fixed position
    tribe_stack_renderer.set_position(sf::Vector2f(window_size.x * 5 / 6.f, 0.f));
    tribe_stack_renderer.render(state.get_tribe_stack());

    player_area_renderer.set_position(sf::Vector2f(window_size.x / 6, window_size.y * 5 / 6));
    player_area_renderer.render(state.get_current_player());

    UI_renderer.render(state);
}

void Renderer::set_selected_area(int area_id)
{
    UI_renderer.set_selected_area(area_id);
}

std::unordered_map<std::string, sf::FloatRect> Renderer::get_layout_infos()
{
    // returns layout infos: map area, tribe stack area, player area

    std::unordered_map<std::string, sf::FloatRect> result;

    result["map"]         = map_renderer.get_layout();
    result["tribe_stack"] = tribe_stack_renderer.get_layout();

    for (const auto& [key, value] : UI_renderer.get_widgets_layout())
    {
        result[key] = value;
    }

    for (const auto& [key, value] : UI_renderer.get_tribe_info_window_layout())
    {
        result[key] = value;
    }

    for (const auto& [key, value] : player_area_renderer.get_layout())
    {
        result[key] = value;
    }
    return result;
}

std::vector<sf::Vector2f> Renderer::get_on_screen_area_positions()
{
    return map_renderer.get_on_screen_area_positions();
}

std::vector<sf::Vector2f> Renderer::get_on_screen_tribe_positions()
{
    return tribe_stack_renderer.get_on_screen_tribe_positions();
}

void Renderer::open_tribe_info_window(state::Tribe& tribe)
{
    UI_renderer.open_tribe_info_window(tribe);
}

void Renderer::close_tribe_info_window()
{
    UI_renderer.close_tribe_info_window();
}

}  // namespace renderer