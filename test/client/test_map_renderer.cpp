#include <renderer.h>
#include <state.h>

#include <SFML/Graphics.hpp>
#include <boost/test/unit_test.hpp>

#include "resources_dir.h"

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestMapRenderer)
{
    // testing initialization
    state::Map map("4_players");
    // opening a sfml window
    sf::RenderWindow       window(sf::VideoMode(800, 600), "Renderer Test");
    renderer::Map_Renderer map_renderer(window);

    map_renderer.render(map);
}