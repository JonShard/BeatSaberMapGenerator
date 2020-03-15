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


	if(m_font.loadFromFile("FONT.ttf"))					//Loads font from file. Gives error in console if
	{
		std::printf("\nLoaded FONT.ttf\n");
	}

    m_dt = 0;
    m_buttonTimeout = 0;
    m_activePanel = nullptr;
}

void Window::update() {

    sf::Event event;
        while(m_window.pollEvent(event)) {
            if(m_buttonTimeout <= 0) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {				// ESC to quit.
                    m_window.close();
                    m_buttonTimeout = c_buttonTimeoutTime;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
                    m_buttonTimeout = c_buttonTimeoutTime;
                    // Create notation
                }
                
            }
            if (event.type == sf::Event::Closed) {								//If the event happening is closed: {															//then close the window as well.
                m_window.close();
            }

            m_dt = m_deltaTime.restart().asSeconds();						//Counts delta-time for consistant movement independent of framerate.
            if (m_buttonTimeout > 0) {
                m_buttonTimeout -= m_dt;
            }

            

            // Set view pos
            // cam.setCenter(players[0]-> getPos());
            m_window.setView(m_view);
            //m_view.zoom(0.6f);

            if (m_activePanel != nullptr) {
            m_activePanel->update(m_dt);
        }
            
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