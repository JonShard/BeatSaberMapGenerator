#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iterator>

namespace OK {

const float c_downKeyWait = 0.02f;      // Time a key waits for other keys to be pressed.

// Input is an extention of SFML's input system.
// It's main function is to keep track of which keys are pressed at the same time, as SFML didn't support this.
class Input {
public:
static float s_scrollDelta;
static std::vector<std::pair<float, sf::Event::KeyEvent>> s_keys; // <time held down(-1 if up), key event>
static std::vector<sf::Event::KeyEvent> s_downKeys; // <time held down(-1 if up), key event>

static int indexOfKey(sf::Keyboard::Key key);
static bool isKeyDown(sf::Keyboard::Key key);
static void update(float dt, sf::RenderWindow & w);

};

} // Namespace OK