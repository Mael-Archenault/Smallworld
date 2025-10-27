#include "Units_Renderer.h"
#include <unordered_map>
#include <fstream>
#include <json/json.h>

namespace renderer {

    bool Units_Renderer::spritesheets_loaded = false;
    
    sf::Texture Units_Renderer::pawn_spritesheet;
    std::unordered_map<std::string, std::pair<int, int>> Units_Renderer::spritesheet_indexing;
    
    Units_Renderer::Units_Renderer(){

        // loading the spritesheet and its indexing
        if (!spritesheets_loaded) {
            if (!pawn_spritesheet.loadFromFile("/home/mael-archenault/Desktop/Smallworld/src/graphic_resources/pawn_spritesheet_150x150.png")) {
                throw std::runtime_error("Units_Renderer::set_species: Failed to load pawn spritesheet");
            }
            spritesheets_loaded = true;

           // loading json that contains the indexing

            std::string file_name = "/home/mael-archenault/Desktop/Smallworld/src/graphic_resources/pawn_spritesheet_indexing.json";
            std::ifstream file(file_name, std::ifstream::binary);
            if (!file.is_open()) {
                throw std::runtime_error("Cannot open file: " + file_name);
            }

            Json::Value root;
            file >> root;
            file.close();

            const Json::Value& size = root["sprite_size"];
            const Json::Value& frames  = root["frames"];

            for (const auto& species_name : frames.getMemberNames()) {
                std::string name = species_name;
                int frame_x = frames[species_name][0].asInt();
                int frame_y = frames[species_name][1].asInt();
                spritesheet_indexing[name] = std::make_pair(frame_x, frame_y);
            }

        }




        set_sprite("lost_tribe");
        if (!font.loadFromFile("/home/mael-archenault/Desktop/Smallworld/src/graphic_resources/arial.ttf")) {
            throw std::runtime_error("Units_Renderer::Units_Renderer: Failed to load font");
        }
        number.setFont(font);
        number.setCharacterSize(64); // in pixels, not points!
        number.setFillColor(sf::Color::White);
        number.setStyle(sf::Text::Bold);

        number.setString("x0");
    };


    void Units_Renderer::set_sprite (std::string species_name) {

        int frameWidth = 150;
        int frameHeight = 150;

        int frameX = spritesheet_indexing[species_name].second;
        int frameY = spritesheet_indexing[species_name].first;

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