#include "renderer.h"
#include "state.h"

namespace renderer
{

std::vector<std::string> phase_type_to_string = {
    "Species Management",
    "Conquer",
    "Redeploy",
};
UI_Renderer::UI_Renderer(sf::RenderWindow& window) : window(window)

{
    int title_character_size = 32;
    int value_character_size = 24;

    int section_width  = window.getSize().x / 6.f;
    int section_height = window.getSize().y / 12.f;

    // player infos
    player_title.set_content("Player:");
    player_title.set_character_size(title_character_size);
    player_title.set_position(sf::Vector2f(0.f, 0.f));
    player_title.set_size(sf::Vector2f(section_width, section_height));

    player_value.set_character_size(value_character_size);
    player_value.set_position(sf::Vector2f(2.f, section_height));
    player_value.set_size(sf::Vector2f(section_width, section_height));
    player_value.set_colors(sf::Color::White, sf::Color(50, 50, 50), sf::Color::White);

    // turn phase infos
    turn_phase_title.set_content("Turn Phase:");
    turn_phase_title.set_character_size(title_character_size);
    turn_phase_title.set_position(sf::Vector2f(0.f, 2 * section_height));
    turn_phase_title.set_size(sf::Vector2f(section_width, section_height));

    turn_phase_value.set_character_size(value_character_size);
    turn_phase_value.set_position(sf::Vector2f(2.f, 3 * section_height));
    turn_phase_value.set_size(sf::Vector2f(section_width, section_height));
    turn_phase_value.set_colors(sf::Color::White, sf::Color(50, 50, 50), sf::Color::White);

    // area info section

    int area_info_height = window.getSize().y - 6 * section_height;
    int area_info_width  = window.getSize().x / 6.f;

    int area_info_y_offset = 5 * section_height;

    area_info_background.setSize(sf::Vector2f(area_info_width, area_info_height));
    area_info_background.setFillColor(sf::Color(20, 20, 20, 200));
    area_info_background.setPosition(sf::Vector2f(0.f, area_info_y_offset));

    area_info_title.set_content("Area Infos");
    area_info_title.set_character_size(title_character_size);
    area_info_title.set_position({2.f, area_info_y_offset});
    area_info_title.set_size({area_info_width, section_height});
    area_info_title.set_colors(sf::Color::White, sf::Color(50, 50, 50), sf::Color::White);

    // attack button
    attack_button.set_content("Attack");
    attack_button.set_character_size(24);
    attack_button.set_position(sf::Vector2f(0.f, window.getSize().y - section_height));
    attack_button.set_size(sf::Vector2f(section_width, section_height));
    attack_button.set_colors(sf::Color::White, sf::Color(100, 0, 0), sf::Color::White);
}

void UI_Renderer::render(state::Game_State& state)
{
    player_title.render(window);
    turn_phase_title.render(window);

    player_value.set_content(std::to_string(state.get_current_player().id));
    player_value.render(window);

    turn_phase_value.set_content(phase_type_to_string.at(state.get_current_turn_phase()));
    turn_phase_value.render(window);

    window.draw(area_info_background);
    area_info_title.render(window);

    // text = "Turn Phase: " + phase_type_to_string.at(state.get_current_turn_phase());
    // phase_indicator.set_content(text);
    // phase_indicator.render(window);
}
}  // namespace renderer