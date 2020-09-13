#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Map.hpp"
#include "Notation.hpp"

namespace OK {

class Editor : public sf::Music {
protected:
    std::string m_songName;
    std::vector<Keyframe> m_keyframes; // <timestamp, type> 
    // TODO: autosave every N minutes

public:
    Editor();

    void loadSong(std::string songFile);
    void loadKeyframe(std::string keyframeFile);
    void createKeyframe(Keyframe keyframe);
    void saveKeyframe();
};



} // namespace OK
