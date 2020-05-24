#pragma once

#include <SFML/Graphics.hpp>

namespace OK {

const float c_buttonTimeoutTime = 0.005f;

class Panel {
protected:
float m_UIScale = 1;
static float s_buttonTimeout;


public:
Panel() {
    m_UIScale = 1;
}

// Pure. Require child to implement:
virtual void update(float dt, std::vector<sf::Event::KeyEvent> keyEvents)
{
    if (s_buttonTimeout > 0) {
        s_buttonTimeout -= dt;
    }
}  
virtual void draw(sf::RenderWindow & window) = 0;
virtual void setUIScale(float scale) = 0;

float getUIScale() {
    return m_UIScale;
}

static bool canPressButton() {
    if (s_buttonTimeout <= 0) {
        s_buttonTimeout = c_buttonTimeoutTime; 
        return true;
    }
    return false;
}

};


} // namespace OK
