#include "Player_Area_Renderer.h"

namespace renderer {

  /// Constructor
  Player_Area_Renderer::Player_Area_Renderer (state::Player& player)
    : player(player), position(0.f, 0.f), angle(0)
  {
    // Load font
    if (!font.loadFromFile("/home/mael-archenault/Desktop/Smallworld/src/graphic_resources/arial.ttf")) {
      // Handle error
    }

    // Setup money title text
    money_title.setFont(font);
    money_title.setString("Money:");
    money_title.setCharacterSize(14);
    money_title.setFillColor(sf::Color::White);

    // Setup money value text
    money_value.setFont(font);
    money_value.setCharacterSize(14);
    money_value.setFillColor(sf::Color::Yellow);

    // Setup free units title text
    free_units_title.setFont(font);
    free_units_title.setString("Free Units:");
    free_units_title.setCharacterSize(14);
    free_units_title.setFillColor(sf::Color::White);
    
    tribes_title.setFont(font);
    tribes_title.setString("Tribes:");
    tribes_title.setCharacterSize(14);
    tribes_title.setFillColor(sf::Color::White);

    // Initialize tribe renderers
    std::vector<state::Tribe*> player_tribes = player.get_tribes();
    tribes.reserve(player_tribes.size());
    for (int i = 0; i < player_tribes.size(); i++) {
      auto renderer = std::make_unique<Tribe_Renderer>();
      renderer->set_sprite(player_tribes[i]->get_species_name(), player_tribes[i]->get_power_name());
      tribes.push_back(std::move(renderer));
    }
  }

  void Player_Area_Renderer::render (sf::RenderWindow& window) {

    sf::Vector2u window_size = window.getSize();

    float section_width = window_size.x*4/6;
    float section_height = window_size.y/6;

    // draw a rectangle
    sf::RectangleShape background(sf::Vector2f(section_width, section_height));

    background.setFillColor(sf::Color(20, 20, 20));
    background.setPosition(position);
    window.draw(background);



    float header_size = section_height/4;
    float body_size = section_height - header_size;

    // Titles in the header
    money_title.setCharacterSize(header_size/2);
    money_title.setPosition(position.x + section_width/8 - money_title.getGlobalBounds().width/2, position.y + header_size/2 - money_title.getGlobalBounds().height/2);
    window.draw(money_title);
    free_units_title.setCharacterSize(header_size/2);
    free_units_title.setPosition(position.x + section_width/4 + section_width/8 - free_units_title.getGlobalBounds().width/2, position.y + header_size/2 - free_units_title.getGlobalBounds().height/2);
    window.draw(free_units_title);
    tribes_title.setCharacterSize(header_size/2);
    tribes_title.setPosition(position.x + section_width*3/4 - tribes_title.getGlobalBounds().width/2, position.y + header_size/2 - tribes_title.getGlobalBounds().height/2);
    window.draw(tribes_title);




    // Money value

    money_value.setString(std::to_string(player.get_money()));
    money_value.setPosition(position.x + section_width/8 - money_value.getGlobalBounds().width/2, position.y + header_size + body_size/2 - money_value.getGlobalBounds().height/2);
    window.draw(money_value);



    // Free units
    std::vector<state::Tribe*> player_tribes = player.get_tribes();

    if (!player_tribes.empty()) {
        int free_units_number = player.get_free_units_number(player_tribes[0]->id);
        float scaling_factor = std::min((section_width/5)/150, body_size/150);
        free_units.scale(scaling_factor, scaling_factor);
        free_units.set_sprite(player_tribes[0]->get_species_name());
        free_units.set_units_number(free_units_number);
        free_units.render(window, sf::Vector2f(position.x + section_width/4+ section_width/8 - 150*scaling_factor/2, position.y + header_size + body_size/2 - 75*scaling_factor));
    }

    // Render tribes
    
    tribes.clear();
    for (int i = 0; i < player_tribes.size(); i++) {
      auto renderer = std::make_unique<Tribe_Renderer>();
      renderer->set_sprite(player_tribes[player_tribes.size()-1-i]->get_species_name(), player_tribes[player_tribes.size()-1-i]->get_power_name());
      tribes.push_back(std::move(renderer));
    }
    
    if (tribes.empty()) {
      return;
    }
    if (tribes.size() >= 1) {
      float scaling_factor = std::min((section_width/4)/500, body_size/194);
      tribes[0]->scale(scaling_factor, scaling_factor);
      tribes[0]->render(window, sf::Vector2f(position.x + section_width/2 + section_width/8 - (500*scaling_factor)/2, position.y + header_size + body_size/2 - (194*scaling_factor)/2));
    }

    if (tribes.size() >= 2) {
      float scaling_factor = std::min((section_width/4)/500, (body_size*2/3)/194);
      tribes[1]->scale(scaling_factor, scaling_factor);
      tribes[1]->render(window, sf::Vector2f(position.x + section_width*3/4 + section_width/8 - (500*scaling_factor)/2, position.y + header_size + body_size/3 - (194*scaling_factor)/2));
    }

    if (tribes.size() >= 3) {
      float scaling_factor = std::min((section_width/4)/500, (body_size*2/3)/194);
      tribes[2]->scale(scaling_factor, scaling_factor);
      tribes[2]->render(window, sf::Vector2f(position.x + section_width*3/4 + section_width/8- (500*scaling_factor)/2, position.y + header_size + body_size*2/3 - (194*scaling_factor)/2));
    }
    
  }

  void Player_Area_Renderer::set_position (sf::Vector2f position) {
    this->position = position;
  }


}