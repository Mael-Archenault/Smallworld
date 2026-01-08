#include <json/json.h>

#include <fstream>

#include "renderer.h"
#include "resources_dir.h"
#include "state.h"

namespace renderer
{

std::vector<std::string> phase_type_to_string = {
    "Species Management",
    "Conquer",
    "Redeploy",
};

Turn_Info_Renderer::Turn_Info_Renderer(sf::RenderWindow& window) : window(window)

{
    int title_character_size = 32;
    int value_character_size = 18;

    section_width  = window.getSize().x / 6.f;
    section_height = window.getSize().y / 24.f;

    // player infos
    player_title.set_content("Player:");
    player_title.set_character_size(title_character_size);
    player_title.set_position(sf::Vector2f(4.f, 4.f));
    player_title.set_size(sf::Vector2f(section_width, section_height));
    player_title.set_colors(sf::Color::White, sf::Color(50, 50, 50), sf::Color::White);

    player_value.set_character_size(value_character_size);
    player_value.set_position(sf::Vector2f(4.f, section_height + 4.f));
    player_value.set_size(sf::Vector2f(section_width, section_height));

    // turn phase infos
    turn_phase_title.set_content("Phase:");
    turn_phase_title.set_character_size(title_character_size);
    turn_phase_title.set_position(sf::Vector2f(4.f, 2 * section_height + 4.f));
    turn_phase_title.set_size(sf::Vector2f(section_width, section_height));
    turn_phase_title.set_colors(sf::Color::White, sf::Color(50, 50, 50), sf::Color::White);

    turn_phase_value.set_character_size(value_character_size);
    turn_phase_value.set_position(sf::Vector2f(4.f, 3 * section_height + 4.f));
    turn_phase_value.set_size(sf::Vector2f(section_width, section_height));

    // attack button
    attack_button.set_content("Attack");
    attack_button.set_character_size(24);
    attack_button.set_position(sf::Vector2f(0.f, window.getSize().y - section_height));
    attack_button.set_size(sf::Vector2f(section_width, section_height));
    attack_button.set_colors(sf::Color::White, sf::Color(100, 0, 0), sf::Color::White);
}

void Turn_Info_Renderer::render(state::Game_State& state)

{
    section_width  = window.getSize().x / 6.f;
    section_height = window.getSize().y / 24.f;

    // player infos
    player_title.set_position(sf::Vector2f(4.f, 4.f));
    player_title.set_size(sf::Vector2f(section_width, section_height));
    player_title.set_character_size(section_width / 15);

    player_value.set_position(sf::Vector2f(4.f, section_height + 4.f));
    player_value.set_size(sf::Vector2f(section_width, section_height));
    player_value.set_character_size(section_width / 20);

    // turn phase infos
    turn_phase_title.set_position(sf::Vector2f(4.f, 2 * section_height + 4.f));
    turn_phase_title.set_size(sf::Vector2f(section_width, section_height));
    turn_phase_title.set_character_size(section_width / 15);

    turn_phase_value.set_position(sf::Vector2f(4.f, 3 * section_height + 4.f));
    turn_phase_value.set_size(sf::Vector2f(section_width, section_height));
    turn_phase_value.set_character_size(section_width / 20);

    // player name
    player_title.render(window);
    turn_phase_title.render(window);

    player_value.set_content(state.get_current_player().get_name());
    player_value.render(window);

    // phase name
    turn_phase_value.set_content(phase_type_to_string.at(state.get_current_turn_phase()));
    turn_phase_value.render(window);
}

}  // namespace renderer