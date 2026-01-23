#include <renderer.h>
#include <state.h>

#include <SFML/Graphics.hpp>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestTribeRenderer)
{
    sf::RenderWindow         window(sf::VideoMode(800, 600), "Renderer Test");
    renderer::Token_Renderer token_renderer("area_specialization");
    token_renderer.set_sprite("Cavern");
    token_renderer.set_number(15);
    token_renderer.scale_token(1.0f, 1.0f);
    token_renderer.scale_text(1.0f, 1.0f);
    token_renderer.render(window, sf::Vector2f(200, 100), false);
}