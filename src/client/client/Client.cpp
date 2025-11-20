#include "client.h"

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
    selected_area_id = 0;
    renderer.set_selected_area(selected_area_id);
}

void Client::run()
{
    sf::View view          = window.getDefaultView();  // store your base view
    bool     mouse_clicked = false;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();

            // When window is resized:
            if (event.type == sf::Event::Resized)
            {
                // Reset the view to match the new window size
                view.setSize(event.size.width, event.size.height);
                view.setCenter(event.size.width / 2.f, event.size.height / 2.f);
                window.setView(view);
            }

            // handle click

            if (event.type == sf::Event::MouseButtonPressed && mouse_clicked == false)
            {
                mouse_clicked          = true;
                sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                std::cout << "Mouse clicked at: " << mouse_pos.x << ", " << mouse_pos.y
                          << std::endl;
                handle_mouse_click(mouse_pos);
            }
            if (event.type == sf::Event::MouseButtonReleased)
            {
                mouse_clicked = false;
            }
        }

        try
        {
            engine.update();
            state = state::Game_State(engine.get_state());
        }
        catch (const std::exception& e)
        {
            engine.remove_last_command();
            std::cerr << "Error executing command: " << e.what() << std::endl;
        }

        window.clear(sf::Color::Black);
        renderer.render(state);
        window.display();
    }
}

void Client::handle_mouse_click(sf::Vector2i position)
{
    std::unordered_map<std::string, sf::FloatRect> layout_infos  = renderer.get_layout_infos();
    state::Turn_Phase                              current_phase = state.get_current_turn_phase();

    if (layout_infos["map"].contains(static_cast<sf::Vector2f>(position)))
    {
        handle_map_click(position, layout_infos["map"]);
    }
    else if (layout_infos["tribe_stack"].contains(static_cast<sf::Vector2f>(position)))
    {
        handle_tribe_stack_click(position, layout_infos["tribe_stack"]);
    }
    else if (layout_infos["conquer_button"].contains(static_cast<sf::Vector2f>(position)) &&
             current_phase == state::Turn_Phase::CONQUER)
    {
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

        std::cout << "Conquer button clicked for area " << selected_area_id << " with price "
                  << price << std::endl;
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
    }
    else if (layout_infos["decline_button"].contains(static_cast<sf::Vector2f>(position)) &&
             current_phase == state::Turn_Phase::START)
    {
        engine.add_command(
            std::make_unique<engine::Decline_Command>(state.get_current_player().id));
    }
    else if (layout_infos["start_conquest_button"].contains(static_cast<sf::Vector2f>(position)) &&
             current_phase == state::Turn_Phase::START)
    {
        engine.add_command(
            std::make_unique<engine::Start_Conquest_Command>(state.get_current_player().id));
    }
    else if (layout_infos["redeploy_button"].contains(static_cast<sf::Vector2f>(position)) &&
             current_phase == state::Turn_Phase::REDEPLOY)
    {
        engine.add_command(std::make_unique<engine::Redeploy_Command>(state.get_current_player().id,
                                                                      selected_area_id, 1));
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
    std::cout << "handling click on tribe stack" << std::endl;
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

    engine.add_command(std::make_unique<engine::Choose_Species_Command>(
        state.get_current_player().id, static_cast<int>(min_index)));
}

}  // namespace client
