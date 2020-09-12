#include "../include/OK/Window.hpp"

namespace OK {

Window::Window() {}

Window::Window(int width, int height) {
    m_view = sf::View(sf::Vector2f(0,0), sf::Vector2f(width, height));
    m_view.setCenter(width/2.0f, height/2.0f);
    
	m_window.create(sf::VideoMode(width, height), "Beat Saber Map Generator", //opens the window and sets the size.
						 sf::Style::Titlebar | sf::Style::Close);	

	m_window.setView(m_view);
	m_window.setFramerateLimit(60);
    m_window.setKeyRepeatEnabled(false);

	if(m_font.loadFromFile("FONT.ttf"))					//Loads font from file. Gives error in console if
	{
		std::printf("\nLoaded FONT.ttf\n");
	}

    m_zoom = 1;
    m_dt = 0;
    m_activePanel = nullptr;
    std::vector<sf::Event::KeyEvent> m_keyEvents = std::vector<sf::Event::KeyEvent>();
}



void Window::update() {
    Input::update(m_dt, m_window);
    
    // Very jankey but hey better than nothing:
    m_zoom = 1;
    if (Input::isKeyDown(sf::Keyboard::Add)) {
        m_zoom += 0.5f;
    }
    if (Input::isKeyDown(sf::Keyboard::Subtract)) {
        m_zoom -= 0.5f;
    }
    if (Input::isKeyDown(sf::Keyboard::Escape)) {				// ESC to quit.
        m_window.close();
    }
    
    m_dt = m_deltaTime.restart().asSeconds();						//Counts delta-time for consistant movement independent of framerate.

    m_view.zoom(m_zoom);
    m_window.setView(m_view);

    if (m_activePanel != nullptr) {
        m_activePanel->update(m_dt);
    }
        
}

void Window::draw() {

    m_window.clear();													//Clears the canvas.
    if (m_activePanel != nullptr) {
        m_activePanel->draw(m_window);
    }

    m_window.display();												//Sends the buffer to the display.	
}

bool Window::isOpen() {
    return m_window.isOpen();
}

float Window::getDeltatime() {
    return m_dt;
}


} // Namesapce OK