#include <renderer/Map_Renderer.h>
#include <iostream>

namespace renderer {

    Map_Renderer::Map_Renderer(std::string file_path, state::Map& map, sf::Vector2u window_size): map(map) {
        if (!this->texture.loadFromFile(file_path)) {
            throw std::runtime_error("Map_Renderer constructor: Failed to load texture");
        }
        this->sprite.setTexture(this->texture);
        int border_size = 100;
        this->sprite.setScale(
            (static_cast<float>((window_size.x)-2*border_size)) / this->texture.getSize().x,
            (static_cast<float>((window_size.y)-2*border_size)) / this->texture.getSize().y
        );
    }

  void Map_Renderer::render (sf::RenderWindow& window, sf::Vector2f position) {
    this->sprite.setPosition(position);
    window.draw(this->sprite);
  }

};