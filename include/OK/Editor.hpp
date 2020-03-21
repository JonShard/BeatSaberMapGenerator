#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Map.hpp"


namespace OK {

enum NotationType { SINGLE, DOUBLE, SPECIAL };

struct Notation {
    int id;
    float time;
    NotationType type;
};

class Editor : public sf::Music {
protected:
    std::vector<Notation> m_notations; // <timestamp, type> 

public:
    Editor();

    void loadSong(std::string songFile);
    void loadAnnotation(std::string annotationFile);
    void createNotation(Notation notation);
    void saveAnnotation();

};



} // namespace OK
