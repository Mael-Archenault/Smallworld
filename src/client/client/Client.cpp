#include "client.h"

#include <ai/Ai_Heuristic.h>
#include <ai/Ai_Random.h>

#include <cmath>
#include <iostream>

#include "engine.h"

namespace client
{

Client::Client(engine::Engine& engine)
    : window(sf::VideoMode(1720, 820), "Smallworld"),
      state(engine.get_state()),
      renderer(state, window),
      engine(engine)
{
    selected_area_id         = 0;
    tribe_info_window_opened = false;
    renderer.set_selected_area(selected_area_id);
    // ais = std::vector<ai::Ai_Interface*>({new ai::Ai_Random(&state,1), new
    // ai::Ai_Heuristic(&state,0)});
    ais = std::vector<ai::Ai_Interface*>();
}

int Client::run()
{
    // first rendering
    sf::Event e;
    while (window.pollEvent(e))
    {
    }  // purge early events

    window.clear(sf::Color::Black);
    renderer.render(state);
    window.display();

    sf::View view           = window.getDefaultView();  // store your base view
    bool     mouse_clicked  = false;
    bool     event_happened = false;
    while (window.isOpen())
    {
        bool is_ai_turn = false;
        for (auto ai : ais)
        {
            if (ai->id == state.get_current_player().id)
            {
                is_ai_turn = true;
                ai->update_state(&state);
                engine.add_command(ai->give_command(state.get_current_turn_phase()));
                event_happened = true;
                // sf::sleep(sf::seconds(1));
            }
        }
        if (!is_ai_turn)
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed) window.close();

                // When window is resized:
                if (event.type == sf::Event::Resized)
                {
                    // Reset the view to match the new window size
                    event_happened = true;
                    view.setSize(event.size.width, event.size.height);
                    view.setCenter(event.size.width / 2.f, event.size.height / 2.f);
                    window.setView(view);
                }

                // handle click

                if (event.type == sf::Event::MouseButtonPressed && mouse_clicked == false)
                {
                    event_happened         = true;
                    mouse_clicked          = true;
                    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                    handle_mouse_click(mouse_pos);
                }
                if (event.type == sf::Event::MouseButtonReleased)
                {
                    mouse_clicked = false;
                }
            }
        }

        try
        {
            engine.update();
            state = state::Game_State(engine.get_state());

            if (state.is_game_finished())
            {
                std::cout << "Maximum rounds reached." << std::endl;
                std::pair<int, int> victorious_player_money = {0, -1};
                for (std::pair<int, int> player_id_money : state.get_all_player_id_money())
                {
                    std::cout << "Player " << player_id_money.first << " has "
                              << player_id_money.second << " money." << std::endl;
                    if (player_id_money.second >= victorious_player_money.second)
                    {
                        victorious_player_money = player_id_money;
                    }
                }
                std::cout << "\nVictory goes to " << victorious_player_money.first << ", with "
                          << victorious_player_money.second << " money !" << std::endl;
                window.close();
                return victorious_player_money.first;
            }
        }
        catch (const std::exception& e)
        {
            engine.remove_last_command();
            std::cerr << "Error executing command: " << e.what() << std::endl;
        }

        if (event_happened)
        {
            window.clear(sf::Color::Black);
            renderer.render(state);
            window.display();
            event_happened = false;
        }
    }
}

void Client::handle_mouse_click(sf::Vector2i position)
{
    std::unordered_map<std::string, sf::FloatRect> layout_infos  = renderer.get_layout_infos();
    state::Turn_Phase                              current_phase = state.get_current_turn_phase();

    // tribe window management
    if (tribe_info_window_opened)
    {
        if (layout_infos["tribe_info_window"].contains(static_cast<sf::Vector2f>(position)))
        {
            handle_info_window_click(position, layout_infos);
            return;
        }
        else
        {
            renderer.close_tribe_info_window();
            tribe_info_window_opened = false;
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

    // click on the player area

    if (layout_infos["player_area"].contains(static_cast<sf::Vector2f>(position)))
    {
        handle_player_area_click(position, layout_infos);
        return;
    }

    // click on the buttons
    if (layout_infos["conquer_button"].contains(static_cast<sf::Vector2f>(position)) &&
        current_phase == state::Turn_Phase::CONQUER)
    {
        // getting attackable areas
        std::vector<std::pair<int, int>> prices =
            state.get_conquest_prices(state.get_current_player().id);
        int price;
        for (const auto& price_info : prices)
        {
            if (price_info.first == selected_area_id)
            {
                price = price_info.second;
            }
        }

        std::pair<int, int> area_to_attack;
        int available_units = state.get_free_units_number(state.get_current_player().id);
        if (price > available_units)
        {
            int dice_units = state.roll_dice_for_bonus_units();
            std::cout << "No more area attackable, rolled the dice : " << dice_units << std::endl;
            engine.add_command(std::make_unique<engine::Conquer_Command>(
                state.get_current_player().id, selected_area_id, available_units, dice_units));
        }
        else
        {
            engine.add_command(std::make_unique<engine::Conquer_Command>(
                state.get_current_player().id, selected_area_id, price, -1));
        }
        return;
    }
    if (layout_infos["decline_button"].contains(static_cast<sf::Vector2f>(position)) &&
        current_phase == state::Turn_Phase::START)
    {
        engine.add_command(
            std::make_unique<engine::Decline_Command>(state.get_current_player().id));
        return;
    }
    if (layout_infos["start_conquests_button"].contains(static_cast<sf::Vector2f>(position)) &&
        current_phase == state::Turn_Phase::START)
    {
        engine.add_command(
            std::make_unique<engine::Start_Conquest_Command>(state.get_current_player().id));
        return;
    }
    if (layout_infos["end_conquests_button"].contains(static_cast<sf::Vector2f>(position)) &&
        current_phase == state::Turn_Phase::CONQUER)
    {
        engine.add_command(
            std::make_unique<engine::End_Conquer_Command>(state.get_current_player().id));
        return;
    }
    if (layout_infos["redeploy_button"].contains(static_cast<sf::Vector2f>(position)) &&
        current_phase == state::Turn_Phase::REDEPLOY)
    {
        engine.add_command(std::make_unique<engine::Redeploy_Command>(state.get_current_player().id,
                                                                      selected_area_id, 1));
        return;
    }
}

void Client::handle_map_click(sf::Vector2i position, sf::FloatRect map_layout)
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
    selected_area_id = static_cast<int>(min_index);
}

void Client::handle_tribe_stack_click(sf::Vector2i position, sf::FloatRect tribe_stack_layout)
{
    std::vector<sf::Vector2f> card_positions = renderer.get_on_screen_tribe_positions();

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

    renderer.open_tribe_info_window(*state.get_tribes_on_top().at(min_index));
    selected_position_in_stack = static_cast<int>(min_index);
    tribe_info_window_opened   = true;
}

void Client::handle_player_area_click(sf::Vector2i                                   position,
                                      std::unordered_map<std::string, sf::FloatRect> layout_infos)
{
    if (layout_infos["active_tribe"].contains(static_cast<sf::Vector2f>(position)))
    {
        renderer.open_tribe_info_window(*state.get_current_player().get_tribes().first);
        tribe_info_window_opened = true;
    }
    else if (layout_infos["in_decline_tribe"].contains(static_cast<sf::Vector2f>(position)))
    {
        renderer.open_tribe_info_window(*state.get_current_player().get_tribes().second);
        tribe_info_window_opened = true;
    }
}

void Client::handle_info_window_click(sf::Vector2i                                   position,
                                      std::unordered_map<std::string, sf::FloatRect> layout_infos)
{
    if (layout_infos["tribe_info_window_close_button"].contains(
            static_cast<sf::Vector2f>(position)))
    {
        renderer.close_tribe_info_window();
        tribe_info_window_opened = false;
    }

    else if (layout_infos["tribe_info_window_buy_button"].contains(
                 static_cast<sf::Vector2f>(position)))
    {
        if (state.get_current_turn_phase() == state::Turn_Phase::START &&
            state.get_current_player().get_tribes().first == nullptr)
        {
            engine.add_command(std::make_unique<engine::Choose_Species_Command>(
                state.get_current_player().id, selected_position_in_stack));
            renderer.close_tribe_info_window();
            tribe_info_window_opened = false;
        }
    }
}
}  // namespace client
