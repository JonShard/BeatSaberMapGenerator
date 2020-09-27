#include "../include/OK/EditorPanel.hpp"



namespace OK {

EditorPanel::EditorPanel() : Panel() {
    m_timelineShape = sf::RectangleShape(sf::Vector2f(5, -MAXFLOAT - 1));
    m_timelineShape.setPosition(sf::Vector2f(Config::editor.windowWidth / 3, Config::editor.windowHeight));
    m_timelineShape.setFillColor(sf::Color::White);

    m_cursorShape = sf::RectangleShape(sf::Vector2f(Config::editor.windowWidth, 4));
    m_cursorShape.setOrigin(sf::Vector2f(Config::editor.windowWidth / 2, 2));
    m_cursorShape.setFillColor(sf::Color(60, 60, 60));
    m_notation = Notation();
    m_autoSaveCountdown = c_autoSavePeriod;
}

bool EditorPanel::loadMusic(const std::string fileName) {
    m_notation = Notation(Util::removeFileExtention(fileName, ".ogg") + "_notation.json");
    return m_music.openFromFile(fileName);
}

void EditorPanel::loadNotation(const std::string fileName) {
    m_notation = Notation(fileName);
    m_notation.load(fileName);
    for (Keyframe k : m_notation.m_keyframes) {
        createKeyframeShape(k);
    }
}


void EditorPanel::update(float dt) {
    if (Config::editor.autosaveEnabled) {
        m_autoSaveCountdown -= dt;
        if (m_autoSaveCountdown < 0) {
            m_autoSaveCountdown = c_autoSavePeriod;
            m_notation.save();
        }
    }

    m_cursorShape.setPosition(getPositionAtTime(m_music.getPlayingOffset().asSeconds()));
    if (Input::isKeyDown(sf::Keyboard::Space)) {
        if (m_music.getStatus() == m_music.Playing) {
            m_music.pause();
        }
        else {
            m_music.play();
        }
    }
    if (Input::isKeyDown(sf::Keyboard::BackSpace)) {
        m_music.setPlayingOffset(sf::seconds(0));
    }
    if (Input::s_scrollDelta) { // 1 or -1 if moved.
        m_music.setPlayingOffset(std::max(sf::seconds(0), m_music.getPlayingOffset() + sf::seconds(1 * Input::s_scrollDelta)));
    }
    if (Input::isKeyDown(sf::Keyboard::PageUp)) {
        m_music.setPitch(m_music.getPitch() + 0.1f);
    }
    if (Input::isKeyDown(sf::Keyboard::PageDown)) {
        m_music.setPitch(m_music.getPitch() - 0.1f);
    }
    if (Input::isKeyDown(sf::Keyboard::F5)) {
        m_notation.save();
    }
    if (Input::isKeyDown(sf::Keyboard::F10)) {  // TODO: Make new panel for generating map.
        Map map = Generator::GenerateMap(m_notation);
        map.save();
    }

    int concurrent = 0;
    for (auto k : Input::s_downKeys) {
        if (k.code > 0 && k.code < 36)     // A-Z + 0-9
            concurrent++;
    }
    if (concurrent > 0) {
        createKeyframe(concurrent);
    }
}

void EditorPanel::draw(sf::RenderWindow & w) { 
    sf::View view = w.getView();
    view.setCenter(getPositionAtTime(m_music.getPlayingOffset().asSeconds()));
    w.setView(view);

    w.draw(m_cursorShape);
    w.draw(m_timelineShape);
    for (sf::CircleShape s : m_keyframeShapes) {
        w.draw(s);
    }
}

void EditorPanel::setUIScale(float scale) {

}

void EditorPanel::createKeyframe(int concurrent) {
    Keyframe k;
    k.time = m_music.getPlayingOffset().asSeconds(); //TODO: Fix BPM not considered.
    k.concurrent = concurrent;
    if (m_notation.m_keyframes.size() > 0) 
        k.id = m_notation.m_keyframes.back().id + 1; //TODO: duplicates possible!!
    else
        k.id = 0;

    createKeyframeShape(k);
    m_notation.m_keyframes.push_back(k);
}

void EditorPanel::createKeyframeShape(Keyframe k) {
    printf("Creating keyframe, id %ld \ttime: %f \tconcurrent: %d\n", m_notation.m_keyframes.size(), k.time, k.concurrent);
    sf::CircleShape shape(((k.concurrent > 1) ? c_keyframeRadius * 1.5f : c_keyframeRadius) * m_UIScale);
    shape.setPosition(getPositionAtTime(k.time)); // -1 cuz top of screen is 0.
    shape.setFillColor(c_notatonStandardColor);
    m_keyframeShapes.push_back(shape);

}

sf::Vector2f EditorPanel::getPositionAtTime(float time) {
    return sf::Vector2f(Config::editor.windowWidth / 2, Config::editor.windowHeight / 2 + time * c_spaceModifier * m_UIScale * -1);
}



} // namespace OK