#pragma once

#include "../TransitionMatrix.hpp"
#include "Factory.hpp" 
#include "../Utilities.hpp"

namespace OK {

// MarkovFactory produces a random note from a Transition matrix of weights where the matrix is a markov chain.
class MarkovFactory : public Factory {
public:
    virtual std::string getName() { return "LineFactory"; }

    virtual bool canProduceAmount(int amount) { return (amount > 1 && amount < 5); }

    virtual std::vector<Note> produce(Notation notation, Map map, int amount) {
        Factory::s_totalProduceAttempts++;
        Keyframe nextKeyframe = notation.getNextKeyframe(map.getLatestTime());
        std::vector<Note> notes;

        for (int i = 0; i < amount; i++) {
            Note note;
            note.m_parentFactory = getName();
            note.m_time = nextKeyframe.time; // Needs to be set this late or it might be overwritten with -nan
            
            notes.push_back(note);
        }        
        return notes;
    }
};

} // namespace OK