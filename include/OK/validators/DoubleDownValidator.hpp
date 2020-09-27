#pragma once

#include "Validator.hpp"
#include "../Config.hpp"

namespace OK {

// DoubleDownValidator ensures the frequency consecutive notes in the same direction of a color is wihin a set tolerance.
class DoubleDownValidator : public Validator {
private:

public:
    DoubleDownValidator() {}

    virtual bool validate(Map map) {
        for (int i = 1; i < map.m_notes.size(); i++) {
            for (int j = i - 1; j >= 0; j--) {
                if (map.m_notes[i].time - map.m_notes[j].time < Config::generator.noteClusterTime)
                    continue;

                if (map.m_notes[i].type != map.m_notes[j].type) // Has to be same color to be double down.
                    continue;

                int angleDelta = Util::angleDelta(CutAngle[map.m_notes[i].cutDirection], CutAngle[map.m_notes[j].cutDirection]);

                if (angleDelta <= Config::generator.validator.doubleDown.angleToBeDoubleDown) {
                    Validator::s_fails++;
                    return false;                    
                }
                else {  // Last note wasn't double down, time to stop looking so we don't compare with notes that aren't the previous one.
                    break;
                }

                if (map.m_notes[i].time - map.m_notes[j].time > Config::generator.validator.validateTimeAfterNote) {
                    break;
                }
            }
        }
        Validator::s_passes++;
        return true;
    }
};

} // Namespace OK