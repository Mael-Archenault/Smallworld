#include <boost/test/unit_test.hpp>
#include <fstream>
#include <iostream>

#include "engine.h"
#include "renderer.h"
#include "state.h"

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(Test_Engine)
{
    {
        state::Game_State  state(2);
        sf::RenderWindow   window(sf::VideoMode(1720, 820), "Renderer Test");
        renderer::Renderer renderer(state, window);

        engine::Engine engine(state);

        sf::View view = window.getDefaultView();  // store your base view

        int selected_area = -1;

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

                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Space)  // Choose species
                    {
                        try
                        {
                            engine.add_command(std::make_unique<engine::Choose_Species_Command>(
                                state.get_current_player().id, 0));
                            engine.update();
                        }
                        catch (std::exception& e)
                        {
                            std::cout << e.what() << std::endl;
                        }
                    }
                    if (event.key.code == sf::Keyboard::S)  // start conquest
                    {
                        try
                        {
                            engine.add_command(std::make_unique<engine::Start_Conquest_Command>(
                                state.get_current_player().id));
                            engine.update();
                        }
                        catch (std::exception& e)
                        {
                            std::cout << e.what() << std::endl;
                        }
                    }
                    if (event.key.code == sf::Keyboard::D)  // decline
                    {
                        try
                        {
                            engine.add_command(std::make_unique<engine::Decline_Command>(
                                state.get_current_player().id));
                            engine.update();
                        }
                        catch (std::exception& e)
                        {
                            std::cout << e.what() << std::endl;
                        }
                    }
                    if (event.key.code == sf::Keyboard::C)  // conquer
                    {
                        try
                        {
                            int dice_units = -1;

                            std::vector<std::pair<int, int>> prices =
                                state.get_conquest_prices(state.get_current_player().id);
                            std::vector<std::pair<int, int>> attackable_areas;
                            for (const auto& price_info : prices)
                            {
                                if (price_info.second <=
                                    state.get_free_units_number(state.get_current_player().id))
                                {
                                    attackable_areas.push_back(price_info);
                                }
                            }

                            std::pair<int, int> area_to_attack;
                            if (attackable_areas.size() == 0)
                            {
                                dice_units     = state.roll_dice_for_bonus_units();
                                area_to_attack = prices[std::rand() % prices.size()];
                                std::cout
                                    << "No more area attackable, rolled the dice : " << dice_units
                                    << std::endl;
                            }
                            else
                            {
                                area_to_attack =
                                    attackable_areas[std::rand() % attackable_areas.size()];
                            }
                            engine.add_command(std::make_unique<engine::Conquer_Command>(
                                state.get_current_player().id, area_to_attack.first,
                                area_to_attack.second, dice_units));
                            engine.update();
                        }
                        catch (std::exception& e)
                        {
                            std::cout << e.what() << std::endl;
                        }
                    }
                    if (event.key.code == sf::Keyboard::E)  // End conquer
                    {
                        try
                        {
                            engine.add_command(std::make_unique<engine::End_Conquer_Command>(
                                state.get_current_player().id));
                            engine.update();
                        }
                        catch (std::exception& e)
                        {
                            std::cout << e.what() << std::endl;
                        }
                    }

                    if (event.key.code == sf::Keyboard::R)  // redeploy command
                    {
                        try
                        {
                            int              player_id = state.get_current_player().id;
                            std::vector<int> redeployable_areas =
                                state.get_redeployable_areas(player_id);

                            if (redeployable_areas.empty())
                            {
                                throw std::runtime_error(
                                    "No redeployable areas available for this player.");
                            }

                            engine.add_command(std::make_unique<engine::Redeploy_Command>(
                                player_id, redeployable_areas.at(0), 1));

                            engine.update();
                            std::cout << "Ok" << std::endl;
                        }
                        catch (std::exception& e)
                        {
                            std::cout << e.what() << std::endl;
                        }
                    }

                    if (event.key.code == sf::Keyboard::N)
                    {
                        renderer.set_selected_area(selected_area);
                        selected_area = (selected_area + 1) % state.get_map().get_areas().size();
                    }
                }
            }

            window.clear(sf::Color::Black);
            renderer.render(state);
            window.display();
        }
    }
}