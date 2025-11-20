#include <iostream>

#include "client.h"
#include "engine.h"

// The following lines are here to check that SFML is installed and working
#include <SFML/Graphics.hpp>

void testSFML()
{
    sf::Texture texture;
}
// end of test SFML

int main(int argc, char* argv[])
{
    engine::Engine engine;
    client::Client client(engine);

    client.run();
}
