#include "renderer.h"

namespace renderer
{

std::vector<std::string> biome_to_string = {"Hills", "Forests", "Mountains",
                                            "Swamp", "Water",   "Farm"};

std::vector<std::string> area_specialization_to_string = {"Mine", "Magic Region", "Cavern"};

std::vector<std::string> special_token_to_string = {
    "Troll Lair", "Fortress", "Mountain", "Encampment", "Hole in the Ground", "Hero", "Dragon"};

Area_Info_Renderer::Area_Info_Renderer(sf::RenderWindow& window) : window(window)
{
    int title_character_size = 32;
    int value_character_size = 18;

    float section_width  = window.getSize().x / 6.f;
    float section_height = window.getSize().y / 12.f;
    // area info section

    float area_info_width  = window.getSize().x / 6.f;
    float area_info_height = window.getSize().y * 2 / 6.f;

    float area_info_y_offset = 5 * section_height;

    area_info_title.set_content("Area Infos");
    area_info_title.set_colors(sf::Color::White, sf::Color(50, 50, 50), sf::Color::White);

    area_info_value.set_colors(sf::Color::White, sf::Color(50, 50, 50), sf::Color::White);
}

void Area_Info_Renderer::render(state::Game_State& state)
{
    float section_width  = window.getSize().x / 6.f - 4.f;
    float section_height = window.getSize().y / 12.f;

    float area_info_height = window.getSize().y * 2 / 6.f;
    float area_info_width  = window.getSize().x / 6.f - 4.f;

    float y = 3 * window.getSize().y / 6.f;
    float x = window.getSize().x * 5 / 6.f;

    area_info_title.set_position({x, y});
    area_info_title.set_size({area_info_width, section_height});
    area_info_title.set_character_size(area_info_width / 10);

    area_info_value.set_position({x, y + section_height});
    area_info_value.set_size({area_info_width, area_info_height - section_height});
    area_info_value.set_character_size(area_info_width / 15);
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
            area_info += "Owner: " + tribe->get_owner()->get_name() + "\n";
            area_info +=
                "Tribe: " + tribe->get_power_name() + " " + tribe->get_species_name() + "\n";
            std::string is_in_decline = tribe->get_is_in_decline() ? " Yes\n" : " No\n";
            area_info += "In decline:" + is_in_decline;
        }
    }

    area_info_value.set_content(area_info);
    area_info_value.render(window);
}

void Area_Info_Renderer::set_selected_area_id(int area_id)
{
    selected_area_id = area_id;
}

}  // namespace renderer