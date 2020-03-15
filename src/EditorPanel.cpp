#include "../include/OK/EditorPanel.hpp"

namespace OK {

EditorPanel::EditorPanel() : Panel(), Editor() {
    m_timePos = 0;

}

void EditorPanel::update(float dt) {

}

void EditorPanel::draw(sf::Window & window) {

}

void EditorPanel::setUIScale(float scale) {

}

// void Editor::createNotation(Notation type, float time) {

//     sf::CircleShape shape(c_notationRadius * m_UIScale);
//     shape.setPosition(c_windowWidth / 2, c_windowHeight / 2 + time * m_UIScale);
//     shape.setFillColor(c_notatonColor);
//     shapes.push_back(shape);
// }


} // namespace OK