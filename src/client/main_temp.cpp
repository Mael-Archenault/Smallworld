#include <iostream>

#include "client.h"
#include "engine.h"

// The following lines are here to check that SFML is installed and working
#include <unistd.h>

#include <SFML/Graphics.hpp>
#include <mutex>
#include <thread>

std::mutex        mtx;
bool              engine_running;
std::vector<bool> client_running;

void testSFML()
{
    sf::Texture texture;
}

// void engine_process(engine::Engine& engine)
// {
//     while (true)
//     {
//         {
//             std::lock_guard<std::mutex> lock(mtx);
//             if (!engine_running)
//             {
//                 break;
//             }
//             try
//             {
//                 engine.update();
//             }
//             catch (std::exception& e)
//             {
//                 engine.remove_last_command();
//                 std::cerr << e.what() << std::endl;
//             }
//         }
//         usleep(50000 / 3);  // 60 updates per secound
//     }
// }

// void state_update_process(client::Game_Client_Multithread& client)
// {
//     while (true)
//     {
//         {
//             std::lock_guard<std::mutex> lock(mtx);
//             if (!client_running.at(client.get_player_id()))
//             {
//                 break;
//             };
//             client.update_state();
//         }
//         usleep(100000);  // 10 updates per secound
//     }
// }
// void client_process(engine::Engine& engine, int player_id)  // contains 2 threads
// //      - state update (request to the engine at 10 ips)
// //      - handle click, send commands then rendering (at 60 ips)
// {
//     client::Game_Client_Multithread client = client::Game_Client_Multithread(engine, player_id);

//     std::thread state_update_thread =
//         std::thread([](client::Game_Client_Multithread& client) { state_update_process(client);
//         },
//                     std::ref(client));

//     client.run();

//     // Signal state_update_thread to exit
//     {
//         std::lock_guard<std::mutex> lock(mtx);
//         client_running.at(client.get_player_id()) = false;
//     }

//     // Wait for state_update_thread to exit before client object is destroyed
//     state_update_thread.join();

//     std::cout << "Stopped client " << client.get_player_id() << std::endl;

//     // if all clients stopped running, stop engine
//     {
//         std::lock_guard<std::mutex> lock(mtx);
//         bool                        all_clients_stopped = true;
//         for (bool client_state : client_running)
//         {
//             if (client_state)
//             {
//                 all_clients_stopped = false;
//                 break;
//             }
//         }
//         if (all_clients_stopped)
//         {
//             engine_running = false;
//             std::cout << "No more clients -> Stopped engine" << std::endl;
//         }
//     }
// }

// end of test SFML

int main(int argc, char* argv[])
{
    // int                      nb_players    = 2;
    // std::vector<std::string> names         = {"Mael", "Alice"};
    // std::vector<int>         victory_count = std::vector<int>(nb_players);
    // int                      nb_of_games   = 1;

    // for (int i = 0; i < nb_players; i++)
    // {
    //     client_running.push_back(false);
    // }

    // for (int i = 0; i < nb_of_games; i++)
    // {
    //     engine_running = true;
    //     engine::Engine engine(nb_players, names);

    //     std::thread engine_thread =
    //         std::thread([](engine::Engine& engine) { engine_process(engine); },
    //         std::ref(engine));

    //     {
    //         std::lock_guard<std::mutex> lock(mtx);
    //         client_running.at(0) = true;
    //     }
    //     std::thread client1_thread = std::thread(
    //         [](engine::Engine& engine) { client_process(engine, 0); }, std::ref(engine));

    //     sleep(1);
    //     {
    //         std::lock_guard<std::mutex> lock(mtx);
    //         client_running.at(1) = true;
    //     }
    //     std::thread client2_thread = std::thread(
    //         [](engine::Engine& engine) { client_process(engine, 1); }, std::ref(engine));

    //     client1_thread.detach();
    //     client2_thread.detach();
    //     engine_thread.join();

    // sleep(1);
    // std::thread client2 = std::thread([](engine::Engine& engine) { client_process(engine, 1);
    // },
    //                                   std::ref(engine));
    // client2.join();

    // victory_count.at(client.run()) += 1;
}
