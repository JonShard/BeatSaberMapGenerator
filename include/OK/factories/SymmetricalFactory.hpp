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
        noteBlue.time = nextKeyframe.time;
        noteBlue.type = BLUE;
        noteBlue.cutDirection = Util::rng(0, 8);
        noteBlue.lineLayer = Util::rng(0, 2);
        
        // Blue is more likely to be on the right.
        int number = Util::rng(0, 100);
        if      (number < 40) noteBlue.lineIndex = 3; 
        else if (number < 80) noteBlue.lineIndex = 2;
        else if (number < 90) noteBlue.lineIndex = 1;
        else noteBlue.lineIndex = 0;

        if (noteBlue.isInCenter()) {
            if (!Config::generator.factory.symmetrical.allowNotesInCenter) {
                if (noteBlue.lineIndex == 1) {
                    noteBlue.lineIndex--;
                }
                else if (noteBlue.lineIndex == 2){
                    noteBlue.lineIndex++;
                }
            }
            else if ((noteBlue.lineIndex == 1 || noteBlue.lineIndex == 2) 
                    && (noteBlue.cutDirection == RIGHT || noteBlue.cutDirection == LEFT)) { // Clap not allowed.
                while (noteBlue.cutDirection == RIGHT || noteBlue.cutDirection == LEFT) {
                    noteBlue.cutDirection = Util::rng(0, 8);
                }
            }
        }

        if (noteBlue.time > 24 && noteBlue.cutDirection == UP && noteBlue.lineLayer == 1 && noteBlue.lineIndex == 2) {
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
                    noteBlue.cutDirection = Util::rng(0, 8);
                }
                if (noteBlue.lineLayer == 1) {      // If in center, move one up or down.
                    noteBlue.lineLayer += 1 - Util::rng(0, 3);
                }
                noteRed = noteBlue;
                noteRed.invertColor();
                noteRed.invertVertical();
                break;
            } 
            else if (mode == VERTICAL && Config::generator.factory.symmetrical.verticalMode) {
                if (!Config::generator.factory.symmetrical.allowOffsetPlane) { // If offset not allowed, blue note is one one of the horizontal edges.
                    noteBlue.lineIndex = (Util::rng(0, 2) == 0) ? 0 : 3;
                }
                if (std::abs(noteBlue.lineIndex - noteRed.lineIndex) < 2) {
                    while (noteBlue.isHorizontal()) { // Ensure vertical.
                        noteBlue.cutDirection = Util::rng(0, 8);
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