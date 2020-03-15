#include "../include/OK/Editor.hpp"

namespace OK {

extern float c_windowWidth;
extern float c_windowHeight;

Editor::Editor() : sf::Music() {

    shapes = std::vector<sf::CircleShape>();
    m_map = Map();
    createNotation(Notation::SINGLE, 0);

}

std::vector<sf::CircleShape>* Editor::getDrawListAdress() {
    return & shapes;
}

void Editor::createNotation(Notation type, float time) {

    sf::CircleShape shape(c_notationRadius * m_UIScale);
    shape.setPosition(c_windowWidth / 2, c_windowHeight / 2 + time * m_UIScale);
    shape.setFillColor(c_notatonColor);
    shapes.push_back(shape);
}



} // namespace OK