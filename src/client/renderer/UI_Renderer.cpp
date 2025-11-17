#include "renderer.h"
#include "state.h"

namespace renderer
{
UI_Renderer::UI_Renderer(sf::RenderWindow& window)
    : sidebar_renderer(window), map_overlay_renderer(window)
{
}
void UI_Renderer::render(state::Game_State& state)
{
    sidebar_renderer.render(state);
    map_overlay_renderer.render(state);

    sidebar_renderer.set_selected_area_id(1);
    map_overlay_renderer.set_selected_area_id(1);
}
}  // namespace renderer