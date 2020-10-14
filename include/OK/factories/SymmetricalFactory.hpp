#pragma once

#include "Factory.hpp" 
#include "../Utilities.hpp"

namespace OK {

enum SymmertyMode { CENTER_POINT, HORIZONTAL, VERTICAL };

// SymmetricalFactory Generates
class SymmetricalFactory : public Factory {
public:

    SymmetricalFactory() {
        int modesEnabled = 0;
        if (Config::generator.factory.symmetrical.centerPointMode) modesEnabled++;
        if (Config::generator.factory.symmetrical.horizontalMode) modesEnabled++;
        if (Config::generator.factory.symmetrical.verticalMode) modesEnabled++;
        if (modesEnabled == 0) printf("Warning: SymmetricalFactory is enabled but has no modes enabled. It will not generate any notes.\n");
    }

    virtual std::vector<Note> produce(Notation notation, Map map) {
        Factory::s_totalProduceAttempts++;
        Keyframe nextKeyframe = notation.getNextKeyframe(map.getLatestTime());
        std::vector<Note> notes;

        Note noteBlue;
        noteBlue.m_time = nextKeyframe.time;
        noteBlue.m_type = BLUE;
        noteBlue.m_cutDirection = (CutDirection)Util::rng(0, 8);
        noteBlue.m_lineLayer = Util::rng(0, 2);
        
        // Blue is more likely to be on the right.
        int number = Util::rng(0, 100);
        if      (number < 40) noteBlue.m_lineIndex = 3; 
        else if (number < 80) noteBlue.m_lineIndex = 2;
        else if (number < 90) noteBlue.m_lineIndex = 1;
        else noteBlue.m_lineIndex = 0;

        if (noteBlue.isInCenter()) {
            if (!Config::generator.factory.symmetrical.allowNotesInCenter) {
                if (noteBlue.m_lineIndex == 1) {
                    noteBlue.m_lineIndex--;
                }
                else if (noteBlue.m_lineIndex == 2){
                    noteBlue.m_lineIndex++;
                }
            }
            else if ((noteBlue.m_lineIndex == 1 || noteBlue.m_lineIndex == 2) 
                    && (noteBlue.m_cutDirection == RIGHT || noteBlue.m_cutDirection == LEFT)) { // Clap not allowed.
                while (noteBlue.m_cutDirection == RIGHT || noteBlue.m_cutDirection == LEFT) {
                    noteBlue.m_cutDirection = (CutDirection)Util::rng(0, 8);
                }
            }
        }

        if (noteBlue.m_time > 24 && noteBlue.m_cutDirection == UP && noteBlue.m_lineLayer == 1 && noteBlue.m_lineIndex == 2) {
            printf("This one\n");
        }

        int modesEnabled = 0;
        if (Config::generator.factory.symmetrical.centerPointMode) modesEnabled++;
        if (Config::generator.factory.symmetrical.horizontalMode) modesEnabled++;
        if (Config::generator.factory.symmetrical.verticalMode) modesEnabled++;
        if (modesEnabled == 0) return std::vector<Note>();
        
        Note noteRed = noteBlue;
        while (true)
        {
            int mode = Util::rng(0, 3);
            if (mode == CENTER_POINT && Config::generator.factory.symmetrical.centerPointMode) {
                noteRed.invertNote();
                noteRed.invertPosition();
                break;
            } 
            else if (mode == HORIZONTAL && Config::generator.factory.symmetrical.horizontalMode) {
                while (noteBlue.isVertical()) { // Ensure horizontal.
                    noteBlue.m_cutDirection = (CutDirection)Util::rng(0, 8);
                }
                if (noteBlue.m_lineLayer == 1) {      // If in center, move one up or down.
                    noteBlue.m_lineLayer += 1 - Util::rng(0, 3);
                }
                noteRed = noteBlue;
                noteRed.invertColor();
                noteRed.invertVertical();
                break;
            } 
            else if (mode == VERTICAL && Config::generator.factory.symmetrical.verticalMode) {
                if (!Config::generator.factory.symmetrical.allowOffsetPlane) { // If offset not allowed, blue note is one one of the horizontal edges.
                    noteBlue.m_lineIndex = (Util::rng(0, 2) == 0) ? 0 : 3;
                }
                if (std::abs(noteBlue.m_lineIndex - noteRed.m_lineIndex) < 2) {
                    while (noteBlue.isHorizontal()) { // Ensure vertical.
                        noteBlue.m_cutDirection = (CutDirection)Util::rng(0, 8);
                    }
                }
                noteRed = noteBlue;
                noteRed.invertColor();
                noteRed.invertHorizontal();
                break;
            }
        }

        notes.push_back(noteBlue);
        notes.push_back(noteRed);
        return notes;
    }
};

} // namespace OK