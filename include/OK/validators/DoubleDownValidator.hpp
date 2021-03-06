#pragma once

#include "Validator.hpp"
#include "../Config.hpp"

namespace OK {

// DoubleDownValidator ensures the frequency consecutive notes in the same direction of a color is wihin a set tolerance.
class DoubleDownValidator : public Validator {
private:

public:
    DoubleDownValidator() {
        m_passes = 0;
        m_fails = 0;
    }

    virtual std::string getName() { return "DoubleDownValidator"; }


    virtual bool validate(Map map) {
        for (int i = 1; i < map.m_notes.size(); i++) {
            for (int j = i - 1; j >= 0; j--) {
                if (std::abs(map.m_notes[i].m_time - map.m_notes[j].m_time) < Config::generator.noteClusterTime) // Skip the ones in the same cluster.
                    continue;

                if (map.m_notes[i].m_type != map.m_notes[j].m_type) // Has to be same color to be double down.
                    continue;

                int angleDelta = Util::angleDelta(CutAngle[map.m_notes[i].m_cutDirection], CutAngle[map.m_notes[j].m_cutDirection]);

                if (angleDelta <= Config::generator.validator.doubleDown.angleToBeDoubleDown) {
                    m_fails++;
                    Validator::s_totalFails++;
                    return false;                    
                }
                else {  // Last note wasn't double down, time to stop looking so we don't compare with notes that aren't the previous one.
                    break;
                }

                if (map.m_notes[i].m_time - map.m_notes[j].m_time > Config::generator.validator.validateTimeAfterNote) {
                    break;
                }
            }
        }
        m_passes++;
        Validator::s_totalPasses++;
        return true;
    }
};

} // Namespace OK