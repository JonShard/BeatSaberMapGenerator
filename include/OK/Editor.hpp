#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../OK/Map.hpp"


namespace OK {

const sf::Color c_notatonColor = sf::Color::Cyan;
const float c_notationRadius = 30;

class Editor : public sf::Music {
    Map m_map;
    std::vector<sf::CircleShape> shapes;


public:
    float m_UIScale = 1; // TODO: Getter, setter updates shapes rad pos.

    Editor();

    std::vector<sf::CircleShape>* getDrawListAdress();
    void loadSong(std::string songFile);
    void loadAnnotation(std::string annotationFile);
    void createNotation(Notation type, float time);
    void saveAnnotation();

};



} // namespace OK
