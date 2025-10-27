#include "Units_Renderer.h"


namespace renderer {

    bool Units_Renderer::pawn_spritesheet_loaded = false;

    sf::Texture Units_Renderer::pawn_spritesheet;
   
    Units_Renderer::Units_Renderer(){
        set_sprite("lost_tribe");
        if (!font.loadFromFile("/home/mael-archenault/Desktop/Smallworld/src/graphic_resources/arial.ttf")) {
            throw std::runtime_error("Units_Renderer::Units_Renderer: Failed to load font");
        }
        number.setFont(font);
        number.setCharacterSize(72); // in pixels, not points!
        number.setFillColor(sf::Color::White);
        number.setStyle(sf::Text::Bold);

        number.setString("x0");
    };


    void Units_Renderer::set_sprite (std::string species_name) {
        if (!pawn_spritesheet_loaded) {
            if (!pawn_spritesheet.loadFromFile("/home/mael-archenault/Desktop/Smallworld/src/graphic_resources/pawn_spritesheet_255x255.png")) {
                throw std::runtime_error("Units_Renderer::set_species: Failed to load pawn spritesheet");
            }
            pawn_spritesheet_loaded = true;
        }

        int frameWidth = 255;
        int frameHeight = 255;

        int frameX = 0; // third frame (0-based)
        int frameY = 0; // second row

        sf::IntRect speciesFrameRect(frameX * frameWidth, frameY * frameHeight, frameWidth, frameHeight);

        // assign texture and rect to the sprite (ensure the texture outlives the sprite in real code)
        sprite.setTexture(pawn_spritesheet);
        sprite.setTextureRect(speciesFrameRect);

    }
    void Units_Renderer::set_units_number (int n_units) {
        number.setString("x"+std::to_string(n_units));
    }

    void Units_Renderer::scale (float scale_x, float scale_y) {
        this->sprite.setScale(scale_x, scale_y);
    }
    void Units_Renderer::render (sf::RenderWindow& window, sf::Vector2f position) {
        this->sprite.setPosition(position);
        window.draw(this->sprite);
        this->number.setPosition(position.x + this->sprite.getGlobalBounds().width + 5, position.y + this->sprite.getGlobalBounds().height / 2 - this->number.getGlobalBounds().height / 2);
        window.draw(this->number);
    }

};