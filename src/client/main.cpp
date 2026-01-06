#include <iostream>

#include "client.h"
#include "engine.h"

// The following lines are here to check that SFML is installed and working
#include <unistd.h>

#include <SFML/Graphics.hpp>
#include <mutex>
#include <thread>

std::mutex mtx;
bool       running = false;

void testSFML()
{
    sf::Texture texture;
}

void engine_process(engine::Engine& engine)
{
    while (running)
    {
        mtx.lock();
        try
        {
            engine.update();
        }
        catch (std::exception& e)
        {
            engine.remove_last_command();
            std::cerr << e.what() << std::endl;
        }
        mtx.unlock();
        usleep(50000 / 3);  // 60 updates per secound
    }
}

void update_process(client::Client_Multithread& client)
{
    while (running)
    {
        try
        {
            {
                std::lock_guard<std::mutex> lock(mtx);
                client.update_process();
            }
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
        usleep(100000);  // 10 updates per secound
    }
}
void client_process(engine::Engine& engine)
{
    client::Client_Multithread client = client::Client_Multithread(engine);

    std::thread update_thread = std::thread(
        [](client::Client_Multithread& client) { update_process(client); }, std::ref(client));
    update_thread.detach();

    client.run();
    running = false;
}

int test_thread()
{
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

    running = true;
    for (int i = 0; i < nb_of_games; i++)
    {
        engine::Engine engine(nb_players, names);

        std::thread engine_thread =
            std::thread([](engine::Engine& engine) { engine_process(engine); }, std::ref(engine));
        engine_thread.detach();

        std::thread client1 =
            std::thread([](engine::Engine& engine) { client_process(engine); }, std::ref(engine));
        client1.detach();

        std::thread client2 =
            std::thread([](engine::Engine& engine) { client_process(engine); }, std::ref(engine));
        client2.join();

        // victory_count.at(client.run()) += 1;
    }

    for (int i = 0; i < victory_count.size(); i++)
    {
        std::cout << "Player " << i << " won " << victory_count.at(i) << " times." << std::endl;
    }
}
