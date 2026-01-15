//
// Created by julien on 12/19/25.
//
#include <cmath>
#include <iostream>

#include "client.h"
#include "engine.h"
#include "renderer.h"

namespace client
{
Click_Handler_Multithread::Click_Handler_Multithread(Client_Multithread& client)
    : client(client),
      state(client.get_state()),
      renderer(client.get_renderer()),
      engine(client.get_engine())
{
}

void Click_Handler_Multithread::handle_click(sf::Vector2i position)
{
    std::unordered_map<std::string, sf::FloatRect> layout_infos = renderer.get_layout_infos();

    // tribe window management
    if (client.get_tribe_info_window_state())
    {
        if (layout_infos["tribe_info_window"].contains(static_cast<sf::Vector2f>(position)))
        {
            handle_info_window_click(position, layout_infos);
            return;
        }
        else
        {
            renderer.close_tribe_info_window();
            client.set_tribe_info_window_state(false);
            return;
        }
    }
    // click on map
    if (layout_infos["map"].contains(static_cast<sf::Vector2f>(position)))
    {
        handle_map_click(position, layout_infos["map"]);
        return;
    }

    // click on the tribe stack
    if (layout_infos["tribe_stack"].contains(static_cast<sf::Vector2f>(position)))
    {
        handle_tribe_stack_click(position, layout_infos["tribe_stack"]);
        return;
    }

    if (layout_infos["player_area"].contains(static_cast<sf::Vector2f>(position)))
    {
        handle_player_area_click(position, layout_infos);
        return;
    }

    if (layout_infos["buttons_area"].contains(static_cast<sf::Vector2f>(position)))
    {
        handle_button_area_click(position, layout_infos);
        return;
    }

    if (layout_infos["opponents_info_area"].contains(static_cast<sf::Vector2f>(position)))
    {
        handle_opponent_area_click(position, layout_infos);
        return;
    }
}

void Click_Handler_Multithread::handle_map_click(sf::Vector2i position, sf::FloatRect map_layout)
{
    std::vector<sf::Vector2f> area_positions = renderer.get_on_screen_area_positions();

    std::vector<float> distances;
    for (size_t i = 0; i < area_positions.size(); i++)
    {
        float distance = std::sqrt(std::pow(area_positions.at(i).x - position.x, 2) +
                                   std::pow(area_positions.at(i).y - position.y, 2));

        distances.push_back(distance);
    }

    // find the index of the minimum distance
    size_t min_index = 0;
    for (size_t i = 1; i < distances.size(); i++)
    {
        if (distances.at(i) < distances.at(min_index)) min_index = i;
    }

    renderer.set_selected_area(static_cast<int>(min_index));
    client.set_selected_area_id(static_cast<int>(min_index));
}

void Click_Handler_Multithread::handle_tribe_stack_click(sf::Vector2i  position,
                                                         sf::FloatRect tribe_stack_layout)
{
    state::Player&            actioning_player = state.get_players().at(client.get_player_id());
    std::vector<sf::Vector2f> card_positions   = renderer.get_on_screen_tribe_positions();

    std::vector<float> distances;
    for (size_t i = 0; i < card_positions.size(); i++)
    {
        float distance = std::sqrt(std::pow(card_positions.at(i).x - position.x, 2) +
                                   std::pow(card_positions.at(i).y - position.y, 2));

        distances.push_back(distance);
    }

    // find the index of the minimum distance
    size_t min_index = 0;
    for (size_t i = 1; i < distances.size(); i++)
    {
        if (distances.at(i) < distances.at(min_index)) min_index = i;
    }

    bool is_buying_possible = false;
    if (state.get_current_turn_phase() == state::Turn_Phase::START &&
        actioning_player.get_tribes().first == nullptr)
    {
        is_buying_possible = true;
    }
    renderer.open_tribe_info_window(*state.get_tribes_on_top().at(min_index), is_buying_possible);
    client.set_selected_position_in_stack(static_cast<int>(min_index));
    client.set_tribe_info_window_state(true);
}

void Click_Handler_Multithread::handle_player_area_click(

    sf::Vector2i position, std::unordered_map<std::string, sf::FloatRect> layout_infos)
{
    state::Player& actioning_player = state.get_players().at(client.get_player_id());
    if (layout_infos["active_tribe"].contains(static_cast<sf::Vector2f>(position)))
    {
        renderer.open_tribe_info_window(*actioning_player.get_tribes().first, false);
        client.set_tribe_info_window_state(true);
    }
    else if (layout_infos["in_decline_tribe"].contains(static_cast<sf::Vector2f>(position)))
    {
        renderer.open_tribe_info_window(*actioning_player.get_tribes().second, false);
        client.set_tribe_info_window_state(true);
    }
}

void Click_Handler_Multithread::handle_info_window_click(
    sf::Vector2i position, std::unordered_map<std::string, sf::FloatRect> layout_infos)
{
    state::Player& actioning_player = state.get_players().at(client.get_player_id());
    if (layout_infos["tribe_info_window_close_button"].contains(
            static_cast<sf::Vector2f>(position)))
    {
        renderer.close_tribe_info_window();
        client.set_tribe_info_window_state(false);
    }

    else if (layout_infos["tribe_info_window_buy_button"].contains(
                 static_cast<sf::Vector2f>(position)))
    {
        if (state.get_current_turn_phase() == state::Turn_Phase::START &&
            actioning_player.get_tribes().first == nullptr)
        {
            auto command = std::make_unique<engine::Choose_Species_Command>(
                actioning_player.id, client.get_selected_position_in_stack());
            Json::Value command_json;
            command->to_json(command_json);
            engine.add_command(command_json);
            renderer.close_tribe_info_window();
            client.set_tribe_info_window_state(false);
        }
    }
}

void Click_Handler_Multithread::handle_button_area_click(
    sf::Vector2i position, std::unordered_map<std::string, sf::FloatRect> layout_infos)
{
    state::Player&    actioning_player = state.get_players().at(client.get_player_id());
    state::Turn_Phase current_phase    = state.get_current_turn_phase();

    if (layout_infos["conquer_button"].contains(static_cast<sf::Vector2f>(position)) &&
        current_phase == state::Turn_Phase::CONQUER)
    {
        // getting attackable areas
        std::vector<std::pair<int, int>> prices = state.get_conquest_prices(actioning_player.id);
        int                              price;
        for (const auto& price_info : prices)
        {
            if (price_info.first == client.get_selected_area_id())
            {
                price = price_info.second;
            }
        }

        std::pair<int, int> area_to_attack;
        int                 available_units = state.get_free_units_number(actioning_player.id);
        if (price > available_units)
        {
            int dice_units = state.roll_dice_for_bonus_units();
            std::cout << "No more area attackable, rolled the dice : " << dice_units << std::endl;
            auto command = std::make_unique<engine::Conquer_Command>(
                actioning_player.id, client.get_selected_area_id(), available_units, dice_units);
            Json::Value command_json;
            command->to_json(command_json);
            engine.add_command(command_json);
        }
        else
        {
            auto command = std::make_unique<engine::Conquer_Command>(
                actioning_player.id, client.get_selected_area_id(), price, -1);
            Json::Value command_json;
            command->to_json(command_json);
            engine.add_command(command_json);
        }
        return;
    }
    if (layout_infos["decline_button"].contains(static_cast<sf::Vector2f>(position)) &&
        current_phase == state::Turn_Phase::START)
    {
        auto command = std::make_unique<engine::Decline_Command>(actioning_player.id);
        Json::Value command_json;
        command->to_json(command_json);
        engine.add_command(command_json);
        return;
    }
    if (layout_infos["start_conquests_button"].contains(static_cast<sf::Vector2f>(position)) &&
        current_phase == state::Turn_Phase::START)
    {
        auto command = std::make_unique<engine::Start_Conquest_Command>(actioning_player.id);
        Json::Value command_json;
        command->to_json(command_json);
        engine.add_command(command_json);
        return;
    }
    if (layout_infos["end_conquests_button"].contains(static_cast<sf::Vector2f>(position)) &&
        current_phase == state::Turn_Phase::CONQUER)
    {
        auto command = std::make_unique<engine::End_Conquer_Command>(actioning_player.id);
        Json::Value command_json;
        command->to_json(command_json);
        engine.add_command(command_json);
        return;
    }
    if (layout_infos["redeploy_button"].contains(static_cast<sf::Vector2f>(position)) &&
        current_phase == state::Turn_Phase::REDEPLOY)
    {
        auto command = std::make_unique<engine::Redeploy_Command>(
            actioning_player.id, client.get_selected_area_id(), 1);
        Json::Value command_json;
        command->to_json(command_json);
        engine.add_command(command_json);
        return;
    }
}

void Click_Handler_Multithread::handle_opponent_area_click(
    sf::Vector2i position, std::unordered_map<std::string, sf::FloatRect> layout_infos)
{
    state::Player& actioning_player = state.get_players().at(client.get_player_id());
    std::cout << "Clicked on opponent info area" << std::endl;

    int i = 0;

    for (auto& player : state.get_players())
    {
        if (player.id == actioning_player.id)
        {
            continue;
        }

        bool clicked_on_active_tribe =
            layout_infos["player_" + std::to_string(i) + "_active_tribe"].contains(
                static_cast<sf::Vector2f>(position));
        bool clicked_on_decline_tribe =
            layout_infos["player_" + std::to_string(i) + "_in_decline_tribe"].contains(
                static_cast<sf::Vector2f>(position));

        if (clicked_on_active_tribe && player.get_tribes().first != nullptr)
        {
            renderer.open_tribe_info_window(*player.get_tribes().first, false);
            client.set_tribe_info_window_state(true);
            return;
        }
        if (clicked_on_decline_tribe && player.get_tribes().second != nullptr)
        {
            renderer.open_tribe_info_window(*player.get_tribes().second, false);
            client.set_tribe_info_window_state(true);
            return;
        }
        i++;
    }
}

}  // namespace client