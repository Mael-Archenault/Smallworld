#include "Token_Renderer.h"
#include "resources_dir.h"
#include <unordered_map>
#include <fstream>
#include <json/json.h>

namespace renderer {

    
    Token_Renderer::Token_Renderer(std::string token_type){


        // loading the spritesheet and its indexing

        if (!spritesheet.loadFromFile(std::string(RESOURCE_DIR) + "/assets/" + token_type + "_spritesheet.png")) {
            throw std::runtime_error("Token_Renderer::set_species: Failed to load " + token_type + " spritesheet");
        }

        // loading json that contains the indexing

        std::string file_name = std::string(RESOURCE_DIR) + "/assets/" + token_type + "_spritesheet_indexing.json";
        std::ifstream file(file_name, std::ifstream::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file: " + file_name);
        }

        Json::Value root;
        file >> root;
        file.close();

        const Json::Value& size = root["sprite_size"];
        const Json::Value& frames  = root["frames"];

        sprite_width = size[0].asInt();
        sprite_height = size[1].asInt();

        for (const auto& species_name : frames.getMemberNames()) {
            std::string name = species_name;
            int frame_x = frames[species_name][0].asInt();
            int frame_y = frames[species_name][1].asInt();
            spritesheet_indexing[name] = std::make_pair(frame_x, frame_y);
        }

        if (!font.loadFromFile(std::string(RESOURCE_DIR) + "/fonts/arial.ttf")) {
            throw std::runtime_error("Token_Renderer::Token_Renderer: Failed to load font");
        }
        number.setFont(font);
        number.setCharacterSize(42);
        number.setFillColor(sf::Color::White);
        number.setStyle(sf::Text::Bold);
    };


    void Token_Renderer::set_sprite (std::string name) {

        int frameWidth = sprite_width;
        int frameHeight = sprite_height;

        int frameX = spritesheet_indexing[name].second;
        int frameY = spritesheet_indexing[name].first;

        sf::IntRect frameRect(frameX * frameWidth, frameY * frameHeight, frameWidth, frameHeight);

        sprite.setTexture(spritesheet);
        sprite.setTextureRect(frameRect);

    }
    void Token_Renderer::set_number (int n) {
        number.setString("x"+std::to_string(n));
    }

    void Token_Renderer::scale (float scale_x, float scale_y) {
        this->sprite.setScale(scale_x, scale_y);
        this->number.setCharacterSize(std::min(42*scale_x, 42*scale_y));
    }
    void Token_Renderer::render (sf::RenderWindow& window, sf::Vector2f position, bool display_number) {
        this->sprite.setPosition(position);
        window.draw(this->sprite);
        if (!display_number) {
            return;
        }
        this->number.setPosition(position.x + this->sprite.getGlobalBounds().width + 5, position.y + this->sprite.getGlobalBounds().height / 2 - this->number.getGlobalBounds().height / 2);
        window.draw(this->number);
    }

    float Token_Renderer::get_sprite_width() {
        return (float)sprite_width;
    }
    float Token_Renderer::get_sprite_height() {
        return (float)sprite_height;
    }

    void Token_Renderer::color (sf::Color color_tint) {
        this->sprite.setColor(color_tint);
    }
};