#include <renderer/Map_Renderer.h>
#include "graphic_resources_dir.h"
#include <iostream>

namespace renderer {

    Map_Renderer::Map_Renderer(state::Map& map, sf::Vector2u window_size): map(map) {
    std::string file_path = std::string(RESOURCE_DIR) + "/map_2580x2580.png";
    if (!this->texture.loadFromFile(file_path)) {
            throw std::runtime_error("Map_Renderer constructor: Failed to load texture");
        }
        this->sprite.setTexture(this->texture);
        float border_size = ((float)window_size.x)/4;
        
    }

  void Map_Renderer::render (sf::RenderWindow& window) {
    sf::Vector2u window_size = window.getSize();

    float scaling_factor = std::min(
        ((float)window_size.x*5) / (6*this->texture.getSize().x),
        ((float)window_size.y*5) / (6*this->texture.getSize().y)
    );
    this->sprite.setScale(scaling_factor, scaling_factor);

    this->sprite.setPosition(sf::Vector2f(((float)window_size.x - this->sprite.getGlobalBounds().width) / 2, 0.f));
    window.draw(this->sprite);
  }

};