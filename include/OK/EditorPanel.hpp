
#pragma once

#include <SFML/Audio.hpp>

#include "Panel.hpp"
#include "Map.hpp"
#include "Notation.hpp"
#include "Input.hpp"
#include "Generator.hpp"


namespace OK {

const sf::Color c_notatonStandardColor = sf::Color::Yellow;
const float c_keyframeRadius = 9;      // Radius of each node.
const float c_spaceModifier = 200;      // Spaceing between each node.
const float c_textSize = 20;


class EditorPanel : public Panel {
private:
    std::vector<sf::CircleShape> m_keyframeShapes;
    std::vector<sf::Text> m_timelineStamps;
    sf::Music m_music;
    sf::RectangleShape m_timelineShape;
    sf::RectangleShape m_cursorShape;
    Notation m_notation;

    void createKeyframe(int concurrent);
    void createKeyframeShape(Keyframe k);
    sf::Vector2f getPositionAtTime(float time);

public:
    EditorPanel();

    bool loadMusic(const std::string fileName);
    void loadNotation(const std::string fileName);
    void update(float dt);
    void draw(sf::RenderWindow & window);
    void setUIScale(float scale);
};

} // namespace OK
