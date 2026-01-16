#include <SFML/Graphics.hpp>
#include <boost/test/unit_test.hpp>

#include "effects.h"
#include "renderer.h"
#include "state.h"

BOOST_AUTO_TEST_CASE(TestStaticAssert)
{
    BOOST_CHECK(1);
}

BOOST_AUTO_TEST_CASE(TestPlayerAreaRenderer)
{
    // instanciating useful classes
    state::Tribe* tribe0 = new state::Tribe(0, new effects::Species_Description("Dwarves", 5, 10),
                                            new effects::Power_Description("Berserk", 3));
    state::Tribe* tribe1 = new state::Tribe(1, new effects::Species_Description("Giants", 5, 10),
                                            new effects::Power_Description("Alchemist", 3));
    state::Tribe* tribe2 = new state::Tribe(2, new effects::Species_Description("Ratmen", 5, 10),
                                            new effects::Power_Description("Bivouacking", 3));
    static state::Area area(0, 1, state::Area_Biome::HILL,
                            std::vector<state::Area_Specialization>(), false);
    state::Player      player(0, "Alice");

    player.choose_active_tribe(tribe0, 0);
    player.choose_active_tribe(tribe1, 0);
    player.choose_active_tribe(tribe2, 0);

    sf::RenderWindow               window(sf::VideoMode(800, 600), "Renderer Test");
    renderer::Player_Area_Renderer player_area_renderer(window);

    player_area_renderer.set_position(sf::Vector2f(50.f, 50.f));

    player_area_renderer.render(player);
}