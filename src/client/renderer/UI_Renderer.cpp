#include "renderer.h"
#include "state.h"

namespace renderer
{
UI_Renderer::UI_Renderer(sf::RenderWindow& window, state::Game_State& state)
    : turn_info_renderer(window),
      map_overlay_renderer(window),
      widgets_renderer(window),
      tribe_info_window(window),
      area_info_renderer(window),
      opponents_info_renderer(window, state)
{
    set_selected_area(0);
}
void UI_Renderer::render(state::Game_State& state)
{
    turn_info_renderer.render(state);
    map_overlay_renderer.render(state);
    widgets_renderer.render(state);
    area_info_renderer.render(state);
    opponents_info_renderer.render(state);
    tribe_info_window.render(state);
}

void UI_Renderer::set_selected_area(int area_id)
{
    area_info_renderer.set_selected_area_id(area_id);
    map_overlay_renderer.set_selected_area_id(area_id);
}

std::unordered_map<std::string, sf::FloatRect> UI_Renderer::get_widgets_layout()
{
    return widgets_renderer.get_layout();
}

void UI_Renderer::open_tribe_info_window(state::Tribe& tribe, bool is_buying_possible)
{
    tribe_info_window.set_values(tribe);
    tribe_info_window.set_visibility(true, is_buying_possible);
}

void UI_Renderer::close_tribe_info_window()
{
    tribe_info_window.set_visibility(false, false);
}

std::unordered_map<std::string, sf::FloatRect> UI_Renderer::get_tribe_info_window_layout()
{
    return tribe_info_window.get_layout();
}

std::unordered_map<std::string, sf::FloatRect> UI_Renderer::get_layout()
{
    // returns layout infos: map area, tribe stack area, player area

    std::unordered_map<std::string, sf::FloatRect> result;

    for (const auto& [key, value] : widgets_renderer.get_layout())
    {
        result[key] = value;
    }

    for (const auto& [key, value] : tribe_info_window.get_layout())
    {
        result[key] = value;
    }

    for (const auto& [key, value] : opponents_info_renderer.get_layout())
    {
        result[key] = value;
    }
    return result;
}
}  // namespace renderer