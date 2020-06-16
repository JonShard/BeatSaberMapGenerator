#include "../include/OK/Editor.hpp"

namespace OK {

Editor::Editor() : sf::Music(){
    m_notations = std::vector<Notation>();
}

void Editor::createNotation(Notation notation) {
    m_notations.push_back(notation);
}

void Editor::saveAnnotation() {
    
}

} // namespace OK