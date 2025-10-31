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
    renderer::Token_Renderer token_renderer("pawn");
    token_renderer.set_sprite("Tritons");
    token_renderer.set_number(15);
    token_renderer.scale(1.0f, 1.0f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        token_renderer.render(window, sf::Vector2f(200,100), true);
        window.display();
    }


  }
}