#include "../include/OK/Editor.hpp"

namespace OK {

Editor::Editor() : sf::Music(){
    m_keyframes = std::vector<Keyframe>();
}

void Editor::createKeyframe(Keyframe keyframe) {
    m_keyframes.push_back(keyframe);
}

void Editor::saveKeyframe() {
    
    printf("Saving annontation to file.\n");
}

} // namespace OK