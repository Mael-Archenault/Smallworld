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
    state::Game_State state({"Alice"}, 0);

    sf::RenderWindow        window(sf::VideoMode(1720, 820), "Renderer Test",
                                   sf::Style::Titlebar | sf::Style::Close);
    renderer::Game_Renderer renderer(state, window);

    renderer.render(state, state.get_current_player().id);
}