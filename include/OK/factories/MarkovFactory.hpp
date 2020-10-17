#pragma once

#include "../TransitionMatrix.hpp"
#include "Factory.hpp" 
#include "../Utilities.hpp"

namespace OK {

// MarkovFactory produces a random note from a Transition matrix of weights where the matrix is a markov chain.
class MarkovFactory : public Factory {
private:
    TransitionMatrix<float> m_matrix;
public:
    MarkovFactory() {
        if (!m_matrix.loadFromFile(Config::generator.factory.markov.markovMatrixFilePath)) {
            printf("\nWarning: unable to load markov matrix file at configured path: %s\n\n", 
                 Config::generator.validator.matrix.binaryMatrixFilePath.data());
        }
        printf("Loaded markov transition matrix containing transitions: %d\n", m_matrix.getNonZeroCount());
    }

    virtual std::vector<Note> produce(Notation notation, Map map) {
        Factory::s_totalProduceAttempts++;

        Keyframe nextKeyframe = notation.getNextKeyframe(map.getLatestTime());

        Note note;
        note.m_time = nextKeyframe.time;
       
 

        std::vector<Note> notes;
        notes.push_back(note);
        return notes;
    }
};

} // namespace OK