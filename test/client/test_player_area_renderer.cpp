#include <boost/test/unit_test.hpp>

#include <SFML/Graphics.hpp>

#include <renderer.h>
#include <state.h>

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
  BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestPlayerAreaRenderer)
{
  {
    // instanciating useful classes
    state::Tribe* tribe0 = new state::Tribe(
      0,
      new state::Species_Description("Dwarves", 5, 10, state::Effects_Bundle()),
      new state::Power_Description("Berserk", 3, state::Effects_Bundle())
    );
    state::Tribe* tribe1 = new state::Tribe(
        1,
        new state::Species_Description("Giants", 5, 10, state::Effects_Bundle()),
        new state::Power_Description("Alchemist", 3, state::Effects_Bundle())
    );
    state::Tribe* tribe2 = new state::Tribe(
        2,
        new state::Species_Description("Ratmen", 5, 10, state::Effects_Bundle()),
        new state::Power_Description("Bivouacking", 3, state::Effects_Bundle())
    );

    static state::Area area(0,1,state::Area_Biome::HILL, std::vector<state::Area_Specialization>());
    state::Player player(0);

    player.set_active_tribe(tribe0);
    player.set_active_tribe(tribe1);
    player.set_active_tribe(tribe2);


    sf::RenderWindow window(sf::VideoMode(800,600), "Renderer Test");
    renderer::Player_Area_Renderer player_area_renderer(player);

    player_area_renderer.set_position(sf::Vector2f(50.f, 50.f));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);
        player_area_renderer.render(window);
        window.display();
    }

  }
}