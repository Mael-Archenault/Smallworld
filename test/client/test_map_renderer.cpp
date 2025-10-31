#include <boost/test/unit_test.hpp>


#include <state.h>
#include <renderer.h>


#include <SFML/Graphics.hpp>
#include "resources_dir.h"

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
  BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestMapRenderer)
{
  {
    // testing initialization
    state::Map map("4_players");
    // opening a sfml window 
    sf::RenderWindow window(sf::VideoMode(800,600), "Renderer Test");
    renderer::Map_Renderer map_renderer(map, window.getSize());
    // // drawing a rectangle on it for 4 seconds

     sf::View view = window.getDefaultView(); // store your base view

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::Resized) {
                // Reset the view to match the new window size
                view.setSize(event.size.width, event.size.height);
                view.setCenter(event.size.width / 2.f, event.size.height / 2.f);
                window.setView(view);
            }
        }

        window.clear(sf::Color::Black);
        map_renderer.render(window);
        window.display();
    }


  }
}