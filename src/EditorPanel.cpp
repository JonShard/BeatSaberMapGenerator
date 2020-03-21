#include "../include/OK/EditorPanel.hpp"



namespace OK {

extern int c_windowWidth;
extern int c_windowHeight;

EditorPanel::EditorPanel() : Panel(), Editor() {
    m_timelineShape = sf::RectangleShape(sf::Vector2f(5, c_windowHeight));
    m_timelineShape.setPosition(sf::Vector2f(c_windowWidth/3, 0));
    m_timelineShape.setFillColor(sf::Color::White);
}

void EditorPanel::update(float dt, std::vector<sf::Event::KeyEvent> keyEvents) {
    for (sf::Event::KeyEvent e : keyEvents) {
        if (e.code == sf::Keyboard::F && e.code == sf::Keyboard::J) {
            createNotation(Editor::getPlayingOffset().asSeconds(), NotationType::DOUBLE); //TODO: Will never happen!!
        } else if (e.code == sf::Keyboard::F || e.code == sf::Keyboard::J) {
            createNotation(Editor::getPlayingOffset().asSeconds(), NotationType::SINGLE);
        }  
    }
}

void EditorPanel::draw(sf::RenderWindow & window) {    //TODO: null check.
    window.draw(m_timelineShape);
    for (sf::CircleShape s : m_notationShapes) {
        window.draw(s);
    }
}

void EditorPanel::setUIScale(float scale) {

}

void EditorPanel::createNotation(float time, NotationType type) {

    printf("Creating notation\n");
    Notation notation;

    if (m_notations.size() > 0) {
        notation.id = m_notations.back().id + 1; //TODO: duplicates possible!!
    }
    notation.time = time;
    notation.type = type;

    sf::CircleShape shape(((notation.type == NotationType::DOUBLE) ? c_notationRadius * 1.5f : c_notationRadius) * m_UIScale);
    shape.setPosition(c_windowWidth / 2, c_windowHeight / 2 + notation.time * c_spaceModifier * m_UIScale * -1); // -1 cuz top of screen is 0.
    shape.setFillColor((notation.type == NotationType::SPECIAL) ? c_notatonSpecialColor: c_notatonStandardColor);
    m_notationShapes.push_back(shape);

    Editor::createNotation(notation);
}



} // namespace OK