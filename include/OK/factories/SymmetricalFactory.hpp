#pragma once

#include "Factory.hpp" 
#include "../Utilities.hpp"

namespace OK {

// SymmetricalFactory Generates
class SymmetricalFactory : public Factory {
public:
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
            if (Config::generator.factory.symmetrical.onlyOnBorder ) {
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

        Note noteRed = noteBlue;
        noteRed.invertNote();
        noteRed.invertPosition();
        
        notes.push_back(noteBlue);
        notes.push_back(noteRed);
        return notes;
    }
};

} // namespace OK