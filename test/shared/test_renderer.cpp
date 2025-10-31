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
                    }
                }
            }

            window.clear(sf::Color::Black);
            renderer.render(window);
            window.display();
        }
    }
}