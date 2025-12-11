#include "renderer.h"
#include "resources_dir.h"
#include "state.h"

namespace renderer
{
Opponents_Info_Renderer::Opponents_Info_Renderer(sf::RenderWindow& window, state::Game_State& state)
    : window(window), players(state.get_players())
{
    // Reserve space to avoid vector reallocation which invalidates font pointers
    names.reserve(players.size());
    coins.reserve(players.size());
    int i = 0;
    for (auto& player : players)
    {
        names.emplace_back();
        names.at(i).set_colors(sf::Color::White, sf::Color(50, 50, 50), sf::Color::White);

        coins.emplace_back();
        coins.at(i).set_colors(sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0), sf::Color::White);
        i++;
    }
}

void Opponents_Info_Renderer::render_one_player_area(state::Player& player, int position)
{
    names.at(position).set_content(player.get_name());
    names.at(position).set_size(sf::Vector2f(section_width * 2 / 3, sub_section_height / 10));
    names.at(position).set_character_size(sub_section_height / 10);
    names.at(position).set_position(this->position +
                                    sf::Vector2f(0.f, position * sub_section_height));
    names.at(position).render(window);

    coins.at(position).set_content("Money: " + std::to_string(player.get_money()));
    coins.at(position).set_size(sf::Vector2f(section_width * 1 / 3, sub_section_height / 10));
    coins.at(position).set_character_size(sub_section_height / 15);
    coins.at(position).set_position(
        this->position + sf::Vector2f(section_width * 2 / 3, position * sub_section_height));

    coins.at(position).render(window);
}

void Opponents_Info_Renderer::render(state::Game_State& state)
{
    position       = sf::Vector2f(4.f, window.getSize().y * 1 / 6);
    section_width  = window.getSize().x / 6.f;
    section_height = window.getSize().y * (7 / 8.f - 1 / 6.f);

    sub_section_height = section_height / (players.size() - 1);

    int rendered_players = 0;
    for (auto& player : players)
    {
        if (player.id == state.get_current_player().id)
        {
            continue;
        }

        render_one_player_area(player, rendered_players);
        rendered_players++;
    }
}
}  // namespace renderer