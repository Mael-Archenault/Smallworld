// tests/test_tribe_stack.cpp
#include <boost/test/included/unit_test.hpp>
#include <state.h>
#include <renderer.h>


BOOST_AUTO_TEST_CASE(TestTribeStackRenderer)
{
    state::Tribe_Stack tribe_stack;
    renderer::Tribe_Stack_Renderer tribe_stack_renderer(tribe_stack);

    sf::RenderWindow window(sf::VideoMode(800,600), "Renderer Test");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                // Simulate taking a tribe from the stack when space is pressed
                try {
                    tribe_stack.take_tribe_at_position(0);
                } catch (const std::out_of_range& e) {
                    // Stack is empty, do nothing
                }
            }
        }

        window.clear(sf::Color::Black);
        tribe_stack_renderer.render(window);
        window.display();
    }
}