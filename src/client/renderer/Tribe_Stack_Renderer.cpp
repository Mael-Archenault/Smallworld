#include "Tribe_Stack_Renderer.h"
#include "resources_dir.h"
#include <state.h>


namespace renderer {

    Tribe_Stack_Renderer::Tribe_Stack_Renderer(state::Tribe_Stack& tribe_stack) : stack(tribe_stack)  {
        // Load font
        if (!font.loadFromFile(std::string(RESOURCE_DIR) + "/fonts/arial.ttf")) {
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
        sf::Vector2u window_size = window.getSize();
        std::vector<state::Tribe*> top_tribes = stack.get_tribes_on_top();


        float section_width = window_size.x/6;
        float section_height = window_size.y*5/6;

        float card_set_width = section_width*3/4;
        float card_set_height = section_height/6;
        for (size_t i = 0; i < top_tribes.size(); ++i) {
            tribes[i]->set_sprite(top_tribes[i]->get_species_name(), top_tribes[i]->get_power_name(), false);
            float scaling_factor = std::min(card_set_width/500, card_set_height/194);
            tribes[i]->scale(scaling_factor, scaling_factor);

            sf::Vector2f tribe_position = position + sf::Vector2f(0.0f, i*card_set_height);
            tribes[i]->render(window, tribe_position);

            prices[i].setPosition(tribe_position + sf::Vector2f(section_width*7/8, card_set_height/2-prices[i].getCharacterSize()/2));
            window.draw(prices[i]);
        }
    }



} 