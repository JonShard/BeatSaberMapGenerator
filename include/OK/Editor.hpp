#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Map.hpp"

namespace OK {

class Editor : public sf::Music {
protected:
    std::string m_songName;
    std::vector<Notation> m_notations; // <timestamp, type> 
    // TODO: autosave every N minutes

public:
    Editor();

    void loadSong(std::string songFile);
    void loadAnnotation(std::string annotationFile);
    void createNotation(Notation notation);
    void saveAnnotation();

};



} // namespace OK
