#pragma once

#include "Validator.hpp"
#include "../TransitionMatrix.hpp"
#include "../Config.hpp"

namespace OK {

// AdjacentValidator ensures that notes that are adjacent (very small time differance) have very easy, readable and predictable transitions.
class AdjacentValidator : public Validator {
public:
    AdjacentValidator() {
        m_passes = 0;
        m_fails = 0;
    }

    virtual std::string getName() { return "AdjacentValidator"; }

    virtual bool validate(Map map) {
        for (int i = 0; i < map.m_notes.size()-1; i++) {
            Note current = map.m_notes[i];
            Note next = map.m_notes[i + 1];
            if (next.m_time - current.m_time < Config::generator.noteClusterTime) // Skip the ones in the same cluster.
                continue;

            if (next.m_time - current.m_time > Config::generator.validator.adjacent.timeToBeAdjacent) // Skip the ones that are too far apart to be adjacent.
                continue;
            
            if (!current.isOppositeCutDirection(next)) {
                m_fails++;
                Validator::s_totalFails++;
                return false;
            }
        }

        m_passes++;
        Validator::s_totalPasses++;
        return true;
    }
};

} // Namespace OK