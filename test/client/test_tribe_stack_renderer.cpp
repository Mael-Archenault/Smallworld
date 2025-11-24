// tests/test_tribe_stack.cpp
#include <renderer.h>
#include <state.h>

#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(TestTribeStackRenderer)
{
    state::Tribe_Stack tribe_stack;

    sf::RenderWindow               window(sf::VideoMode(800, 600), "Renderer Test");
    renderer::Tribe_Stack_Renderer tribe_stack_renderer(window);

    tribe_stack_renderer.render(tribe_stack);
}