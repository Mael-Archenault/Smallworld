#include "client.h"

namespace client
{
Clickable_State_Interface::Clickable_State_Interface() {}

void Clickable_State_Interface::register_click(sf::Vector2i position)
{
    click_position = position;
}

void Clickable_State_Interface::register_layout(
    std::unordered_map<std::string, sf::FloatRect> layout)
{
    layout_infos = layout;
}

bool Clickable_State_Interface::clicked_on(std::string target_name)
{
    if (layout_infos.find(target_name) != layout_infos.end())
    {
        sf::FloatRect target_rect = layout_infos.at(target_name);
        return target_rect.contains(static_cast<sf::Vector2f>(click_position));
    }
    return false;
}

}  // namespace client