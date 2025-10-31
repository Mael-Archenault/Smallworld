#include <boost/test/unit_test.hpp>

#include <state.h>
#include <renderer.h>
#include <fstream>

std::string json_data = R"({
  "biomes": { "0": ["MOUNTAIN"], "1": ["HILL"], "2": ["HILL"] },
  "specializations": { "0": [], "1": [], "2": ["MAGIC_REGION"] },
  "units": { "0": 0, "1": 0, "2": 0 },
  "relations": { "0": [1], "1": [0, 2], "2": [1] }
})";


BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestGameState)
{
    {
        state::Game_State state(1);

        sf::RenderWindow window(sf::VideoMode(1720,820), "Renderer Test", sf::Style::Titlebar | sf::Style::Close);
        renderer::Renderer renderer(state, window.getSize());

        state.take_tribe_at_position(0,0);
        int tribe_id = 0;
        state.conquer(0, 0, 7, 2, 0);
        
        
        sf::View view = window.getDefaultView(); // store your base view
        
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                window.close();
                
                // When window is resized:
                if (event.type == sf::Event::Resized) {
                    // Reset the view to match the new window size
                    view.setSize(event.size.width, event.size.height);
                    view.setCenter(event.size.width / 2.f, event.size.height / 2.f);
                    window.setView(view);
                }
                
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Space) {
                        state.go_in_decline(0);
                        state.take_tribe_at_position(0,0);
                        tribe_id ++;
                    }
                    if (event.key.code == sf::Keyboard::N) {
                        std::vector<std::pair<int, int>> prices = state.get_conquest_prices(0,0);
                        std::vector<std::pair<int, int>> attackable_areas;
                        for (const auto& price_info : prices) {
                           if (price_info.second <= state.get_free_units_number(0,0)) {
                               attackable_areas.push_back(price_info);
                           }
                        }
                        if (attackable_areas.size() == 0) {
                            return;
                        }

                        std::pair<int, int> area_to_attack = attackable_areas[0];
                        state.conquer(0, 0, area_to_attack.first,area_to_attack.second,0);
                    }
                }
                
            }

            window.clear(sf::Color::Black);
            renderer.render(window);
            window.display();
        }
    }
}