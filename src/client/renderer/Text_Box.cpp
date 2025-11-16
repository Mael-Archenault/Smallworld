#include <sstream>

#include "renderer.h"
#include "resources_dir.h"

namespace renderer
{

void Text_Box::wrap_text()  // used for managing word wrap and alignment
// generated using chatGPT with some manual adjustments
{
    float       maxWidth = box.getSize().x - 10.f;  // padding
    std::string original = text.getString();
    std::string wrapped;

    std::string       word;
    std::stringstream ss(original);

    // ---------- WORD WRAP ----------
    while (ss >> word)
    {
        std::string testLine = wrapped;
        if (!wrapped.empty()) testLine += " ";
        testLine += word;

        text.setString(testLine);

        if (text.getLocalBounds().width > maxWidth)
        {
            if (!wrapped.empty()) wrapped += "\n";
            wrapped += word;
        }
        else
        {
            if (!wrapped.empty()) wrapped += " ";
            wrapped += word;
        }
    }

    // Now "wrapped" contains the raw wrapped text with '\n'

    // ---------- SPLIT INTO LINES ----------
    std::vector<std::string> lines;
    {
        std::stringstream ls(wrapped);
        std::string       line;
        while (std::getline(ls, line, '\n')) lines.push_back(line);
    }

    // ---------- ALIGN EACH LINE ----------
    std::string finalText;
    for (size_t i = 0; i < lines.size(); ++i)
    {
        text.setString(lines[i]);
        float lineWidth = text.getLocalBounds().width;
        float spaceWidth;

        // Measure width of a single space (used for indentation)
        text.setString(" ");
        spaceWidth = text.getLocalBounds().width;

        int spacesToAdd = 0;

        if (alignment == "right")
        {
            spacesToAdd = static_cast<int>((maxWidth - lineWidth) / spaceWidth);
        }
        else if (alignment == "center")
        {
            spacesToAdd = static_cast<int>(((maxWidth - lineWidth) / 2.f) / spaceWidth);
        }
        else
        {
            // left alignment → no padding
            spacesToAdd = 0;
        }

        // Build aligned line
        finalText += std::string(std::max(0, spacesToAdd), ' ') + lines[i];

        if (i < lines.size() - 1) finalText += "\n";
    }

    // ---------- APPLY ----------
    text.setString(finalText);
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
    set_alignment("left");
}

void Text_Box::set_font(std::string font_name)
{
    if (!font.loadFromFile(std::string(RESOURCE_DIR) + "/fonts/" + font_name + ".ttf"))
    {
        throw std::runtime_error("Failed to load font: " + font_name);
    }
    text.setFont(font);
    wrap_text();
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
    wrap_text();
}

void Text_Box::set_content(std::string new_content)
{
    text.setString(new_content);
    wrap_text();
    set_position(box.getPosition());  // replacing the text inside the box
}

void Text_Box::set_character_size(int new_size)
{
    text.setCharacterSize(new_size);
    wrap_text();
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
    wrap_text();
    set_position(box.getPosition());  // replacing the text inside the box
}

void Text_Box::set_alignment(std::string new_alignment)
{
    alignment = new_alignment;
    wrap_text();
    set_position(box.getPosition());  // replacing the text inside the box
}

void Text_Box::render(sf::RenderWindow& window)
{
    window.draw(box);

    // sf::Text text;
    // text.setFont(font);
    // text.setString(content);
    // text.setCharacterSize(static_cast<unsigned int>(size.y * 0.6f));
    // text.setFillColor(sf::Color::White);

    // // Center the text within the box
    // sf::FloatRect textBounds = text.getLocalBounds();
    // text.setOrigin(textBounds.left + textBounds.width / 2.0f,
    //                textBounds.top + textBounds.height / 2.0f);
    // text.setPosition(position.x + size.x / 2.0f, position.y + size.y / 2.0f);

    window.draw(text);
}

}  // namespace renderer