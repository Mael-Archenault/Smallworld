#include "renderer.h"
#include "resources_dir.h"

namespace renderer
{
Menu_Renderer::Menu_Renderer(sf::RenderWindow& window) : window(window)
{
    // load background image
    std::string file_path = std::string(RESOURCE_DIR) + "/assets/menu_background.png";
    if (!background_texture.loadFromFile(file_path))
    {
        throw std::runtime_error("Menu_Renderer constructor: Failed to load texture :" + file_path);
    }
    background_sprite.setTexture(background_texture);

    online_button.set_content("Online Game");
    online_button.set_colors(sf::Color::White, sf::Color(50, 50, 50), sf::Color::White);

    local_button.set_content("Local Game");
    local_button.set_colors(sf::Color::White, sf::Color(50, 50, 50), sf::Color::White);
}

void Menu_Renderer::render()
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

    float button_width  = window_size.x / 7.f;
    float button_height = window_size.y / 12.f;

    local_button.set_size(sf::Vector2f(button_width, button_height));
    local_button.set_position(
        sf::Vector2f(window_size.x / 2 - 2 * button_width, window_size.y * 4 / 5));
    local_button.set_character_size(button_width / 10);
    local_button.render(window);

    online_button.set_size(sf::Vector2f(button_width, button_height));
    online_button.set_position(
        sf::Vector2f(window_size.x / 2 + button_width, window_size.y * 4 / 5));
    online_button.set_character_size(button_width / 10);
    online_button.render(window);
}

std::unordered_map<std::string, sf::FloatRect> Menu_Renderer::get_layout_infos()
{
    std::unordered_map<std::string, sf::FloatRect> button_positions;
    button_positions["local_button"]  = local_button.get_rect();
    button_positions["online_button"] = online_button.get_rect();

    return button_positions;
}
}  // namespace renderer