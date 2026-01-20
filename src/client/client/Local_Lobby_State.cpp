#include <iostream>

#include "client.h"
#include "ai.h"

namespace client
{

Local_Lobby_State::Local_Lobby_State()
{
    std::cout << "Local Lobby State" << std::endl;
}

void Local_Lobby_State::handle_input(sf::Event event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::M)
        {
            std::cout << "Switching to Menu State" << std::endl;
            Menu_State* new_state = new Menu_State(this->context->get_window());
            this->context->change_state(new_state);
        }

        if (event.key.code == sf::Keyboard::G)
        {
            std::cout << "Switching to Local Game State" << std::endl;
            int                      nb_players   = 2;  //TODO delete
            std::vector<std::string> player_names = {"Alice", "Bob"};

            std::vector<ai::Ai_Type> ais = {ai::AI_Advanced_t};
            nb_players+= ais.size();



            for (auto i : ais) {
                switch (i) {
                    case ai::Ai_Random_t:
                        player_names.push_back("AI_Random");
                        break;
                    case ai::Ai_Heuristic_t:
                        player_names.push_back("AI_Heuristic");
                        break;
                    case ai::AI_Advanced_t:
                        player_names.push_back("AI_Advanced");
                        break;
                    default:
                        player_names.push_back("Patrice !!");
                        break;
                }
            }


            engine::Engine*          engine       = new engine::Engine(nb_players, player_names); // TODO make a smart pointer
            Local_Game_State*        new_state =
                new Local_Game_State(this->context->get_window(), *engine, ais);
            this->context->change_state(new_state);
        }
    }
    // Handle input events specific to the local lobby state
}

}  // namespace client
