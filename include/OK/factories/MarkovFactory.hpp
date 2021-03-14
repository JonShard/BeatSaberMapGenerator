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

    virtual bool canProduceAmount(int amount) { return (amount != 0); }

    Note pickTransitionFromMarkovChain(Note fromNote) {
        std::vector<std::pair<float, Note>> possibleTransitions = m_matrix.getTransitionsFromNote(fromNote);
        Note noteTo;
        if (possibleTransitions.size() == 0) {
            m_absorbingNotesHit++;
            printf("Warning: MarkovFactory encountered an absorbing state in the markov chain, picking a random transition (%d)\n", m_absorbingNotesHit);
            noteTo.randomize();
        }
        else {
            bool onlyBombsAndDot = true;
            for (std::pair<float, Note> trans : possibleTransitions) 
                if (trans.second.m_type != BOMB && trans.second.m_cutDirection != DOT) 
                    onlyBombsAndDot = false;
            
            if (onlyBombsAndDot) {
                printf("Warning: MarkovFactory encountered a state in the markov chain that only transitions to bombs or dot, picking a random transition (%d)\n", m_absorbingNotesHit);
                noteTo.randomize();
            }
            else {
                do {
                    noteTo = possibleTransitions[Util::rng(0, possibleTransitions.size())].second;
                } while(noteTo.m_type == BOMB || noteTo.m_cutDirection == DOT);
            }
        }
        return noteTo;
    }

    virtual Cluster produce(Notation notation, Map map, int amount) {
        Factory::s_totalProduceAttempts++;
        Keyframe nextKeyframe = notation.getNextKeyframe(map.getLatestTime());
        Cluster cluster;
        Note note;
        if (map.m_clusters.size() == 0) {
            note.m_time= nextKeyframe.time;
            note.randomize();
            cluster += note;
            return cluster;
        }

        note = pickTransitionFromMarkovChain(map.getLatestNote());
        note.m_parentFactory = getName();
        note.m_time = nextKeyframe.time; // Needs to be set this late or it might be overwritten with -nan
        cluster += note;
        
        if (amount == 2) {
            int failsafe = 0;
            Note secondNote;
            
            // do {
            //     secondNote = pickTransitionFromMarkovChain(notes[0]);
            //     failsafe++;
            // } while(secondNote.m_type == note.m_type && failsafe < 100000);
            // if (failsafe < 100000) {
            //     printf("Warning: MarkovFactory faild to produce two notes, none was produced\n\n");
            //     return std::vector<Note>();
            // }
            
            secondNote = pickTransitionFromMarkovChain(note);
            secondNote.m_parentFactory = getName();
            secondNote.m_time = nextKeyframe.time; // Needs to be set this late or it might be overwritten with -nan
            cluster += secondNote;
        }
        return cluster;
    }
};

} // namespace OK