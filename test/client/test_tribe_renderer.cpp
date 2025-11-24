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
    renderer::Tribe_Renderer tribe_renderer;
    tribe_renderer.set_sprite("Giants", "Alchemist", false);
    tribe_renderer.scale(0.5f, 0.5f);

    tribe_renderer.render(window, sf::Vector2f(200, 100));
}