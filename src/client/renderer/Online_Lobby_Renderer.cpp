#include "renderer.h"
#include "resources_dir.h"

namespace renderer
{
Online_Lobby_Renderer::Online_Lobby_Renderer(sf::RenderWindow& window, std::string room_id)
    : window(window)
{
    // load background image
    std::string file_path = std::string(RESOURCE_DIR) + "/assets/general_background_blurred.png";
    if (!background_texture.loadFromFile(file_path))
    {
        throw std::runtime_error("Online_Lobby_Renderer constructor: Failed to load texture :" +
                                 file_path);
    }
    background_sprite.setTexture(background_texture);

    room_id_indicator.set_content("Room_id : " + room_id);
    room_id_indicator.set_colors(sf::Color::White, sf::Color(50, 50, 50), sf::Color::White);

    exit_button.set_content("Exit room");
    exit_button.set_colors(sf::Color::Black, sf::Color(50, 50, 50), sf::Color::White);

    start_button.set_content("Start Game");
    start_button.set_colors(sf::Color::Black, sf::Color(50, 50, 50), sf::Color::White);
}

void Online_Lobby_Renderer::render(std::vector<std::string> player_names)
{
    window.clear(sf::Color::Black);

    sf::Vector2u window_size = window.getSize();

    // map
    float scaling_factor = std::min(((float) window_size.x / background_texture.getSize().x),
                                    ((float) window_size.y / background_texture.getSize().y));
    background_sprite.setScale(scaling_factor, scaling_factor);
    sf::Vector2f map_position =
        sf::Vector2f(((float) window_size.x - background_sprite.getGlobalBounds().width) / 2,
                     ((float) window_size.y - background_sprite.getGlobalBounds().height) / 2);
    background_sprite.setPosition(map_position);
    window.draw(background_sprite);

    float button_width  = window_size.x / 4.f;
    float button_height = window_size.y / 12.f;

    room_id_indicator.set_size(sf::Vector2f(button_width, button_height));
    room_id_indicator.set_position(sf::Vector2f(0.f, 0.f));
    room_id_indicator.set_character_size(button_height / 3);
    room_id_indicator.render(window);

    exit_button.set_size(sf::Vector2f(button_width / 2, button_height));
    exit_button.set_position(
        sf::Vector2f(window_size.x - button_width / 2, window_size.y - button_height));
    exit_button.set_character_size(button_height / 3);
    exit_button.render(window);

    start_button.set_size(sf::Vector2f(button_width / 2, button_height));
    start_button.set_position(
        sf::Vector2f(window_size.x / 2 - button_width / 4, window_size.y - button_height));
    start_button.set_character_size(button_height / 3);
    start_button.render(window);

    for (size_t i = 0; i < player_names.size(); ++i)
    {
        Text_Box player_box;
        player_box.set_content(player_names[i]);
        if (i == 0)  // owner
        {
            player_box.set_colors(sf::Color::Yellow, sf::Color(50, 50, 50), sf::Color::Yellow);
        }
        else
        {
            player_box.set_colors(sf::Color::White, sf::Color(50, 50, 50), sf::Color::White);
        }
        player_box.set_size(sf::Vector2f(button_width, button_height));
        player_box.set_position(sf::Vector2f(window_size.x / 2 - button_width / 2,
                                             window_size.y / 4 + i * (button_height + 10)));
        player_box.set_character_size(button_height / 2);
        player_box.render(window);
    }
}

std::unordered_map<std::string, sf::FloatRect> Online_Lobby_Renderer::get_layout_infos()
{
    std::unordered_map<std::string, sf::FloatRect> button_positions;
    button_positions["exit_button"]  = exit_button.get_rect();
    button_positions["start_button"] = start_button.get_rect();

    return button_positions;
}
}  // namespace renderer