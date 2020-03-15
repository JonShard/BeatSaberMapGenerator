
#pragma once

#include "Panel.hpp"
#include "Editor.hpp"
#include "Map.hpp"

namespace OK {

const sf::Color c_notatonStatndardColor = sf::Color::Yellow;
const sf::Color c_notatonSpecialColor = sf::Color::Cyan;
const float c_notationRadius = 30;
const float c_textSize = 20;

class EditorPanel : public Panel, public Editor {
private:
    float m_timePos;
    std::vector<sf::CircleShape> m_notationShapes;

public:
    
    EditorPanel();

    void update(float dt);
    void draw(sf::Window & window);
    void setUIScale(float scale);

};

} // namespace OK
