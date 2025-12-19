#include <iostream>

#include "client.h"
#include "engine.h"

// The following lines are here to check that SFML is installed and working
#include <SFML/Graphics.hpp>
#include <thread>

void testSFML()
{
    sf::Texture texture;
}


std::thread create_client(engine::Engine engine) {
    client::Client_Multithread client =  client::Client_Multithread(engine);
    std::thread client_thread([&client](){client.run();});
    client_thread.detach();
    return client_thread;
}
int test_thread() {
    std::cout << "test_thread" << std::endl;
    return 0;
}
// end of test SFML

int main(int argc, char* argv[])
{
    int                      nb_players    = 2;
    std::vector<std::string> names         = {"Mael", "Alice"};
    std::vector<int>         victory_count = std::vector<int>(nb_players);
    int                      nb_of_games   = 1;

    for (int i = 0; i < nb_of_games; i++)
    {
        engine::Engine engine(nb_players, names);


        std::thread client1 = create_client(engine);








        //victory_count.at(client.run()) += 1;
    }

    for (int i = 0; i < victory_count.size(); i++)
    {
        std::cout << "Player " << i << " won " << victory_count.at(i) << " times." << std::endl;
    }
}
