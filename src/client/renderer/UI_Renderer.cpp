#include "renderer.h"
#include "state.h"

namespace renderer
{
UI_Renderer::UI_Renderer(sf::RenderWindow& window) : sidebar_renderer(window) {}
void UI_Renderer::render(state::Game_State& state)
{
    sidebar_renderer.render(state);
}
}  // namespace renderer