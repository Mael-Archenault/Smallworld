#include "Tribe_Renderer.h"


namespace renderer {

    bool Tribe_Renderer::power_spritesheet_loaded = false;
    bool Tribe_Renderer::species_spritesheet_loaded = false;

    sf::Texture Tribe_Renderer::species_spritesheet;
    sf::Texture Tribe_Renderer::power_spritesheet;

    Tribe_Renderer::Tribe_Renderer(){
    };


    void Tribe_Renderer::set_sprite (std::string species_name, std::string power_name) {
        sf::Sprite species_sprite;
        if (!species_spritesheet_loaded) {
            if (!species_spritesheet.loadFromFile("/home/mael-archenault/Desktop/Smallworld/src/graphic_resources/species_card_360x194.png")) {
                throw std::runtime_error("Tribe_Renderer::set_species: Failed to load species spritesheet");
            }
            species_spritesheet_loaded = true;
        }

        int frameWidth = 360;
        int frameHeight = 194;

        int frameX = 0; // third frame (0-based)
        int frameY = 0; // second row

        sf::IntRect speciesFrameRect(frameX * frameWidth, frameY * frameHeight, frameWidth, frameHeight);

        // assign texture and rect to the sprite (ensure the texture outlives the sprite in real code)
        species_sprite.setTexture(species_spritesheet);
        species_sprite.setTextureRect(speciesFrameRect);


        sf::Sprite power_sprite;
        if (!power_spritesheet_loaded) {
            if (!power_spritesheet.loadFromFile("/home/mael-archenault/Desktop/Smallworld/src/graphic_resources/powers_194x194.png")) {
                throw std::runtime_error("Tribe_Renderer::set_power: Failed to load power spritesheet");
            }
            power_spritesheet_loaded = true;
        }

        frameWidth = 194;
        frameHeight = 194;

        frameX = 0; // first frame (0-based)
        frameY = 0; // first row

        sf::IntRect powerFrameRect(frameX * frameWidth, frameY * frameHeight, frameWidth, frameHeight);

        // assign texture and rect to the sprite (ensure the texture outlives the sprite in real code)
        power_sprite.setTexture(power_spritesheet);
        power_sprite.setTextureRect(powerFrameRect);


        this->texture.create(360+194, 194);
        this->texture.clear(sf::Color::Transparent);
        power_sprite.setPosition(sf::Vector2f(0,0));
        this->texture.draw(power_sprite);
        species_sprite.setPosition(sf::Vector2f(power_sprite.getGlobalBounds().width-50,0));
        this->texture.draw(species_sprite);

        this->texture.display();
        this->sprite.setTexture(this->texture.getTexture());
    }

    void Tribe_Renderer::scale (float scale_x, float scale_y) {
        this->sprite.setScale(scale_x, scale_y);
    }
    void Tribe_Renderer::render (sf::RenderWindow& window, sf::Vector2f position) {
        this->sprite.setPosition(position);
        window.draw(this->sprite);
    }

};