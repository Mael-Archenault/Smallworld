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

    free_units.scale(0.5f,0.5f);

    
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
      if (i == player_tribes.size()-1){
        renderer->scale(0.5f,0.5f);
      }
      else{
        renderer->scale(0.2f,0.2f);
      }
      tribes.push_back(std::move(renderer));
    }
  }

  void Player_Area_Renderer::render (sf::RenderWindow& window) {
    // Update money value
    money_value.setString(std::to_string(player.get_money()));

    // Render money texts
    money_title.setPosition(position.x + 50.f, position.y + 50.f);
    window.draw(money_title);
    money_value.setPosition(position.x + 120.f, position.y + 80.f);
    window.draw(money_value);


    free_units_title.setPosition(position.x + 200.f, position.y + 50.f);
    window.draw(free_units_title);

    // Update and render free units
    std::vector<state::Tribe*> player_tribes = player.get_tribes();
    if (!player_tribes.empty()) {
        int free_units_number = player.get_free_units_number(player_tribes[0]->id);
        free_units.set_sprite(player_tribes[0]->get_species_name());
        free_units.set_units_number(free_units_number);
        free_units.render(window, sf::Vector2f(position.x + 200.f, position.y + 80.f));
    }

    // Render tribes

    tribes_title.setPosition(position.x + 350.f, position.y + 50.f);
    window.draw(tribes_title);

    float tribe_x = position.x + 370.f;
    float tribe_y = position.y + 80.f;

    for (int i = tribes.size()-1; i > -1; i--) {
        tribes[i]->render(window, sf::Vector2f(tribe_x, tribe_y));
        if (i == tribes.size()-1){
          tribe_x += 540 *0.5f + 10.f;
        }
        else{
          tribe_y += 194 * 0.2f + 10.f;
        }
    }
  }

  void Player_Area_Renderer::set_position (sf::Vector2f position) {
    this->position = position;
  }


}