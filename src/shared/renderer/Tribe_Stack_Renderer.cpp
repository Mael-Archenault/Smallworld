#include "Tribe_Stack_Renderer.h"
#include <state.h>


namespace renderer {

    Tribe_Stack_Renderer::Tribe_Stack_Renderer(state::Tribe_Stack& tribe_stack) : stack(tribe_stack)  {
        // Load font
        if (!font.loadFromFile("/home/mael-archenault/Desktop/Smallworld/src/graphic_resources/arial.ttf")) {
            throw std::runtime_error("Tribe Stack Renderer: Failed to load font");
        }

        // Initialize Tribe_Renderers and prices
        std::vector<state::Tribe*> top_tribes = stack.get_tribes_on_top();
        for (size_t i = 0; i < top_tribes.size(); ++i) {
            tribes.push_back(std::make_unique<Tribe_Renderer>());
            sf::Text price_text;
            price_text.setFont(font);
            price_text.setString(std::to_string(i));
            price_text.setCharacterSize(32);
            price_text.setFillColor(sf::Color::White);
            prices.push_back(price_text);
        }

        set_position(sf::Vector2f(0.f, 0.f));
    }

    void Tribe_Stack_Renderer::set_position(sf::Vector2f pos) {
        position = pos;
    }

    void Tribe_Stack_Renderer::render(sf::RenderWindow& window) {
        std::vector<state::Tribe*> top_tribes = stack.get_tribes_on_top();
        for (size_t i = 0; i < top_tribes.size(); ++i) {
            tribes[i]->set_sprite(top_tribes[i]->get_species_name(), top_tribes[i]->get_power_name());
            tribes[i]->scale(0.5f, 0.5f);
            sf::Vector2f tribe_position = position + sf::Vector2f(0.0f, i*110.f);
            tribes[i]->render(window, tribe_position);

            prices[i].setPosition(tribe_position + sf::Vector2f(300.f, 40.f));
            window.draw(prices[i]);
        }
    }



} // namespace renderer