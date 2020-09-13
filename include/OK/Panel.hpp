// Base class for all UI panels. 
// A panel typically contains lists of visual elements which it updates based on input from mouse and keyboard.
#pragma once

#include <SFML/Graphics.hpp>

namespace OK {

class Panel {
protected:
float m_UIScale = 1;

public:
Panel() {
    m_UIScale = 1;
}

// Pure. Require child to implement:
virtual void update(float dt) = 0;
virtual void draw(sf::RenderWindow & window) = 0;
virtual void setUIScale(float scale) = 0;

float getUIScale() {
    return m_UIScale;
}

};


} // namespace OK
