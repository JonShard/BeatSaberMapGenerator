#pragma once

#include "Factory.hpp" 
#include "../Utilities.hpp"

namespace OK {

// RandomFactory produces a random note based on some weights to get more notes far down and colors their side more often.
class RandomFactory : public Factory {
public:
    virtual std::vector<Note> produce(Notation notation, Map map) {
        Factory::s_totalProduceAttempts++;

        Keyframe nextKeyframe = notation.getNextKeyframe(map.getLatestTime());

        Note note;
        note.time= nextKeyframe.time;
        note.type = (Util::rng(0, 100) < 60);   // Type 0 = red, 1 = blue
        note.cutDirection = Util::rng(0, 8);    // Dot note is 9 none of those for now.
        
        int layer = Util::rng(0, 100);
        if      (layer > 50) note.lineLayer = 0;
        else if (layer > 20) note.lineLayer = 1;
        else if (layer >= 0) note.lineLayer = 2;

        int number = Util::rng(0, 100);
        if (number < 30) note.lineIndex = 3; 
        else if (number < 66) note.lineIndex = 2;
        else if (number < 85) note.lineIndex = 1;
        else note.lineIndex = 0;
        if (note.type == 0) { // If red, invert location.
            note.lineIndex = 3 - note.lineIndex;
        }
        
        std::vector<Note> notes;
        notes.push_back(note);
        return notes;
    }
};

} // namespace OK