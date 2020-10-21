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

    virtual std::string getName() { return "MarkovFactory"; }

    virtual bool canProduceAmount(int amount) { return (amount == 1); }

    Note pickTransitionFromMarkovChain(Note fromNote) {
        std::vector<std::pair<float, Note>> possibleTransitions = m_matrix.getTransitionsFromNote(fromNote);
        Note noteTo;
        if (possibleTransitions.size() == 0) {
            m_absorbingNotesHit++;
            printf("Warning: MarkovFactory encountered an absorbing state in the markov chain picking a random transition (%d)\n", m_absorbingNotesHit);
            noteTo.randomize();
        }
        else {
            do {
                noteTo = possibleTransitions[Util::rng(0, possibleTransitions.size())].second;
            } while(noteTo.m_type == BOMB || noteTo.m_cutDirection == DOT);
        }
        return noteTo;
    }

    virtual std::vector<Note> produce(Notation notation, Map map, int amount) {
        Factory::s_totalProduceAttempts++;
        Keyframe nextKeyframe = notation.getNextKeyframe(map.getLatestTime());
        std::vector<Note> notes;
        Note note;
        if (map.m_notes.size() == 0) {
            note.m_time= nextKeyframe.time;
            note.randomize();
            notes.push_back(note);
            return notes;
        }

        note = pickTransitionFromMarkovChain(map.m_notes[map.m_notes.size() - 1]);
        note.m_parentFactory = getName();
        note.m_time = nextKeyframe.time; // Needs to be set this late or it might be overwritten with -nan
        notes.push_back(note);
        
        if (amount == 2) {
            int failsafe = 0;
            Note secondNote;
            do {
                secondNote = pickTransitionFromMarkovChain(notes[0]);
                failsafe++;
            } while(secondNote.m_type == note.m_type && failsafe < 100000);
            
            secondNote.m_parentFactory = getName();
            secondNote.m_time = nextKeyframe.time; // Needs to be set this late or it might be overwritten with -nan
            notes.push_back(secondNote);
        }

        return notes;
    }
};

} // namespace OK