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
        std::vector<Note> notes;
        Keyframe nextKeyframe = notation.getNextKeyframe(map.getLatestTime());

        if (map.m_notes.size() == 0) {
            Note note;
            note.m_time= nextKeyframe.time;
            note.m_type = (Type)(Util::rng(0, 100) < 60);   // Type 0 = red, 1 = blue
            note.m_cutDirection = (CutDirection)Util::rng(0, 8);    // Dot note is 9 none of those for now.
            
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
            notes.push_back(note);
            return notes;
        }
        
        Note previous = map.m_notes[map.m_notes.size() - 1];
        std::vector<std::pair<float, Note>> possibleTransitions = m_matrix.getTransitionsFromNote(previous);

        float random = Util::rng0To1();
        float number = 0;
        Note note;
        for(std::pair<float, Note> pair : possibleTransitions) {
            number += pair.first;
            if (number >= random) {
                note = pair.second;
                break;
            }
        }
        note.m_time = nextKeyframe.time;
        notes.push_back(note);
        return notes;
    }
};

} // namespace OK