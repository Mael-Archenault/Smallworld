#include <iostream>

#include "client.h"

namespace client
{

Endgame_State::Endgame_State(sf::RenderWindow&                        window,
                             std::vector<std::pair<std::string, int>> players_money)
    : renderer(window)
{
    // ordering names by money
    ordered_names = players_money;
    std::sort(ordered_names.begin(), ordered_names.end(),
              [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b)
              { return a.second > b.second; });
    std::cout << "Endgame screen" << std::endl;
}

void Endgame_State::handle_input(sf::Event event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        register_click(sf::Mouse::getPosition(context->get_window()));
        register_layout(renderer.get_layout_infos());
        if (clicked_on("exit_button"))
        {
            Menu_State* new_state = new Menu_State(this->context->get_window());
            this->context->change_state(new_state);
        }
    }
}

void Endgame_State::render(sf::RenderWindow& window)
{
    renderer.render(ordered_names);
}

}  // namespace client