#pragma once

#include <SFML/Graphics.hpp>		//for Graphics stuff
#include "Panel.hpp"
#include "Input.hpp"

namespace OK {


class Window {
private:
sf::View m_view;
sf::RenderWindow m_window;						// The window that draws the graphics on the screen.
sf::Clock m_deltaTime;
sf::Font m_font;							    // The font imported from file used to print text on Screen.

float m_zoom;
float m_dt;                                     // Delta-time, time between each frame.

public:
Panel* m_activePanel;
Window();
Window(int width, int height);

void update();
void draw();

bool isOpen();
float getDeltatime();

};


} // Namesapce OK