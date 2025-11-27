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
std::vector<std::string> biome_to_string = {"Hills", "Forests", "Mountains",
                                            "Swamp", "Water",   "Farm"};

std::vector<std::string> area_specialization_to_string = {"Mine", "Magic Region", "Cavern"};

std::vector<std::string> special_token_to_string = {
    "Troll Lair", "Fortress", "Mountain", "Encampment", "Hole in the Ground", "Hero", "Dragon"};

Sidebar_Renderer::Sidebar_Renderer(sf::RenderWindow& window) : window(window)

{
    int title_character_size = 32;
    int value_character_size = 18;

    float section_width  = window.getSize().x / 6.f;
    float section_height = window.getSize().y / 12.f;

    // player infos
    player_title.set_content("Player:");
    player_title.set_character_size(title_character_size);
    player_title.set_position(sf::Vector2f(0.f, 0.f));
    player_title.set_size(sf::Vector2f(section_width, section_height));
    player_title.set_colors(sf::Color::White, sf::Color(50, 50, 50), sf::Color::White);

    player_value.set_character_size(value_character_size);
    player_value.set_position(sf::Vector2f(2.f, section_height));
    player_value.set_size(sf::Vector2f(section_width, section_height));

    // turn phase infos
    turn_phase_title.set_content("Phase:");
    turn_phase_title.set_character_size(title_character_size);
    turn_phase_title.set_position(sf::Vector2f(0.f, 2 * section_height));
    turn_phase_title.set_size(sf::Vector2f(section_width, section_height));
    turn_phase_title.set_colors(sf::Color::White, sf::Color(50, 50, 50), sf::Color::White);

    turn_phase_value.set_character_size(value_character_size);
    turn_phase_value.set_position(sf::Vector2f(2.f, 3 * section_height));
    turn_phase_value.set_size(sf::Vector2f(section_width, section_height));

    // area info section

    set_selected_area_id(-1);
    float area_info_height = window.getSize().y - 6 * section_height;
    float area_info_width  = window.getSize().x / 6.f;

    float area_info_y_offset = 5 * section_height;

    area_info_title.set_content("Area Infos");
    area_info_title.set_character_size(title_character_size);
    area_info_title.set_position({2.f, area_info_y_offset});
    area_info_title.set_size({area_info_width, section_height});
    area_info_title.set_colors(sf::Color::White, sf::Color(50, 50, 50), sf::Color::White);

    area_info_value.set_character_size(value_character_size);
    area_info_value.set_position({2.f, area_info_y_offset + section_height});
    area_info_value.set_size({area_info_width - 4.f, area_info_height - section_height - 4.f});
    area_info_value.set_colors(sf::Color::White, sf::Color(50, 50, 50), sf::Color::White);

    // attack button
    attack_button.set_content("Attack");
    attack_button.set_character_size(24);
    attack_button.set_position(sf::Vector2f(0.f, window.getSize().y - section_height));
    attack_button.set_size(sf::Vector2f(section_width, section_height));
    attack_button.set_colors(sf::Color::White, sf::Color(100, 0, 0), sf::Color::White);
}

void Sidebar_Renderer::render(state::Game_State& state)

{
    float section_width  = window.getSize().x / 6.f;
    float section_height = window.getSize().y / 12.f;

    // player infos
    player_title.set_position(sf::Vector2f(0.f, 0.f));
    player_title.set_size(sf::Vector2f(section_width, section_height));

    player_value.set_position(sf::Vector2f(2.f, section_height));
    player_value.set_size(sf::Vector2f(section_width, section_height));

    // turn phase infos
    turn_phase_title.set_position(sf::Vector2f(0.f, 2 * section_height));
    turn_phase_title.set_size(sf::Vector2f(section_width, section_height));

    turn_phase_value.set_position(sf::Vector2f(2.f, 3 * section_height));
    turn_phase_value.set_size(sf::Vector2f(section_width, section_height));

    float area_info_height = window.getSize().y - 6 * section_height;
    float area_info_width  = window.getSize().x / 6.f;

    float area_info_y_offset = 5 * section_height;

    area_info_title.set_position({2.f, area_info_y_offset});
    area_info_title.set_size({area_info_width, section_height});

    area_info_value.set_position({2.f, area_info_y_offset + section_height});
    area_info_value.set_size({area_info_width - 4.f, area_info_height - section_height - 4.f});

    // player name
    player_title.render(window);
    turn_phase_title.render(window);

    player_value.set_content(std::to_string(state.get_current_player().id));
    player_value.render(window);

    // phase name
    turn_phase_value.set_content(phase_type_to_string.at(state.get_current_turn_phase()));
    turn_phase_value.render(window);

    // area info
    area_info_title.render(window);

    std::string area_info;

    if (selected_area_id == -1)
    {
        area_info = "No area selected";
    }
    else
    {
        state::Area& area = state.get_map().get_area(selected_area_id);
        area_info         = "Biome: " + biome_to_string.at(area.get_biome()) + "\n";
        area_info += "Units: " + std::to_string(area.get_units_number()) + "\n";
        area_info += "Specializations:\n";
        if (area.get_area_specialization().empty())
        {
            area_info += "None\n";
        }
        else
        {
            for (auto& spec : area.get_area_specialization())
            {
                area_info += "- " + area_specialization_to_string.at(spec) + "\n";
            }
        }

        area_info += "Special Tokens: ";

        if (area.get_special_tokens().empty())
        {
            area_info += "None\n";
        }
        else
        {
            for (auto& token : area.get_special_tokens())
            {
                area_info += "- " + special_token_to_string.at(token) + "\n";
            }
        }

        state::Tribe* tribe = area.get_owner_tribe();
        if (tribe != nullptr)
        {
            area_info += "Owner: " + std::to_string(tribe->get_owner()->id) + "\n";
            area_info +=
                "Tribe: " + tribe->get_power_name() + " " + tribe->get_species_name() + "\n";
            std::string is_in_decline = tribe->is_in_decline() ? " Yes\n" : " No\n";
            area_info += "In decline:" + is_in_decline;
        }
    }

    area_info_value.set_content(area_info);
    area_info_value.render(window);
}

void Sidebar_Renderer::set_selected_area_id(int area_id)
{
    selected_area_id = area_id;
}
}  // namespace renderer