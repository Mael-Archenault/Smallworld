#include "ai.h"
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

    close_window_button.set_content("Close window");
    close_window_button.set_colors(sf::Color::White, sf::Color(100, 100, 100), sf::Color::White);

    add_button.set_content("Add Player");
    add_button.set_colors(sf::Color::White, sf::Color(100, 100, 100), sf::Color::White);
}

void Player_Adder_Window::handle_input(sf::Event event) {}

void Player_Adder_Window::render(bool modifying_name, std::string name,
                                 state::Player_Type selected_player_type)
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
    if (modifying_name)
    {
        name_box.set_colors(sf::Color(50, 50, 50), sf::Color(150, 150, 150), sf::Color(50, 50, 50));
    }
    else
    {
        name_box.set_colors(sf::Color(50, 50, 50), sf::Color(250, 250, 250), sf::Color(50, 50, 50));
    }

    name_box.set_content(name);
    name_box.set_character_size(button_height / 2);
    name_box.render(window);

    real_player_label.set_size(sf::Vector2f(button_width, button_height));
    real_player_label.set_position(
        sf::Vector2f(window_size.x / 2 - button_width / 2, offset.y + box_size.y * 0.4));
    real_player_label.set_character_size(button_height / 2);
    if (selected_player_type == ai::Human_t)
    {
        real_player_label.set_colors(sf::Color::Black, sf::Color::White, sf::Color::Black);
    }
    else
    {
        real_player_label.set_colors(sf::Color::White, sf::Color(200, 200, 200), sf::Color::Black);
    }
    real_player_label.render(window);

    random_ai_label.set_size(sf::Vector2f(button_width, button_height));
    random_ai_label.set_position(
        sf::Vector2f(window_size.x / 2 - 3.1 * button_width / 2, offset.y + box_size.y * 0.6));
    random_ai_label.set_character_size(button_height / 2);
    if (selected_player_type == ai::Ai_Random_t)
    {
        random_ai_label.set_colors(sf::Color::Black, sf::Color::White, sf::Color::Black);
    }
    else
    {
        random_ai_label.set_colors(sf::Color::White, sf::Color(200, 200, 200), sf::Color::Black);
    }
    random_ai_label.render(window);

    heuristic_ai_label.set_size(sf::Vector2f(button_width, button_height));
    heuristic_ai_label.set_position(
        sf::Vector2f(window_size.x / 2 - button_width / 2, offset.y + box_size.y * 0.6));
    heuristic_ai_label.set_character_size(button_height / 2);
    if (selected_player_type == ai::Ai_Heuristic_t)
    {
        heuristic_ai_label.set_colors(sf::Color::Black, sf::Color::White, sf::Color::Black);
    }
    else
    {
        heuristic_ai_label.set_colors(sf::Color::White, sf::Color(200, 200, 200), sf::Color::Black);
    }
    heuristic_ai_label.render(window);

    advanced_ai_label.set_size(sf::Vector2f(button_width, button_height));
    advanced_ai_label.set_position(
        sf::Vector2f(window_size.x / 2 + 1.1 * button_width / 2, offset.y + box_size.y * 0.6));
    advanced_ai_label.set_character_size(button_height / 2);
    if (selected_player_type == ai::Ai_Advanced_t)
    {
        advanced_ai_label.set_colors(sf::Color::Black, sf::Color::White, sf::Color::Black);
    }
    else
    {
        advanced_ai_label.set_colors(sf::Color::White, sf::Color(200, 200, 200), sf::Color::Black);
    }
    advanced_ai_label.render(window);

    close_window_button.set_size(
        sf::Vector2f(background.getSize().x * 0.3, background.getSize().y * 0.1));
    close_window_button.set_character_size(button_height / 2);
    close_window_button.set_position(
        sf::Vector2f(background.getPosition().x + background.getSize().x * 0.1,
                     background.getPosition().y + background.getSize().y * 0.85));

    close_window_button.render(window);

    add_button.set_size(sf::Vector2f(background.getSize().x * 0.3, background.getSize().y * 0.1));
    add_button.set_character_size(button_height / 2);
    add_button.set_position(
        sf::Vector2f(background.getPosition().x + background.getSize().x * 0.6,
                     background.getPosition().y + background.getSize().y * 0.85));
    add_button.render(window);
}
std::unordered_map<std::string, sf::FloatRect> Player_Adder_Window::get_layout_infos()
{
    std::unordered_map<std::string, sf::FloatRect> rects;
    rects["adder_window"]                     = background.getGlobalBounds();
    rects["adder_window_name_box"]            = name_box.get_rect();
    rects["adder_window_real_player_button"]  = real_player_label.get_rect();
    rects["adder_window_random_ai_button"]    = random_ai_label.get_rect();
    rects["adder_window_heuristic_ai_button"] = heuristic_ai_label.get_rect();
    rects["adder_window_advanced_ai_button"]  = advanced_ai_label.get_rect();
    rects["adder_window_close_button"]        = close_window_button.get_rect();
    rects["adder_window_add_button"]          = add_button.get_rect();
    return rects;
}
}  // namespace renderer