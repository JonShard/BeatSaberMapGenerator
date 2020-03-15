#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Map.hpp"


namespace OK {

class Editor : public sf::Music {
private:
    std::map<float, Notation> m_notations; // <timestamp, type> 

public:
    Editor();

    void loadSong(std::string songFile);
    void loadAnnotation(std::string annotationFile);
    void createNotation(Notation type, float time);
    void saveAnnotation();

};



} // namespace OK
