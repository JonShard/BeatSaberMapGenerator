#pragma once

#include "Factory.hpp" 

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
            int layer = random()%100;
            if      (layer > 50) layer = 0;
            else if (layer > 20) layer = 1;
            else if (layer >= 0) layer = 2;

            // TODO: move into function
            float time  = 0;
            if (map.m_map.size() > 0) {
                float lastNoteTime = map.m_map.back().time;
                for (int j = 0; j < notations.size(); j++)
                {
                    time = notations[j].time;
                    if (time > lastNoteTime) 
                        break;
                }
            }

            Note note = Note{
                note.time= time,
                note.lineIndex = random()%4,
                note.lineLayer = layer,
                note.type = (random()%100 > 60),
                note.cutDirection = random()%8,
            };
            notes.push_back(note);
        }
        return notes;
        }
};

} // namespace OK