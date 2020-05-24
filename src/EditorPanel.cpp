#include "../include/OK/EditorPanel.hpp"



namespace OK {

extern int c_windowWidth;
extern int c_windowHeight;

EditorPanel::EditorPanel() : Panel(), Editor() {
    m_timelineShape = sf::RectangleShape(sf::Vector2f(5, -MAXFLOAT-1));
    m_timelineShape.setPosition(sf::Vector2f(c_windowWidth/3, c_windowHeight));
    m_timelineShape.setFillColor(sf::Color::White);
}

sf::Vector2f EditorPanel::getCurrentPosition() {
    return sf::Vector2f(c_windowWidth / 2, c_windowHeight / 2 + Editor::getPlayingOffset().asSeconds() * c_spaceModifier * m_UIScale * -1);
}

void EditorPanel::update(float dt, std::vector<sf::Event::KeyEvent> keyEvents) {
    Panel::update(dt, keyEvents);


    int concurrent = 0;
    bool special = false;
    for (sf::Event::KeyEvent e : keyEvents) {
        if (e.code == sf::Keyboard::Space) {
            if (Editor::getStatus() == Editor::Playing) {
                Editor::pause();
                printf("Paused\n");
            }
            else {
                Editor::play();
                printf("Play\n");
            }
        }

        if (e.code > 0 && e.code < 36) {    // A-Z + 0-9
            concurrent++;
            
        }
        if (e.code > 25 && e.code < 36) {   // 0-9
            special = true;
        }
    }
    if (special == true) {
        createNotation(NotationType::SPECIAL, concurrent);
    } else if (concurrent == 1) {
        createNotation(NotationType::SINGLE, concurrent);
    } else if (concurrent > 1) {
        createNotation(NotationType::DOUBLE, concurrent);
    }
    
}

void EditorPanel::draw(sf::RenderWindow & w) { 
    sf::View view = w.getView();
    view.setCenter(getCurrentPosition());
    w.setView(view);

    w.draw(m_timelineShape);
    for (sf::CircleShape s : m_notationShapes) {
        w.draw(s);
    }
}

void EditorPanel::setUIScale(float scale) {

}

void EditorPanel::createNotation(NotationType type, int concurrent) {

    Notation n;

    if (m_notations.size() > 0) 
        n.id = m_notations.back().id + 1; //TODO: duplicates possible!!
    else
        n.id = 0;

    n.time = Editor::getPlayingOffset().asSeconds();
    n.concurrent = concurrent;
    n.type = type;
    printf("Creating notation, id %ld \ttime: %f \tconcurrent: %d\n", m_notations.size(), n.time, n.concurrent);

    sf::CircleShape shape(((n.type == NotationType::DOUBLE) ? c_notationRadius * 1.5f : c_notationRadius) * m_UIScale);
    shape.setPosition(getCurrentPosition()); // -1 cuz top of screen is 0.
    shape.setFillColor((n.type == NotationType::SPECIAL) ? c_notatonSpecialColor: c_notatonStandardColor);
    m_notationShapes.push_back(shape);

    Editor::createNotation(n);
}



} // namespace OK