#include "Renderer.h"

#include <state.h>

#include <SFML/Graphics.hpp>

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

}  // namespace renderer