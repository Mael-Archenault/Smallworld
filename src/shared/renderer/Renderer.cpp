#include "Renderer.h"
#include <state.h>
#include <SFML/Graphics.hpp>


namespace renderer {

    Renderer::Renderer (state::Game_State& state, sf::Vector2u window_size)
    : state(state),
      map_renderer(state.get_map(), window_size),
      tribe_stack_renderer(state.get_tribe_stack()),
      player_area_renderer(state.get_current_player())
    {
    };

    void Renderer::render (sf::RenderWindow& window) {
        sf::Vector2u window_size = window.getSize();
        // Render the map at position (0,0)
        map_renderer.render(window);

        // Render the tribe stack at a fixed position
        tribe_stack_renderer.set_position(sf::Vector2f(window_size.x*5/6.f, 0.f));
        tribe_stack_renderer.render(window);

        player_area_renderer.set_position(sf::Vector2f(window_size.x/6, window_size.y*5/6));
        player_area_renderer.render(window);
    }

} // namespace renderer