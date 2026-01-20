#include "renderer.h"
#include "resources_dir.h"

namespace renderer
{
Online_Lobby_Renderer::Online_Lobby_Renderer(sf::RenderWindow& window, std::string room_id)
    : window(window), ai_adder_window(window, false)
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
    exit_button.set_colors(sf::Color(110, 40, 0), sf::Color(50, 50, 50), sf::Color::White);

    start_button.set_content("Start Game");
    start_button.set_colors(sf::Color::Black, sf::Color(50, 50, 50), sf::Color::White);

    add_ai_button.set_content("Add AI");
    add_ai_button.set_colors(sf::Color::Black, sf::Color(50, 50, 50), sf::Color::White);

    remove_ai_button.set_content("Remove AI");
    remove_ai_button.set_colors(sf::Color::Black, sf::Color(50, 50, 50), sf::Color::White);
}

void Online_Lobby_Renderer::render(std::vector<std::string>        player_names,
                                   std::vector<state::Player_Type> player_types, bool show_adder,
                                   bool modifying_name, std::string name,
                                   state::Player_Type selected_player_type)
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
    exit_button.set_position(sf::Vector2f(window_size.x / 6 - button_width / 4,
                                          window_size.y * 9 / 10 - button_height / 2));
    exit_button.set_character_size(button_height / 3);
    exit_button.render(window);

    start_button.set_size(sf::Vector2f(button_width / 2, button_height));
    start_button.set_position(sf::Vector2f(window_size.x * 5 / 6 - button_width / 4,
                                           window_size.y * 9 / 10 - button_height / 2));
    start_button.set_character_size(button_height / 3);
    start_button.render(window);

    add_ai_button.set_size(sf::Vector2f(button_width / 2, button_height));
    add_ai_button.set_position(sf::Vector2f(window_size.x * 9 / 10 - button_width / 4,
                                            window_size.y / 2 - button_height / 2));
    add_ai_button.set_character_size(button_height / 3);
    add_ai_button.render(window);

    remove_ai_button.set_size(sf::Vector2f(button_width / 2, button_height));
    remove_ai_button.set_position(sf::Vector2f(window_size.x * 9 / 10 - button_width / 4,
                                               window_size.y * 5 / 8 - button_height / 2));
    remove_ai_button.set_character_size(button_height / 3);
    remove_ai_button.render(window);

    for (size_t i = 0; i < player_names.size(); ++i)
    {
        Text_Box    player_box;
        std::string player_type;
        switch (player_types.at(i))
        {
            case 0:
                player_type = " (Human)";
                break;
            case 1:
                player_type = " (Random AI)";
                break;
            case 2:
                player_type = " (Heuristic AI)";
                break;
            case 3:
                player_type = " (Advanced AI)";
                break;
            default:
                player_type = " (Unknown)";
                break;
        }
        player_box.set_content(player_names.at(i) + player_type);

        player_box.set_colors(sf::Color::White, sf::Color(50, 50, 50), sf::Color::White);

        player_box.set_size(sf::Vector2f(button_width, button_height));
        player_box.set_position(sf::Vector2f(window_size.x / 2 - button_width / 2,
                                             window_size.y / 4 + i * (button_height + 10)));
        player_box.set_character_size(button_height / 2);
        player_box.render(window);
    }

    if (show_adder)
    {
        ai_adder_window.render(modifying_name, name, selected_player_type);
    }
}

std::unordered_map<std::string, sf::FloatRect> Online_Lobby_Renderer::get_layout_infos()
{
    std::unordered_map<std::string, sf::FloatRect> button_positions;
    button_positions["exit_button"]      = exit_button.get_rect();
    button_positions["start_button"]     = start_button.get_rect();
    button_positions["add_ai_button"]    = add_ai_button.get_rect();
    button_positions["remove_ai_button"] = remove_ai_button.get_rect();
    for (const auto& [key, value] : ai_adder_window.get_layout_infos())
    {
        button_positions[key] = value;
    }

    return button_positions;
}
}  // namespace renderer