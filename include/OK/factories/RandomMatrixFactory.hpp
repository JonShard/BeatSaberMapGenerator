#pragma once

#include "Factory.hpp" 

namespace OK {

class RandomMatrixFactory : public Factory {
    int notesToGenerate;
    int notesToGenerateDeviation;
    // matrix of weights
    virtual void loadConfig() {
        // load maxAttempts
        // load notes to generate
        // load notes to generGeneratorate deviation
        // load matrix
    }

public:
    RandomMatrixFactory() {
        // seed random
        attempts = 0;
        maxAttempts = 5;
        notesToGenerate = 1;
    }

    virtual std::vector<Note> produce(Map map) {
        // int dev = random +- deviation
         for (int i = 0; i < maxAttempts; i++) {
            std::vector<Note> notes;
             for (int j = 0; j < notesToGenerate; j ++) {
                Note note = Note{
                    note.time=0,
                    note.lineIndex = random()%4,
                    note.lineLayer = random()%3,
                    note.type = random()%2,
                    note.cutDirection = random()%8,
                };
                notes.push_back(note);
            }
            return notes;
        }
        return std::vector<Note>(); // If runs out of attempts, return empty.
    }
};

} // namespace OK