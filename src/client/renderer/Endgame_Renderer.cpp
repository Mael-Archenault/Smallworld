#include "renderer.h"
#include "resources_dir.h"

namespace renderer
{

Endgame_Renderer::Endgame_Renderer(sf::RenderWindow& window) : window(window)
{
    // load background image
    std::string file_path = std::string(RESOURCE_DIR) + "/assets/general_background_blurred.png";
    if (!background_texture.loadFromFile(file_path))
    {
        throw std::runtime_error("Online_Lobby_Renderer constructor: Failed to load texture :" +
                                 file_path);
    }
    background_sprite.setTexture(background_texture);

    game_finished_label.set_content("Game Finished");
    game_finished_label.set_colors(sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0),
                                   sf::Color(30, 30, 30));

    exit_button.set_content("Exit");
    exit_button.set_colors(sf::Color::Black, sf::Color(50, 50, 50), sf::Color::White);
}

void Endgame_Renderer::render(std::vector<std::pair<std::string, int>> players)
{
    window.clear(sf::Color::Black);

    sf::Vector2u window_size   = window.getSize();
    float        button_width  = window_size.x / 4.f;
    float        button_height = window_size.y / 12.f;

    float scaling_factor = std::max(((float) window_size.x / background_texture.getSize().x),
                                    ((float) window_size.y / background_texture.getSize().y));
    background_sprite.setScale(scaling_factor, scaling_factor);
    sf::Vector2f map_position =
        sf::Vector2f(((float) window_size.x - background_sprite.getGlobalBounds().width) / 2,
                     ((float) window_size.y - background_sprite.getGlobalBounds().height) / 2);
    background_sprite.setPosition(map_position);
    window.draw(background_sprite);

    float title_width  = window_size.x / 2;
    float title_height = window_size.y / 5;
    game_finished_label.set_size(sf::Vector2f(title_width, title_height));
    game_finished_label.set_position(
        sf::Vector2f(window_size.x / 2 - title_width / 2, window_size.y / 10 - title_height / 2));
    game_finished_label.set_character_size(title_height / 3);
    game_finished_label.render(window);

    exit_button.set_size(sf::Vector2f(button_width / 2, button_height));
    exit_button.set_position(
        sf::Vector2f(window_size.x - button_width / 2, window_size.y - button_height));
    exit_button.set_character_size(button_height / 3);
    exit_button.render(window);

    float entry_width  = window_size.x / 2;
    float entry_height = window_size.y / 10;
    for (size_t i = 0; i < players.size(); ++i)
    {
        Text_Box    player_box;
        std::string player_type;
        player_box.set_content(players.at(i).first + " | Rank : " + std::to_string(i + 1) +
                               " | Money : " + std::to_string(players.at(i).second));

        player_box.set_colors(sf::Color::White, sf::Color(50, 50, 50), sf::Color::White);

        player_box.set_size(sf::Vector2f(entry_width, entry_height));
        player_box.set_position(sf::Vector2f(window_size.x / 2 - entry_width / 2,
                                             window_size.y / 4 + i * (entry_height + 10)));
        player_box.set_character_size(entry_height / 3);
        player_box.render(window);
    }
}

std::unordered_map<std::string, sf::FloatRect> Endgame_Renderer::get_layout_infos()
{
    std::unordered_map<std::string, sf::FloatRect> layout_infos;

    layout_infos["exit_button"] = exit_button.get_rect();

    return layout_infos;
}
}  // namespace renderer