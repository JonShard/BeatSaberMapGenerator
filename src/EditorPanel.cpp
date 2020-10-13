#include "../include/OK/EditorPanel.hpp"

namespace OK {

EditorPanel::EditorPanel() : Panel() {
    initSFML();
    m_autoSaveCountdown = c_autoSavePeriod;
}

EditorPanel::EditorPanel(std::string songFile, std::string notationFile) : Panel() {
    m_autoSaveCountdown = c_autoSavePeriod;
    initSFML();

    if (loadMusic(songFile)) {
        printf("Loaded song %s\n", songFile.data());
    }

    OK::Song song(songFile);
    song.setSongFileName(songFile);
    if (!OK::Util::isFileExtention(songFile, ".ogg")) {
        printf("Error: Unexpected file extention: %s\nExpected .ogg\n", songFile.data());
        return;
    }
    if (notationFile.length() > 0) {
        if (!OK::Util::isFileExtention(notationFile, ".json")) {
            printf("Error: Unexpected file extention: %s\nExpected .json\n", notationFile.data());
            return;
        }
        song.getNotation(song.loadNotation(notationFile));
        printf("Loaded notation %s\n", notationFile.data());
    }
    if (song.getNotationCount() == 0) {
        song.addNotation(OK::Notation("Expert"));
    }
    m_song = song;
}

void EditorPanel::initSFML() {
    m_timelineShape = sf::RectangleShape(sf::Vector2f(5, -MAXFLOAT - 1));
    m_timelineShape.setPosition(sf::Vector2f(Config::editor.windowWidth / 3, Config::editor.windowHeight));
    m_timelineShape.setFillColor(sf::Color::White);

    m_cursorShape = sf::RectangleShape(sf::Vector2f(Config::editor.windowWidth, 4));
    m_cursorShape.setOrigin(sf::Vector2f(Config::editor.windowWidth / 2, 2));
    m_cursorShape.setFillColor(sf::Color(60, 60, 60));
}

bool EditorPanel::loadMusic(const std::string fileName) {
    return m_music.openFromFile(fileName);
}

void EditorPanel::loadNotation(Notation notation) {
    for (Keyframe k : m_song.getNotation(0)->m_keyframes) {
        createKeyframeShape(k);
    }
}

void EditorPanel::update(float dt) {
    if (Config::editor.autosaveEnabled) {
        m_autoSaveCountdown -= dt;
        if (m_autoSaveCountdown < 0) {
            m_autoSaveCountdown = c_autoSavePeriod;
            m_song.saveNotation(0);
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
    if (Input::isKeyDown(sf::Keyboard::End)) {
        m_music.setPitch(1);
    }
    if (Input::isKeyDown(sf::Keyboard::F5)) {
        m_song.saveNotation(0);
    }
    if (Input::isKeyDown(sf::Keyboard::F10)) {  // TODO: Make new panel for generating map.
        m_song.addMap(Generator::GenerateMap(*m_song.getNotation(0)));
        m_song.saveMap(0);
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
    k.time = m_music.getPlayingOffset().asSeconds() - 0.1f; // 0.1 random offset I think the editor is causing. 
    k.concurrent = concurrent;                              //TODO: Fix BPM not considered.
    if (m_song.getNotation(0)->m_keyframes.size() > 0) 
        k.id = m_song.getNotation(0)->m_keyframes.back().id + 1; //TODO: duplicates possible!!
    else
        k.id = 0;

    createKeyframeShape(k);
    m_song.getNotation(0)->m_keyframes.push_back(k);
}

void EditorPanel::createKeyframeShape(Keyframe k) {
    printf("Creating keyframe, id %ld \ttime: %f \tconcurrent: %d\n", m_song.getNotation(0)->m_keyframes.size(), k.time, k.concurrent);
    sf::CircleShape shape(((k.concurrent > 1) ? c_keyframeRadius * 1.5f : c_keyframeRadius) * m_UIScale);
    shape.setPosition(getPositionAtTime(k.time)); // -1 cuz top of screen is 0.
    shape.setFillColor(c_notatonStandardColor);
    m_keyframeShapes.push_back(shape);

}

sf::Vector2f EditorPanel::getPositionAtTime(float time) {
    return sf::Vector2f(Config::editor.windowWidth / 2, Config::editor.windowHeight / 2 + time * c_spaceModifier * m_UIScale * -1);
}



} // namespace OK