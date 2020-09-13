// RandomFactory produces a random note based on some weights to get more notes far down and colors their side more often.
#pragma once

#include "Factory.hpp" 
#include "../Utilities.hpp"

namespace OK {

class RandomFactory : public Factory {
    virtual void loadConfig() {}

public:
    virtual std::vector<Note> produce(std::vector<Notation> notations, Map map) {
        Factory::s_totalProduceAttempts++;

        // TODO: move into function in new annotation class
        float time = 0;
        if (notations.size() > 0) {
            time = notations[0].time;
        }
        if (map.m_notes.size() > 0) {
            float lastNoteTime = map.m_notes.back().time;
            for (int j = 0; j < notations.size(); j++)
            {
                time = notations[j].time;
                if (time > lastNoteTime) 
                    break;
            }
        }

        Note note;
        note.time= time;
        note.type = (Util::rng(0, 100) < 60); // Type 0 = red, 1 = blue
        note.cutDirection = Util::rng(0, 8);
        
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