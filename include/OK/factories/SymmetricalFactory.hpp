#pragma once

#include "Factory.hpp" 
#include "../Utilities.hpp"

namespace OK {

// SymmetricalFactory Generates
class SymmetricalFactory : public Factory {
public:
    virtual std::vector<Note> produce(Notation notation, Map map) {
        Factory::s_totalProduceAttempts++;
        Keyframe nextKeyframe = notation.getNextKeyframe(map.getLatestTime());
        std::vector<Note> notes;

        Note note;
        note.time= nextKeyframe.time;
        note.type = (Util::rng(0, 100) < 60); // Type 0 = red, 1 = blue
        note.cutDirection = Util::rng(0, 8);



        Util::rng(0, 100);
        notes.push_back(note);


        return notes;
    }
};

} // namespace OK