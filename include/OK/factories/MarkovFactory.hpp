#pragma once

#include "../TransitionMatrix.hpp"
#include "Factory.hpp" 
#include "../Utilities.hpp"

namespace OK {

// MarkovFactory produces a random note from a Transition matrix of weights where the matrix is a markov chain.
class MarkovFactory : public Factory {
private:
    TransitionMatrix<float> m_matrix;
    int m_absorbingNotesHit;
public:
    MarkovFactory() {
        m_absorbingNotesHit = 0;
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
            note.randomize();
            notes.push_back(note);
            return notes;
        }
        
        Note previous = map.m_notes[map.m_notes.size() - 1];
        std::vector<std::pair<float, Note>> possibleTransitions = m_matrix.getTransitionsFromNote(previous);

        float random = Util::rng0To1();
        float number = 0;
        bool isNoteSet = false;
        Note note;
        for(std::pair<float, Note> pair : possibleTransitions) {
            number += pair.first;
            if (number >= random) {
                note = pair.second;
                isNoteSet = true;
                break;
            }
        }
        if (!isNoteSet) {
            m_absorbingNotesHit++;
            printf("Warning: MarkovFactory encountered an absorbing state in the markov chain picking a random transition\n");
            note.randomize();
        }

        note.m_time = nextKeyframe.time; // Needs to be set this late or it might be overwritten with -nan
        notes.push_back(note);
        return notes;
    }
};

} // namespace OK