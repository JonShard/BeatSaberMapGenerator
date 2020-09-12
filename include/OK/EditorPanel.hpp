
#pragma once

#include "Panel.hpp"
#include "Editor.hpp"
#include "Map.hpp"
#include "Input.hpp"
#include "Generator.hpp"

namespace OK {

const sf::Color c_notatonStandardColor = sf::Color::Yellow;
const float c_notationRadius = 9;      // Radius of each node.
const float c_spaceModifier = 200;      // Spaceing between each node.
const float c_textSize = 20;


class EditorPanel : public Panel, public Editor {
private:
    std::vector<sf::CircleShape> m_notationShapes;
    std::vector<sf::Text> m_timelineStamps;
    sf::RectangleShape m_timelineShape;
    sf::RectangleShape m_cursorShape;

public:
    EditorPanel();

    sf::Vector2f getCurrentPosition();
    void update(float dt);
    void draw(sf::RenderWindow & window);
    void setUIScale(float scale);
    void createNotation(NotationType type, int concurrent);
    bool openFromFile(const std::string fileName);
};

} // namespace OK
