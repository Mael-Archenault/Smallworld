#include <boost/test/unit_test.hpp>


#include <state.h>
#include <renderer.h>


#include <SFML/Graphics.hpp>

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
  BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestTribeRenderer)
{
  {

    sf::RenderWindow window(sf::VideoMode(800,600), "Renderer Test");
    renderer::Token_Renderer units_renderer;
    units_renderer.set_sprite("Tritons");
    units_renderer.set_units_number(15);
    units_renderer.scale(1.0f, 1.0f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        units_renderer.render(window, sf::Vector2f(200,100));
        window.display();
    }


  }
}