#include "ai.h"
#include "renderer.h"
#include "resources_dir.h"

namespace renderer
{
Local_Lobby_Renderer::Local_Lobby_Renderer(sf::RenderWindow& window)
    : window(window), player_adder_window(window, true)
{
    // load background image
    std::string file_path = std::string(RESOURCE_DIR) + "/assets/general_background_blurred.png";
    if (!background_texture.loadFromFile(file_path))
    {
        throw std::runtime_error("Online_Lobby_Renderer constructor: Failed to load texture :" +
                                 file_path);
    }
    background_sprite.setTexture(background_texture);

    file_path = std::string(RESOURCE_DIR) + "/assets/plus.png";
    if (!add_button_texture.loadFromFile(file_path))
    {
        throw std::runtime_error("Local_Lobby_Renderer constructor: Failed to load texture :" +
                                 file_path);
    }
    add_button_sprite.setTexture(add_button_texture);

    remove_player_button.set_content("Remove Player");
    remove_player_button.set_colors(sf::Color::Black, sf::Color(50, 50, 50), sf::Color::White);

    exit_button.set_content("Exit");
    exit_button.set_colors(sf::Color::Black, sf::Color(50, 50, 50), sf::Color::White);

    start_button.set_content("Start Game");
    start_button.set_colors(sf::Color::Black, sf::Color(50, 50, 50), sf::Color::White);
}

void Local_Lobby_Renderer::render(std::vector<std::string>        player_names,
                                  std::vector<state::Player_Type> player_types,
                                  bool show_player_adder, bool modifying_name, std::string name,
                                  state::Player_Type selected_player_type)
{
    window.clear(sf::Color::Black);

    sf::Vector2u window_size   = window.getSize();
    float        button_width  = window_size.x / 4.f;
    float        button_height = window_size.y / 12.f;

    // map
    float scaling_factor = std::min(((float) window_size.x / background_texture.getSize().x),
                                    ((float) window_size.y / background_texture.getSize().y));
    background_sprite.setScale(scaling_factor, scaling_factor);
    sf::Vector2f map_position =
        sf::Vector2f(((float) window_size.x - background_sprite.getGlobalBounds().width) / 2,
                     ((float) window_size.y - background_sprite.getGlobalBounds().height) / 2);
    background_sprite.setPosition(map_position);
    window.draw(background_sprite);

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

    if (player_names.size() < 4)
    {
        add_button_sprite.setScale(button_height / add_button_texture.getSize().y,
                                   button_height / add_button_texture.getSize().y);
        add_button_sprite.setPosition(
            sf::Vector2f(window_size.x / 2 - button_height / 2,
                         window_size.y / 4 + player_names.size() * (button_height + 10)));
        window.draw(add_button_sprite);
    }
    else
    {
        // hiding the button
        add_button_sprite.setPosition(sf::Vector2f(-button_height, -button_height));
    }

    if (player_names.size() >= 1)
    {
        remove_player_button.set_size(sf::Vector2f(button_width / 2, button_height));
        remove_player_button.set_position(sf::Vector2f(window_size.x * 9 / 10 - button_width / 4,
                                                       window_size.y / 2 - button_height / 2));
        remove_player_button.set_character_size(button_height / 3);
        remove_player_button.render(window);
    }
    else
    {
        remove_player_button.set_size(sf::Vector2f(0.f, 0.f));
    }

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

    if (show_player_adder)
    {
        player_adder_window.render(modifying_name, name, selected_player_type);
    }
}

std::unordered_map<std::string, sf::FloatRect> Local_Lobby_Renderer::get_layout_infos()
{
    std::unordered_map<std::string, sf::FloatRect> button_positions;
    button_positions["exit_button"]          = exit_button.get_rect();
    button_positions["start_button"]         = start_button.get_rect();
    button_positions["add_button"]           = add_button_sprite.getGlobalBounds();
    button_positions["remove_player_button"] = remove_player_button.get_rect();

    for (const auto& pair : player_adder_window.get_layout_infos())
    {
        button_positions[pair.first] = pair.second;
    }

    return button_positions;
}
}  // namespace renderer