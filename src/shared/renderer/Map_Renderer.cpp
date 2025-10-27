#include <renderer/Map_Renderer.h>
#include <iostream>

namespace renderer {

    Map_Renderer::Map_Renderer(std::string file_path, state::Map& map, sf::Vector2u window_size): map(map) {
        if (!this->texture.loadFromFile(file_path)) {
            // Handle loading error
            std::cout<<"Error loading map texture from " << file_path << std::endl;
            throw std::runtime_error("Failed to load texture");
        }
        this->sprite.setTexture(this->texture);
        // setting the size of the sprite, according to the window size
        this->sprite.setScale(
            static_cast<float>(window_size.x) / this->texture.getSize().x,
            static_cast<float>(window_size.y) / this->texture.getSize().y
        );
    }

  void Map_Renderer::render (sf::RenderWindow& window, sf::Vector2f position) {
    this->sprite.setPosition(position);
    window.draw(this->sprite);
  }

};