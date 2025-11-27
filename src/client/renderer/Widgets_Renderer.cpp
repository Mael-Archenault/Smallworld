#include "renderer.h"

namespace renderer
{
Widgets_Renderer::Widgets_Renderer(sf::RenderWindow& window) : window(window)
{
    int button_width  = 200;
    int button_height = 50;

    int font_size = 18;
    ;
    conquer_button.set_content("Conquer");
    conquer_button.set_character_size(font_size);
    conquer_button.set_position(sf::Vector2f(0.f, 0.f));
    conquer_button.set_size(sf::Vector2f(button_width, button_height));
    conquer_button.set_colors(sf::Color::White, sf::Color(50, 50, 50), sf::Color::White);

    end_conquests_button.set_content("End Conquests");
    end_conquests_button.set_character_size(font_size);
    end_conquests_button.set_position(sf::Vector2f(0.f, 0.f));
    end_conquests_button.set_size(sf::Vector2f(button_width, button_height));
    end_conquests_button.set_colors(sf::Color::White, sf::Color(50, 50, 50), sf::Color::White);

    decline_button.set_content("Decline");
    decline_button.set_character_size(font_size);
    decline_button.set_position(sf::Vector2f(0.f, 0.f));
    decline_button.set_size(sf::Vector2f(button_width, button_height));
    decline_button.set_colors(sf::Color::White, sf::Color(50, 50, 50), sf::Color::White);

    start_conquests_button.set_content("Start conquests");
    start_conquests_button.set_character_size(font_size);
    start_conquests_button.set_position(sf::Vector2f(0.f, 0.f));
    start_conquests_button.set_size(sf::Vector2f(button_width, button_height));
    start_conquests_button.set_colors(sf::Color::White, sf::Color(50, 50, 50), sf::Color::White);

    redeploy_button.set_content("Redeploy");
    redeploy_button.set_character_size(font_size);
    redeploy_button.set_position(sf::Vector2f(0.f, 0.f));
    redeploy_button.set_size(sf::Vector2f(button_width, button_height));
    redeploy_button.set_colors(sf::Color::White, sf::Color(50, 50, 50), sf::Color::White);
}

void Widgets_Renderer::render(state::Game_State& state)
{
    int section_width  = window.getSize().x / 6.f;
    int section_height = window.getSize().y / 6.f;

    int position_x = window.getSize().x - section_width;
    int position_y = window.getSize().y - section_height;

    state::Turn_Phase current_phase = state.get_current_turn_phase();

    if (current_phase == state::Turn_Phase::START)
    {
        start_conquests_button.set_size(
            sf::Vector2f(section_width * 4 / 6, 2 * section_height / 7));
        start_conquests_button.set_position(
            sf::Vector2f(position_x + section_width / 6, position_y + 1 * section_height / 7));

        start_conquests_button.render(window);

        decline_button.set_size(sf::Vector2f(section_width * 4 / 6, 2 * section_height / 7));
        decline_button.set_position(
            sf::Vector2f(position_x + section_width / 6, position_y + 4 * section_height / 7));

        decline_button.render(window);
    }

    if (current_phase == state::Turn_Phase::CONQUER)
    {
        conquer_button.set_size(sf::Vector2f(section_width * 4 / 6, 2 * section_height / 7));
        conquer_button.set_position(
            sf::Vector2f(position_x + section_width / 6, position_y + 1 * section_height / 7));

        conquer_button.render(window);

        end_conquests_button.set_size(sf::Vector2f(section_width * 4 / 6, 2 * section_height / 7));
        end_conquests_button.set_position(
            sf::Vector2f(position_x + section_width / 6, position_y + 4 * section_height / 7));

        end_conquests_button.render(window);
    }

    if (current_phase == state::Turn_Phase::REDEPLOY)
    {
        redeploy_button.set_size(sf::Vector2f(section_width * 4 / 6, section_height / 3));
        redeploy_button.set_position(
            sf::Vector2f(position_x + section_width / 6, position_y + section_height / 3));

        redeploy_button.render(window);
    }
}

std::unordered_map<std::string, sf::FloatRect> Widgets_Renderer::get_layout()
{
    std::unordered_map<std::string, sf::FloatRect> button_positions;
    button_positions["conquer_button"]         = conquer_button.get_rect();
    button_positions["decline_button"]         = decline_button.get_rect();
    button_positions["start_conquests_button"] = start_conquests_button.get_rect();
    button_positions["end_conquests_button"]   = end_conquests_button.get_rect();
    button_positions["redeploy_button"]        = redeploy_button.get_rect();
    return button_positions;
}

}  // namespace renderer