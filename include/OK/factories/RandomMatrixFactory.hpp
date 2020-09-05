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
        // load notes to generate deviation
        // load matrix
    }

public:
    RandomMatrixFactory() {
        // seed random
        attempts = 0;
    }

    virtual std::vector<Note> produce(Map map) {
    //    // int dev = random +- deviation
    //     for (int i = 0; i < maxAttempts; i++) {
    //         for (int j = dev; j )
    //     }
        return std::vector<Note>(); // If runs out of attempts, return empty.
    }
};

} // namespace OK