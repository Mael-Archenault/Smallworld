#include "renderer.h"

namespace renderer
{
Player_Adder_Window::Player_Adder_Window(sf::RenderWindow& window) : window(window)
{
    name_label.set_content("Player Name : ");
    name_label.set_colors(sf::Color(50, 50, 50), sf::Color(50, 50, 50), sf::Color::White);

    name_box.set_content("");
    name_box.set_colors(sf::Color::White, sf::Color(10, 10, 10), sf::Color::White);

    real_player_label.set_content("Real Player");
    real_player_label.set_colors(sf::Color::White, sf::Color(200, 200, 200), sf::Color::Black);

    random_ai_label.set_content("Random AI");
    random_ai_label.set_colors(sf::Color::White, sf::Color(200, 200, 200), sf::Color::Black);

    heuristic_ai_label.set_content("Heuristic AI");
    heuristic_ai_label.set_colors(sf::Color::White, sf::Color(200, 200, 200), sf::Color::Black);

    advanced_ai_label.set_content("Advanced AI");
    advanced_ai_label.set_colors(sf::Color::White, sf::Color(200, 200, 200), sf::Color::Black);

    background.setFillColor(sf::Color(50, 50, 50));
}

void Player_Adder_Window::handle_input(sf::Event event) {}

void Player_Adder_Window::render()
{
    sf::Vector2u window_size   = window.getSize();
    float        button_width  = window_size.x / 4.f;
    float        button_height = window_size.y / 12.f;
    background.setSize(sf::Vector2f(window_size.x * 0.8, window_size.y * 0.8));
    background.setPosition(sf::Vector2f(window_size.x * 0.1, window_size.y * 0.1));
    window.draw(background);

    sf::Vector2f box_size = background.getSize();
    sf::Vector2f offset   = background.getPosition();

    name_label.set_size(sf::Vector2f(button_width, button_height));
    name_label.set_position(sf::Vector2f(offset.x + box_size.x * 0.1, offset.y + box_size.y * 0.2));
    name_label.set_character_size(button_height / 2);
    name_label.render(window);

    name_box.set_size(sf::Vector2f(1.5 * button_width, button_height));
    name_box.set_position(
        sf::Vector2f(offset.x + box_size.x * 0.1 + button_width, offset.y + box_size.y * 0.2));
    name_box.set_character_size(button_height / 2);
    name_box.render(window);

    real_player_label.set_size(sf::Vector2f(button_width, button_height));
    real_player_label.set_position(
        sf::Vector2f(window_size.x / 2 - button_width / 2, offset.y + box_size.y * 0.4));
    real_player_label.set_character_size(button_height / 2);
    real_player_label.render(window);

    random_ai_label.set_size(sf::Vector2f(button_width, button_height));
    random_ai_label.set_position(
        sf::Vector2f(window_size.x / 2 - 3 * button_width / 2, offset.y + box_size.y * 0.6));
    random_ai_label.set_character_size(button_height / 2);
    random_ai_label.render(window);

    heuristic_ai_label.set_size(sf::Vector2f(button_width, button_height));
    heuristic_ai_label.set_position(
        sf::Vector2f(window_size.x / 2 - button_width / 2, offset.y + box_size.y * 0.6));
    heuristic_ai_label.set_character_size(button_height / 2);
    heuristic_ai_label.render(window);

    advanced_ai_label.set_size(sf::Vector2f(button_width, button_height));
    advanced_ai_label.set_position(
        sf::Vector2f(window_size.x / 2 + button_width / 2, offset.y + box_size.y * 0.6));
    advanced_ai_label.set_character_size(button_height / 2);
    advanced_ai_label.render(window);
}
}  // namespace renderer