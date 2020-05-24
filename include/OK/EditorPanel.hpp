
#pragma once

#include "Panel.hpp"
#include "Editor.hpp"
#include "Map.hpp"

namespace OK {

const sf::Color c_notatonStandardColor = sf::Color::Yellow;
const sf::Color c_notatonSpecialColor = sf::Color::Cyan;
const float c_notationRadius = 9;      // Radius of each node.
const float c_spaceModifier = 200;      // Spaceing between each node.
const float c_textSize = 20;


class EditorPanel : public Panel, public Editor {
private:
    std::vector<sf::CircleShape> m_notationShapes;
    std::vector<sf::Text> m_timelineStamps;
    sf::RectangleShape m_timelineShape;

public:
    EditorPanel();

    sf::Vector2f getCurrentPosition();
    void update(float dt, std::vector<sf::Event::KeyEvent> keyEvents);
    void draw(sf::RenderWindow & window);
    void setUIScale(float scale);
    void createNotation(NotationType type, int concurrent);
};

} // namespace OK
