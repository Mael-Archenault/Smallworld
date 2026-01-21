    #include "renderer.h"
#include "resources_dir.h"
#include "state.h"

namespace renderer
{
Opponents_Info_Renderer::Opponents_Info_Renderer(sf::RenderWindow& window, state::Game_State& state)
    : window(window), players(state.get_players())
{
    // Reserve space to avoid vector reallocation which invalidates font pointers
    int opponents_number = players.size() - 1;
    names.reserve(opponents_number);
    coins.reserve(opponents_number);
    active_tribes_renderers.reserve(opponents_number);
    decline_tribes_renderers.reserve(opponents_number);

    for (int i = 0; i < opponents_number; i++)
    {
        names.emplace_back();
        names.at(i).set_colors(sf::Color::White, sf::Color(50, 50, 50), sf::Color::White);//sf::Color(30, 30, 30));

        coins.emplace_back();
        coins.at(i).set_colors(sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0), sf::Color(30, 30, 30));

        active_tribes_renderers.emplace_back();
        decline_tribes_renderers.emplace_back();
    }
}

void Opponents_Info_Renderer::render_one_player_area(state::Player& player, int position)
{
    names.at(position).set_content(player.get_name());
    names.at(position).set_size(sf::Vector2f(section_width * 2 / 3, sub_section_height / 10));
    names.at(position).set_character_size(section_width / 10);
    names.at(position).set_position(this->position +
                                    sf::Vector2f(0.f, position * sub_section_height));
    names.at(position).render(window);

    coins.at(position).set_content("Money: " + std::to_string(player.get_money()));
    coins.at(position).set_size(sf::Vector2f(section_width * 1 / 3, sub_section_height / 10));
    coins.at(position).set_character_size(section_width / 15);
    coins.at(position).set_position(
        this->position + sf::Vector2f(section_width * 2 / 3, position * sub_section_height));

    coins.at(position).render(window);

    float active_tribe_scaling_factor =
        std::min(section_width / 500, (sub_section_height * 10 / 15) / 194);
    float decline_tribe_scaling_factor =
        std::min(section_width / (2 * 500), (sub_section_height * 5 / 15) / 194);

    std::pair<state::Tribe*, state::Tribe*> player_tribes = player.get_tribes();
    if (player_tribes.first != nullptr)
    {
        active_tribes_renderers.at(position).set_sprite(
            player.get_tribes().first->get_species_name(),
            player.get_tribes().first->get_power_name(), false);

        active_tribes_renderers.at(position).scale(active_tribe_scaling_factor,
                                                   active_tribe_scaling_factor);
        active_tribes_renderers.at(position).render(
            window, this->position +
                        sf::Vector2f(0.f, position * sub_section_height + sub_section_height / 10));
    }

    if (player_tribes.second != nullptr)
    {
        decline_tribes_renderers.at(position).set_sprite(
            player.get_tribes().second->get_species_name(),
            player.get_tribes().second->get_power_name(), true);

        decline_tribes_renderers.at(position).scale(decline_tribe_scaling_factor,
                                                    decline_tribe_scaling_factor);
        decline_tribes_renderers.at(position).render(
            window, this->position +
                        sf::Vector2f(0.f, position * sub_section_height + sub_section_height / 10 +
                                              active_tribe_scaling_factor * 194));
    }
}

void Opponents_Info_Renderer::render(state::Game_State& state, int rendering_player_id)
{
    position       = sf::Vector2f(4.f, window.getSize().y / 3 + 4);
    section_width  = window.getSize().x / 6.f;
    section_height = window.getSize().y * 2 / 3 - 4;

    sub_section_height = section_height / (players.size() - 1);

    int rendered_players = 0;
    for (auto& player : players)
    {
        if (player.id == rendering_player_id)
        {
            continue;
        }

        render_one_player_area(player, rendered_players);
        rendered_players++;
    }
}

std::unordered_map<std::string, sf::FloatRect> Opponents_Info_Renderer::get_layout()
{
    std::unordered_map<std::string, sf::FloatRect> layout_infos;

    layout_infos["opponents_info_area"] =
        sf::FloatRect(position.x, position.y, section_width, section_height);

    for (size_t i = 0; i < players.size() - 1; i++)
    {
        layout_infos["player_" + std::to_string(i) + "_active_tribe"] =
            active_tribes_renderers.at(i).get_rect();
        layout_infos["player_" + std::to_string(i) + "_in_decline_tribe"] =
            decline_tribes_renderers.at(i).get_rect();
    }

    return layout_infos;
}
}  // namespace renderer