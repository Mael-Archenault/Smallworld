#include <renderer.h>
#include <state.h>

#include <boost/test/unit_test.hpp>
#include <fstream>

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(Test_Renderer)
{
    state::Game_State state(1, {"Alice"});

    sf::RenderWindow   window(sf::VideoMode(1720, 820), "Renderer Test",
                              sf::Style::Titlebar | sf::Style::Close);
    renderer::Renderer renderer(state, window);

    renderer.render(state, state.get_current_player().id);
}