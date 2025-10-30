#include <boost/test/unit_test.hpp>


#include <state.h>
#include <renderer.h>


#include <SFML/Graphics.hpp>
#include "graphic_resources_dir.h"

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
  BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestMapRenderer)
{
  {
    // testing initialization
    state::Map map;
  std::string filepath = std::string(RESOURCE_DIR) + "/map_2580x2580.png";
    
    // opening a sfml window 
    sf::RenderWindow window(sf::VideoMode(800,600), "Renderer Test");
    renderer::Map_Renderer map_renderer(filepath, map, window.getSize());
    // drawing a rectangle on it for 4 seconds

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        map_renderer.render(window, sf::Vector2f(100,100));
        window.display();
    }


  }
}