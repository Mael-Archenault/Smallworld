#include "client.h"

#include <unistd.h>

#include <iostream>

namespace client
{

Client::Client() : window(sf::VideoMode(1280, 720), "Smallworld")
{
    current_state = new Menu_State(window);
    current_state->set_context(this);
    name = "No Name";
}

sf::RenderWindow& Client::get_window()
{
    return window;
}

std::string Client::get_name()
{
    return name;
}

void Client::set_name(std::string new_name)
{
    name = new_name;
}

void Client::run()
{
    sf::View view = window.getDefaultView();  // store your base view
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            // When window is resized:
            if (event.type == sf::Event::Resized)
            {
                // Reset the view to match the new window size
                view.setSize(event.size.width, event.size.height);
                view.setCenter(event.size.width / 2.f, event.size.height / 2.f);
                window.setView(view);
            }

            current_state->handle_input(event);
        }

        window.clear(sf::Color::Black);

        current_state->render(window);  // modifies the window

        window.display();
        usleep(1000000 / 60);  // limit to 60 fps
    }
}

void Client::change_state(Client_State_Interface* new_state)
{
    current_state = new_state;
    current_state->set_context(this);
}

}  // namespace client
