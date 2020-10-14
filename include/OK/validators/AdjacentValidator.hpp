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
        if (Config::generator.validator.adjacent.timeToBeAdjacent < Config::generator.noteClusterTime) {
            printf("\nWarning: timeToBeAdjacent should not be smaller than noteClusterTime\n\n");
        }
        if (Config::generator.validator.adjacent.timeToBeAdjacent < Config::generator.validator.adjacent.timeToEnforceSameTrack) {
            printf("\nWarning: timeToBeAdjacent should not be smaller than timeToEnforceSameTrack\n\n");
        }
    }

    virtual std::string getName() { return "AdjacentValidator"; }

    virtual bool validate(Map map) {
        for (int i = map.m_notes.size()-1; i > 1; i--) {
            Note current = map.m_notes[i];
            Note previous = map.getPreviousNoteOfColor(i, (OK::Type)current.m_type);

            float timeDelta = std::abs(previous.m_time - current.m_time);
            if (timeDelta < Config::generator.noteClusterTime) // Skip the ones in the same cluster.
                continue;

            if (timeDelta > Config::generator.validator.adjacent.timeToBeAdjacent) // Skip the ones that are too far apart to be adjacent.
                continue;

            if (timeDelta < Config::generator.validator.adjacent.timeToEnforceSameTrack) { // If time is really short, enforce notes to be on same track.
                if (!current.isOnSamePlane(previous)) {
                    m_fails++;
                    Validator::s_totalFails++;
                    return false;
                }
            }
            
            if (!current.isOppositeCutDirection(previous)) {
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