#pragma once

#include <SFML/Audio.hpp>

#include "Panel.hpp"
#include "Song.hpp"
#include "Input.hpp"
#include "Generator.hpp"


namespace OK {

const sf::Color c_notatonStandardColor = sf::Color::Yellow;
const float c_keyframeRadius = 9;      // Radius of each node.
const float c_spaceModifier = 200;     // Spaceing between each node.
const float c_textSize = 20;
const float c_autoSavePeriod = 60;     // Seconds

// EditorPanel is the user interface to create Keyframes which are used by the Generator to make a Map.
class EditorPanel : public Panel {
private:
    std::vector<sf::CircleShape> m_keyframeShapes;
    std::vector<sf::Text> m_timelineStamps;
    sf::Music m_music;
    sf::RectangleShape m_timelineShape;
    sf::RectangleShape m_cursorShape;
    Song m_song;
    float m_autoSaveCountdown;

    void createKeyframe(int concurrent);
    void createKeyframeShape(Keyframe k);
    sf::Vector2f getPositionAtTime(float time);

    void initSFML();

public:
    EditorPanel();
    EditorPanel(const std::string songFile, const std::string notationFile);

    bool loadMusic(const std::string fileName);
    void loadNotation(Notation notation);
    void update(float dt);
    void draw(sf::RenderWindow & window);
    void setUIScale(float scale);
};

} // namespace OK
