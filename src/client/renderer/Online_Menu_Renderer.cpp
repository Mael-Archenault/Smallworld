#include "renderer.h"
#include "resources_dir.h"

namespace renderer
{
Online_Menu_Renderer::Online_Menu_Renderer(sf::RenderWindow& window) : window(window)
{
    // load background image
    std::string file_path = std::string(RESOURCE_DIR) + "/assets/general_background_blurred.png";
    if (!background_texture.loadFromFile(file_path))
    {
        throw std::runtime_error("Menu_Renderer constructor: Failed to load texture :" + file_path);
    }
    background_sprite.setTexture(background_texture);

    create_button.set_content("Create room");
    create_button.set_colors(sf::Color::White, sf::Color(50, 50, 50), sf::Color::White);

    join_button.set_content("Join room");
    join_button.set_colors(sf::Color::White, sf::Color(50, 50, 50), sf::Color::White);

    room_id_box.set_content("");
    room_id_box.set_colors(sf::Color(50, 50, 50), sf::Color(250, 250, 250), sf::Color(50, 50, 50));

    name_label.set_content("Name :");
    name_label.set_colors(sf::Color::White, sf::Color(50, 50, 50), sf::Color::White);

    name_box.set_content("");
    name_box.set_colors(sf::Color(50, 50, 50), sf::Color(250, 250, 250), sf::Color(50, 50, 50));
}

void Online_Menu_Renderer::render(bool modifying_room_id, std::string room_id, bool modifying_name,
                                  std::string name)
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

    create_button.set_size(sf::Vector2f(button_width, button_height));
    create_button.set_position(
        sf::Vector2f(window_size.x / 2 - button_width / 2, window_size.y * 2 / 5));
    create_button.set_character_size(button_width / 10);
    create_button.render(window);

    join_button.set_size(sf::Vector2f(button_width, button_height));
    join_button.set_position(
        sf::Vector2f(window_size.x / 2 - button_width / 2, window_size.y * 3 / 5));
    join_button.set_character_size(button_width / 10);
    join_button.render(window);

    room_id_box.set_content(room_id);
    if (modifying_room_id)
    {
        room_id_box.set_colors(sf::Color(50, 50, 50), sf::Color(150, 150, 150),
                               sf::Color(50, 50, 50));
    }
    else
    {
        room_id_box.set_colors(sf::Color(50, 50, 50), sf::Color(250, 250, 250),
                               sf::Color(50, 50, 50));
    }
    room_id_box.set_size(sf::Vector2f(button_width / 2, button_height));
    room_id_box.set_position(
        sf::Vector2f(window_size.x / 2 + button_width / 2, window_size.y * 3 / 5));
    room_id_box.set_character_size(button_width / 10);
    room_id_box.render(window);

    name_label.set_size(sf::Vector2f(button_width, button_height));
    name_label.set_position(sf::Vector2f(5, 5));
    name_label.set_character_size(button_width / 10);
    name_label.render(window);

    name_box.set_content(name);
    if (modifying_name)
    {
        name_box.set_colors(sf::Color(50, 50, 50), sf::Color(150, 150, 150), sf::Color(50, 50, 50));
    }
    else
    {
        name_box.set_colors(sf::Color(50, 50, 50), sf::Color(250, 250, 250), sf::Color(50, 50, 50));
    }
    name_box.set_size(sf::Vector2f(button_width * 1.5, button_height));
    name_box.set_position(sf::Vector2f(button_width + 5, 5));
    name_box.set_character_size(button_width / 10);
    name_box.render(window);
}

std::unordered_map<std::string, sf::FloatRect> Online_Menu_Renderer::get_layout_infos()
{
    std::unordered_map<std::string, sf::FloatRect> button_positions;
    button_positions["create_button"] = create_button.get_rect();
    button_positions["join_button"]   = join_button.get_rect();
    button_positions["room_id_box"]   = room_id_box.get_rect();
    button_positions["name_box"]      = name_box.get_rect();

    return button_positions;
}
}  // namespace renderer