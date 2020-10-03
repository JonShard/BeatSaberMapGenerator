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
        note.m_time= nextKeyframe.time;
        note.m_type = (Util::rng(0, 100) < 60);   // Type 0 = red, 1 = blue
        note.m_cutDirection = Util::rng(0, 8);    // Dot note is 9 none of those for now.
        
        // Notes are more likely to be towards the bottom.
        int layer = Util::rng(0, 100);
        if      (layer > 50) note.m_lineLayer = 0;
        else if (layer > 20) note.m_lineLayer = 1;
        else if (layer >= 0) note.m_lineLayer = 2;

        // Blue is more likely to be on the right.
        int number = Util::rng(0, 100);
        if      (number < 30) note.m_lineIndex = 3; 
        else if (number < 66) note.m_lineIndex = 2;
        else if (number < 85) note.m_lineIndex = 1;
        else note.m_lineIndex = 0;
        if (note.m_type == 0) { // If red, invert location.
            note.m_lineIndex = 3 - note.m_lineIndex;
        }
        
        std::vector<Note> notes;
        notes.push_back(note);
        return notes;
    }
};

} // namespace OK