#include <json/json.h>

#include <fstream>
#include <iostream>

#include "renderer.h"
#include "resources_dir.h"

namespace renderer
{
Tribe_Info_Window::Tribe_Info_Window(sf::RenderWindow& window) : window(window)
{
    background.setFillColor(sf::Color(50, 50, 50));

    if (!font.loadFromFile(std::string(RESOURCE_DIR) + "/fonts/arial.ttf"))
    {
        throw std::runtime_error("Cannot load font arial.ttf");
    }
    // Initialize attributes
    is_visible = false;
    // Initialize texts

    power_title.set_content("None");
    power_title.set_colors(sf::Color::White, sf::Color(50, 50, 50), sf::Color::White);

    species_title.set_content("None");
    species_title.set_colors(sf::Color::White, sf::Color(50, 50, 50), sf::Color::White);

    power_content.set_content("None");
    power_content.set_colors(sf::Color::Transparent, sf::Color::Transparent, sf::Color::White);

    species_content.set_content("None");
    species_content.set_colors(sf::Color::Transparent, sf::Color::Transparent, sf::Color::White);

    // Load descriptions

    open_descriptions();

    close_window_button.set_content("Close info window");
    close_window_button.set_colors(sf::Color::White, sf::Color(100, 100, 100), sf::Color::White);

    buy_tribe_button.set_content("Buy this tribe");
    buy_tribe_button.set_colors(sf::Color::White, sf::Color(100, 100, 100), sf::Color::White);
}

void Tribe_Info_Window::set_visibility(bool new_visibility)
{
    is_visible = new_visibility;
}

void Tribe_Info_Window::open_descriptions()
{
    std::string   file_name = std::string(RESOURCE_DIR) + "/config/effects_descriptions.json";
    std::ifstream file(file_name, std::ifstream::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file: " + file_name);
    }

    Json::Value root;
    file >> root;
    file.close();

    Json::Value species = root["species"];
    Json::Value powers  = root["powers"];

    for (const auto& species_name : species.getMemberNames())
    {
        species_descriptions[species_name] = species[species_name].asString();
    }
    for (const auto& power_name : powers.getMemberNames())
    {
        power_descriptions[power_name] = powers[power_name].asString();
    }
}

void Tribe_Info_Window::set_values(state::Tribe& tribe)
{
    std::string species_name = tribe.get_species_name();
    std::string power_name   = tribe.get_power_name();

    power_title.set_content(power_name);
    species_title.set_content(species_name);

    auto power_desc_it = power_descriptions.find(power_name);
    if (power_desc_it != power_descriptions.end())
    {
        power_content.set_content(power_desc_it->second);
    }
    else
    {
        power_content.set_content("Description not found.");
    }

    auto species_desc_it = species_descriptions.find(species_name);
    if (species_desc_it != species_descriptions.end())
    {
        species_content.set_content(species_desc_it->second);
    }
    else
    {
        species_content.set_content("Description not found.");
    }
}

void Tribe_Info_Window::render(state::Game_State& state)
{
    if (!is_visible) return;

    // Draw background
    background.setSize(sf::Vector2f(window.getSize().x * 0.8, window.getSize().y * 0.8));
    background.setPosition(window.getSize().x * 0.1, window.getSize().y * 0.1);
    window.draw(background);

    // Draw texts
    float section_width  = background.getSize().x / 2.f;
    float section_height = background.getSize().y;

    int title_font_size   = window.getSize().y / 20;
    int content_font_size = window.getSize().y / 30;

    power_title.set_position(sf::Vector2f(background.getPosition().x, background.getPosition().y));
    power_title.set_size(sf::Vector2f(section_width, section_height * 0.2));
    power_title.set_character_size(title_font_size);

    power_content.set_position(sf::Vector2f(background.getPosition().x,
                                            background.getPosition().y + section_height * 0.2));
    power_content.set_size(sf::Vector2f(section_width, section_height * 0.6));
    power_content.set_character_size(content_font_size);

    power_title.render(window);
    power_content.render(window);

    species_title.set_position(
        sf::Vector2f(background.getPosition().x + section_width, background.getPosition().y));
    species_title.set_size(sf::Vector2f(section_width, section_height * 0.2));
    species_title.set_character_size(title_font_size);

    species_content.set_position(sf::Vector2f(background.getPosition().x + section_width,
                                              background.getPosition().y + section_height * 0.2));
    species_content.set_size(sf::Vector2f(section_width, section_height * 0.6));
    species_content.set_character_size(content_font_size);

    species_title.render(window);
    species_content.render(window);

    close_window_button.set_size(
        sf::Vector2f(background.getSize().x * 0.3, background.getSize().y * 0.1));
    close_window_button.set_character_size(content_font_size);
    close_window_button.set_position(
        sf::Vector2f(background.getPosition().x + background.getSize().x * 0.1,
                     background.getPosition().y + background.getSize().y * 0.85));

    close_window_button.render(window);
    if (state.get_current_turn_phase() == state::Turn_Phase::START &&
        state.get_current_player().get_tribes().first == nullptr)
    {
        buy_tribe_button.set_size(
            sf::Vector2f(background.getSize().x * 0.3, background.getSize().y * 0.1));
        buy_tribe_button.set_character_size(content_font_size);
        buy_tribe_button.set_position(
            sf::Vector2f(background.getPosition().x + background.getSize().x * 0.6,
                         background.getPosition().y + background.getSize().y * 0.85));
        buy_tribe_button.render(window);
    }
}

std::unordered_map<std::string, sf::FloatRect> Tribe_Info_Window::get_layout()
{
    std::unordered_map<std::string, sf::FloatRect> layout;
    layout["tribe_info_window"]              = background.getGlobalBounds();
    layout["tribe_info_window_close_button"] = close_window_button.get_rect();
    layout["tribe_info_window_buy_button"]   = buy_tribe_button.get_rect();
    return layout;
}
}  // namespace renderer