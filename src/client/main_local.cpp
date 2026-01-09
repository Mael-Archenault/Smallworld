#include <iostream>

#include "client.h"
#include "engine.h"

// The following lines are here to check that SFML is installed and working
#include <unistd.h>

#include <SFML/Graphics.hpp>
#include <mutex>
#include <thread>

std::mutex mtx;
bool       engine_running;
bool       client_running;

void testSFML()
{
    sf::Texture texture;
}

void engine_process(engine::Engine& engine)
{
    while (engine_running)
    {
        {
            std::lock_guard<std::mutex> lock(mtx);
            try
            {
                engine.update();
            }
            catch (std::exception& e)
            {
                engine.remove_last_command();
                std::cerr << e.what() << std::endl;
            }
        }
        usleep(50000 / 3);  // 60 updates per secound
    }
}

void state_update_process(client::Client& client)
{
    while (true)
    {
        {
            std::lock_guard<std::mutex> lock(mtx);
            if (!client_running)
            {
                break;
            };
            client.update_state();
        }
        usleep(100000);  // 10 updates per secound
    }
}
void client_process(engine::Engine& engine, int player_id)  // contains 2 threads
//      - state update (request to the engine at 10 ips)
//      - handle click, send commands then rendering (at 60 ips)
{
    client::Client client = client::Client(engine);

    std::thread state_update_thread =
        std::thread([](client::Client& client) { state_update_process(client); }, std::ref(client));

    client.run();

    // Signal state_update_thread to exit
    {
        std::lock_guard<std::mutex> lock(mtx);
        client_running = false;
    }

    // Wait for state_update_thread to exit before client object is destroyed
    state_update_thread.join();

    std::cout << "Stopped client " << std::endl;

    engine_running = false;
    std::cout << "Stopped engine" << std::endl;
}

// end of test SFML

int main(int argc, char* argv[])
{
    int                      nb_players    = 3;
    std::vector<std::string> names         = {"Mael", "Victor", "Tarek"};
    std::vector<int>         victory_count = std::vector<int>(nb_players);
    int                      nb_of_games   = 1;

    for (int i = 0; i < nb_of_games; i++)
    {
        engine_running = true;
        engine::Engine engine(nb_players, names);

        std::thread engine_thread =
            std::thread([](engine::Engine& engine) { engine_process(engine); }, std::ref(engine));

        client_running            = true;
        std::thread client_thread = std::thread(
            [](engine::Engine& engine) { client_process(engine, 0); }, std::ref(engine));

        client_thread.join();
        engine_thread.join();

        // sleep(1);
        // std::thread client2 = std::thread([](engine::Engine& engine) { client_process(engine, 1);
        // },
        //                                   std::ref(engine));
        // client2.join();

        // victory_count.at(client.run()) += 1;
    }
}
