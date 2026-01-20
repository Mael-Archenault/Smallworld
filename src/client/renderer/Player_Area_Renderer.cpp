#include "Player_Area_Renderer.h"

#include "resources_dir.h"

namespace renderer
{

/// Constructor
Player_Area_Renderer::Player_Area_Renderer(sf::RenderWindow& window)
    : window(window), angle(0), free_units_renderer("pawn")
{
    // Load font
    if (!font.loadFromFile(std::string(RESOURCE_DIR) + "/fonts/arial.ttf"))
    {
        throw std::runtime_error("Player_Area_Renderer constructor: Failed to load font");
    }

    // Setup money title text
    money_title.setFont(font);
    money_title.setString("Money:");
    money_title.setCharacterSize(14);
    money_title.setFillColor(sf::Color::White);

    // Setup money value text
    money_value.setFont(font);
    money_value.setCharacterSize(14);
    money_value.setFillColor(sf::Color::Yellow);

    // Setup free units title text
    free_units_title.setFont(font);
    free_units_title.setString("Free Units:");
    free_units_title.setCharacterSize(14);
    free_units_title.setFillColor(sf::Color::White);

    tribes_title.setFont(font);
    tribes_title.setString("Tribes:");
    tribes_title.setCharacterSize(14);
    tribes_title.setFillColor(sf::Color::White);

    tribes_rects.resize(3);
}

void Player_Area_Renderer::render(state::Player& player)
{
    sf::Vector2u window_size = window.getSize();

    float section_width  = window_size.x * 7 / 12;
    float section_height = window_size.y / 6;

    sf::Vector2f position =
        sf::Vector2f(window_size.x / 2 - section_width / 2, window_size.y - section_height);
    // draw a rectangle
    sf::RectangleShape background(sf::Vector2f(section_width, section_height));

    background.setFillColor(sf::Color(20, 20, 20));
    background.setPosition(position);
    window.draw(background);

    float header_size = section_height / 4;
    float body_size   = section_height - header_size;

    // Titles in the header
    money_title.setCharacterSize(header_size / 2);
    money_title.setPosition(
        position.x + section_width / 8 - money_title.getGlobalBounds().width / 2,
        position.y + header_size / 2 - money_title.getGlobalBounds().height / 2);
    window.draw(money_title);
    free_units_title.setCharacterSize(header_size / 2);
    free_units_title.setPosition(
        position.x + section_width / 4 + section_width / 8 -
            free_units_title.getGlobalBounds().width / 2,
        position.y + header_size / 2 - free_units_title.getGlobalBounds().height / 2);
    window.draw(free_units_title);
    tribes_title.setCharacterSize(header_size / 2);
    tribes_title.setPosition(
        position.x + section_width * 3 / 4 - tribes_title.getGlobalBounds().width / 2,
        position.y + header_size / 2 - tribes_title.getGlobalBounds().height / 2);
    window.draw(tribes_title);

    // Money value

    money_value.setString(std::to_string(player.get_money()));
    money_value.setPosition(
        position.x + section_width / 8 - money_value.getGlobalBounds().width / 2,
        position.y + header_size + body_size / 2 - money_value.getGlobalBounds().height / 2);
    window.draw(money_value);

    // Free units
    std::pair<state::Tribe*, state::Tribe*> player_tribes = player.get_tribes();

    if (player_tribes.first != nullptr)
    {
        int   free_units_number = player.get_free_units_number();
        float scaling_factor    = std::min((section_width / 5) / 150, body_size / 150);
        free_units_renderer.scale_token(scaling_factor, scaling_factor);
        free_units_renderer.scale_text(scaling_factor / 2, scaling_factor / 2);
        free_units_renderer.set_sprite(player_tribes.first->get_species_name());
        free_units_renderer.set_number(free_units_number);
        free_units_renderer.render(
            window,
            sf::Vector2f(
                position.x + section_width / 4 + section_width / 8 - 150 * scaling_factor / 2,
                position.y + header_size + body_size / 2 - 75 * scaling_factor),
            true);
    }

    // Render tribe_renderers

    if (player_tribes.first != nullptr)
    {
        float scaling_factor = std::min((section_width / 4) / 500, body_size / 194);
        tribe_renderer.set_sprite(player_tribes.first->get_species_name(),
                                  player_tribes.first->get_power_name(), false);
        tribe_renderer.scale(scaling_factor, scaling_factor);
        tribe_renderer.render(
            window,
            sf::Vector2f(
                position.x + section_width / 2 + section_width / 8 - (500 * scaling_factor) / 2,
                position.y + header_size + body_size / 2 - (194 * scaling_factor) / 2));

        tribes_rects.at(0) = tribe_renderer.get_rect();
    }

    if (player_tribes.second != nullptr)
    {
        float scaling_factor = std::min((section_width / 4) / 500, (body_size * 2 / 3) / 194);
        tribe_renderer.set_sprite(player_tribes.second->get_species_name(),
                                  player_tribes.second->get_power_name(), true);
        tribe_renderer.scale(scaling_factor, scaling_factor);
        tribe_renderer.render(
            window,
            sf::Vector2f(
                position.x + section_width * 3 / 4 + section_width / 8 - (500 * scaling_factor) / 2,
                position.y + header_size + body_size / 3 - (194 * scaling_factor) / 2));
        tribes_rects.at(1) = tribe_renderer.get_rect();
    }
}

std::unordered_map<std::string, sf::FloatRect> Player_Area_Renderer::get_layout()
{
    std::unordered_map<std::string, sf::FloatRect> layout_infos;
    layout_infos["player_area"]      = background.getGlobalBounds();
    layout_infos["active_tribe"]     = tribes_rects.at(0);
    layout_infos["in_decline_tribe"] = tribes_rects.at(1);
    return layout_infos;
}

}  // namespace renderer