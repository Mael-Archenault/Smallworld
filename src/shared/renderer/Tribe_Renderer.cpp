#include "Tribe_Renderer.h"
#include "resources_dir.h"
#include <unordered_map>
#include <fstream>
#include <json/json.h>






namespace renderer {

    Tribe_Renderer::Tribe_Renderer(){
        // loading the spritesheet and its indexing
       
        if (!species_spritesheet.loadFromFile(std::string(RESOURCE_DIR) + "/assets/species_spritesheet.png")) {
            throw std::runtime_error("Tribe_Renderer::set_species: Failed to load species spritesheet");
        }
        if (!power_spritesheet.loadFromFile(std::string(RESOURCE_DIR) + "/assets/power_spritesheet.png")) {
            throw std::runtime_error("Tribe_Renderer::set_species: Failed to load power spritesheet");
        }

        species_spritesheet_indexing = open_indexing_file(std::string(RESOURCE_DIR) + "/assets/species_spritesheet_indexing.json");
        power_spritesheet_indexing = open_indexing_file(std::string(RESOURCE_DIR) + "/assets/power_spritesheet_indexing.json");


        
    };


    void Tribe_Renderer::set_sprite (std::string species_name, std::string power_name, bool in_decline) {
        if (species_spritesheet_indexing.find(species_name) == species_spritesheet_indexing.end()) {
            //throw std::runtime_error("Tribe_Renderer::set_sprite: Species name not found in indexing: " + species_name);
            species_name = "Dwarves";
        }
        if (power_spritesheet_indexing.find(power_name) == power_spritesheet_indexing.end()) {
            //throw std::runtime_error("Tribe_Renderer::set_sprite: Power name not found in indexing: " + power_name);
            power_name = "Blank";
        }
        sf::Sprite species_sprite;

        int frameWidth = 360;
        int frameHeight = 194;

        int frameX = species_spritesheet_indexing[species_name].second; 
        int frameY = species_spritesheet_indexing[species_name].first;

        sf::IntRect speciesFrameRect(frameX * frameWidth, frameY * frameHeight, frameWidth, frameHeight);

        // assign texture and rect to the sprite (ensure the texture outlives the sprite in real code)
        species_sprite.setTexture(species_spritesheet);
        species_sprite.setTextureRect(speciesFrameRect);


        sf::Sprite power_sprite;

        frameWidth = 194;
        frameHeight = 194;

        frameX = power_spritesheet_indexing[power_name].second;
        frameY = power_spritesheet_indexing[power_name].first;

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
        if (in_decline) {
            this->sprite.setColor(sf::Color(39, 224, 245));
        } else {
            this->sprite.setColor(sf::Color(255,255,255));
        }
    }

    void Tribe_Renderer::scale (float scale_x, float scale_y) {
        this->sprite.setScale(scale_x, scale_y);
    }
    void Tribe_Renderer::render (sf::RenderTarget& window, sf::Vector2f position) {
        this->sprite.setPosition(position);
        window.draw(this->sprite);
    }


    std::unordered_map<std::string, std::pair<int, int>> Tribe_Renderer::open_indexing_file(std::string file_name) {
        std::unordered_map<std::string, std::pair<int, int>> indexing;


        
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
            indexing[name] = std::make_pair(frame_x, frame_y);
        }
        return indexing;
    }
};