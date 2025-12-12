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
    int                      nb_players    = 3;
    std::vector<std::string> names         = {"Victor", "Julien", "Tarek"};
    std::vector<int>         victory_count = std::vector<int>(nb_players);
    int                      nb_of_games   = 1;

    for (int i = 0; i < nb_of_games; i++)
    {
        engine::Engine engine(nb_players, names);
        client::Client client(engine);

        victory_count.at(client.run()) += 1;
    }

    for (int i = 0; i < victory_count.size(); i++)
    {
        std::cout << "Player " << i << " won " << victory_count.at(i) << " times." << std::endl;
    }
}
