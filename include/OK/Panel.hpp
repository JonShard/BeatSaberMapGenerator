#pragma once

#include <SFML/Graphics.hpp>

namespace OK {

class Panel {
private:
    float m_UIScale;

public:
    Panel() {}

    // Pure. Require child to implement:
    virtual void update(float dt) = 0;  
    virtual void draw(sf::Window & window) = 0;
    virtual void setUIScale(float scale) = 0;
    
    float getUIScale() {
        return m_UIScale;
    }
};

} // namespace OK
