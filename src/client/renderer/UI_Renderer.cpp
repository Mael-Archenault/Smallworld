#include "renderer.h"
#include "state.h"

namespace renderer
{
UI_Renderer::UI_Renderer(sf::RenderWindow& window)
    : sidebar_renderer(window), map_overlay_renderer(window), widgets_renderer(window)
{
    set_selected_area(-1);
}
void UI_Renderer::render(state::Game_State& state)
{
    sidebar_renderer.render(state);
    map_overlay_renderer.render(state);
    widgets_renderer.render(state);
}

void UI_Renderer::set_selected_area(int area_id)
{
    sidebar_renderer.set_selected_area_id(area_id);
    map_overlay_renderer.set_selected_area_id(area_id);
}

std::unordered_map<std::string, sf::FloatRect> UI_Renderer::get_widgets_layout()
{
    return widgets_renderer.get_layout();
}
}  // namespace renderer