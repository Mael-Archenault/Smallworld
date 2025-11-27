#include <iostream>
#include <sstream>

#include "renderer.h"
#include "resources_dir.h"

namespace renderer
{

void Text_Box::set_content_with_wrapping()
// wrapping text to avoid it getting out of the box (generated using ChatGPT)
{
    const float       maxWidth = box.getSize().x - 10.f;  // Padding
    const std::string input    = content;

    // Text used only for width measurement
    sf::Text measure(text);

    // ===== 1. TOKENIZE (words, spaces, newlines) =====
    std::vector<std::string> tokens;
    std::string              current;

    for (char c : input)
    {
        if (c == '\n' || c == ' ')
        {
            if (!current.empty())
            {
                tokens.push_back(current);
                current.clear();
            }
            tokens.emplace_back(1, c);  // push "\n" or " "
        }
        else
        {
            current += c;
        }
    }
    if (!current.empty()) tokens.push_back(current);

    // ===== 2. BUILD LINES WITH WRAPPING =====
    std::vector<std::string> lines;
    std::string              line;

    for (const auto& token : tokens)
    {
        // Explicit line break
        if (token == "\n")
        {
            lines.push_back(line);
            line.clear();
            continue;
        }

        // Test if adding token exceeds maxWidth
        std::string candidate = line + token;
        measure.setString(candidate);

        const bool isSpace = (token == " ");

        if (!line.empty() && measure.getLocalBounds().width > maxWidth)
        {
            // Wrap
            lines.push_back(line);
            line = isSpace ? "" : token;  // avoid leading space
        }
        else
        {
            line = candidate;
        }
    }

    if (!line.empty()) lines.push_back(line);

    // ===== 3. JOIN LINES =====
    std::string finalOutput;
    for (size_t i = 0; i < lines.size(); ++i)
    {
        finalOutput += lines[i];
        if (i + 1 < lines.size()) finalOutput += "\n";
    }

    // ===== 4. SET TEXT =====
    text.setString(finalOutput);
}

Text_Box::Text_Box()
{
    // default values
    box.setOutlineThickness(2.0f);
    set_font("arial");
    set_content("");
    set_position({0, 0});
    set_size({200, 100});
    set_character_size(40);
    set_colors(sf::Color(0, 0, 0, 0), sf::Color(0, 0, 0, 0), sf::Color::White);
}

void Text_Box::set_font(std::string font_name)
{
    if (!font.loadFromFile(std::string(RESOURCE_DIR) + "/fonts/" + font_name + ".ttf"))
    {
        throw std::runtime_error("Failed to load font: " + font_name);
    }
    text.setFont(font);
    set_content_with_wrapping();
    set_position(box.getPosition());  // replacing the text inside the box
}

void Text_Box::set_position(sf::Vector2f new_position)
{
    box.setPosition(new_position);
    // placing the text in the center of the box
    sf::FloatRect textbounds = text.getLocalBounds();
    text.setOrigin(textbounds.left + textbounds.width / 2.f,
                   textbounds.top + textbounds.height / 2.f);
    text.setPosition(new_position.x + box.getSize().x / 2.f,
                     new_position.y + box.getSize().y / 2.f);
}

void Text_Box::set_content(std::string new_content)
{
    content = new_content;

    set_content_with_wrapping();
    set_position(box.getPosition());  // replacing the text inside the box
}

void Text_Box::set_character_size(int new_size)
{
    text.setCharacterSize(new_size);
    set_content_with_wrapping();
    set_position(box.getPosition());  // replacing the text inside the box
}

void Text_Box::set_colors(sf::Color line_color, sf::Color fill_color, sf::Color font_color)
{
    box.setOutlineColor(line_color);
    box.setFillColor(fill_color);
    text.setFillColor(font_color);
}

void Text_Box::set_size(sf::Vector2f new_size)
{
    box.setSize(new_size);
    set_content_with_wrapping();
    set_position(box.getPosition());  // replacing the text inside the box
}

void Text_Box::render(sf::RenderWindow& window)
{
    window.draw(box);
    window.draw(text);
}

sf::FloatRect Text_Box::get_rect()
{
    return box.getGlobalBounds();
}

}  // namespace renderer