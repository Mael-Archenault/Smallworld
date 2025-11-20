#include "renderer.h"

namespace renderer
{
Button::Button() {}

void Button::set_on_click_action(std::function<void()> action)
{
    on_click_action = action;
}

bool Button::is_clicked(sf::Vector2f mouse_position)
{
    return box.getGlobalBounds().contains(mouse_position);
}

void Button::execute()
{
    if (on_click_action)
    {
        on_click_action();
    }
}

sf::FloatRect Button::get_rect()
{
    return box.getGlobalBounds();
}
}  // namespace renderer