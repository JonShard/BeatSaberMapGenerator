#pragma once

#include "Factory.hpp" 
#include "../Utilities.hpp"

namespace OK {

// RandomFactory produces a random note based on some weights to get more notes far down and colors their side more often.
class RandomFactory : public Factory {
public:
    virtual std::string getName() { return "RandomFactory"; }

    virtual std::vector<Note> produce(Notation notation, Map map) {
        Factory::s_totalProduceAttempts++;
        Keyframe nextKeyframe = notation.getNextKeyframe(map.getLatestTime());

        Note note;
        note.m_parentFactory = getName();
        note.m_time= nextKeyframe.time;
        note.randomize();

        std::vector<Note> notes;
        notes.push_back(note);
        return notes;
    }
};

} // namespace OK