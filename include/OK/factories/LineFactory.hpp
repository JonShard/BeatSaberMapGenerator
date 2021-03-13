#pragma once

#include "../TransitionMatrix.hpp"
#include "Factory.hpp" 
#include "../Utilities.hpp"

namespace OK {

// MarkovFactory produces a random note from a Transition matrix of weights where the matrix is a markov chain.
class LineFactory : public Factory {
public:
    virtual std::string getName() { return "LineFactory"; }

    virtual bool canProduceAmount(int amount) { return (amount > 1 && amount < 5); }

    virtual Cluster produce(Notation notation, Map map, int amount) {
        Factory::s_totalProduceAttempts++;
        Keyframe nextKeyframe = notation.getNextKeyframe(map.getLatestTime());
        Cluster cluster;

        Note noteMain;
        noteMain.m_parentFactory = getName();
        noteMain.m_time = nextKeyframe.time;
        do {
            noteMain.randomize();
        } while (noteMain.getLongestLineLength() < amount);
        cluster += noteMain;
        
        // TODO: Fix. Can get stuck if noteMain is diagonal corner perpendicualar to diagonal. or you tell it to make 3 then longest line possible is 2.
        for (int i = 1; i < amount; i++) {
            Note note = noteMain;
            std::pair<int, int> pos;
            do {
                if (Util::cointoss())
                    pos = cluster.m_notes.front().getPositionAbove();
                else
                    pos = cluster.m_notes.back().getPositionBelow();
            
                note.m_lineIndex = pos.first;      
                note.m_lineLayer = pos.second;      
            } while (!note.isValid());
            cluster += note;
        }
        return cluster;
    }
};

} // namespace OK