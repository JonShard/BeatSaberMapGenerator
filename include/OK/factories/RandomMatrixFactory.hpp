#pragma once

#include "Factory.hpp" 
#include "../Utilities.hpp"

namespace OK {

class RandomMatrixFactory : public Factory {
    int notesToGenerate;
    int notesToGenerateDeviation;
    // matrix of weights
    virtual void loadConfig() {
        // load notes to generate
        // load matrix
    }

public:
    RandomMatrixFactory() {
        // seed random
        attempts = 0;
        notesToGenerate = 1;
    }

    virtual std::vector<Note> produce(std::vector<Notation> notations, Map map) {
        std::vector<Note> notes;
            for (int i = 0; i < notesToGenerate; i ++) {
            int layer = Util::rng(0, 100);
            if      (layer > 50) layer = 0;
            else if (layer > 20) layer = 1;
            else if (layer >= 0) layer = 2;

            // TODO: move into function
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
            note.lineLayer = layer;
            note.type = (Util::rng(0, 100) < 60); // Type 0 = red, 1 = blue
            note.cutDirection = Util::rng(0, 8);
            
            int number = Util::rng(0, 100);
            if (number < 30) {
                note.lineIndex = 3;
            } 
            else if (number < 66){
                note.lineIndex = 2;
            }
            else if (number < 85){
                note.lineIndex = 1;
            }
            else {
                note.lineIndex = 0;
            }
            if (note.type == 0) { // If red, invert location.
                note.lineIndex = 3 - note.lineIndex;
            }
            notes.push_back(note);
        }
        return notes;
        }
};

} // namespace OK