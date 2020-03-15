#pragma once

#include <SFML/Graphics.hpp>		//for Graphics stuff

namespace OK {

const float c_buttonTimeoutTime = 0.05f;

class Window {
sf::View m_view;
sf::RenderWindow m_window;						//The window that draws the graphics on the screen.
sf::Clock m_deltaTime;
sf::Font m_font;									//The font imported from file used to print text on Screen.

float m_dt;
float m_buttonTimeout;

public:
std::vector<sf::CircleShape>* m_drawList;

Window();
Window(int width, int height);

void update();
void draw();

bool isOpen();

float getDeltatime();

};


} // Namesapce OK