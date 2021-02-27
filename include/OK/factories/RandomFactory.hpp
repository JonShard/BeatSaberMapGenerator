#pragma once

#include "Factory.hpp" 
#include "../Utilities.hpp"

namespace OK {

// RandomFactory produces a random note based on some weights to get more notes far down and colors their side more often.
class RandomFactory : public Factory {
public:
    virtual std::string getName() { return "RandomFactory"; }

    virtual bool canProduceAmount(int amount) { return (amount == 1); }

    virtual Cluster produce(Notation notation, Map map, int amount) {
        Factory::s_totalProduceAttempts++;
        Keyframe nextKeyframe = notation.getNextKeyframe(map.getLatestTime());
        Cluster cluster;

        for (int i = 0; i < amount; i++) {
            Note note;
            note.m_parentFactory = getName();
            note.m_time= nextKeyframe.time;
            note.randomize();
            cluster += note;
        }
        return cluster;
    }
};

} // namespace OK