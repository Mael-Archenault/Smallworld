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
    renderer::Tribe_Renderer tribe_renderer;
    tribe_renderer.set_sprite("Giants", "Alchemist");
    tribe_renderer.scale(0.5f, 0.5f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        tribe_renderer.render(window, sf::Vector2f(200,100));
        window.display();
    }


  }
}