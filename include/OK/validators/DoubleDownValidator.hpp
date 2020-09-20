// DoubleDownValidator ensures the frequency consecutive notes in the same direction of a color is wihin a set tolerance.
#pragma once

#include "Validator.hpp"
#include "../Config.hpp"

namespace OK {

class DoubleDownValidator : public Validator {
private:
    int angleToBeDoubleDown = 90; //0, 45, 90, 135

public:
    DoubleDownValidator() {}

    virtual bool validate(Map map) {
        for (int i = 1; i < map.m_notes.size(); i++) {
            for (int j = i - 1; j >= 0; j--) {
                if (map.m_notes[i].time - map.m_notes[j].time < Config::generator.noteClusterTime)
                    continue;

                if (map.m_notes[i].type != map.m_notes[j].type) // Has to be same color to be double down.
                    continue;

                if (std::abs(CutAngle[map.m_notes[i].cutDirection] - CutAngle[map.m_notes[j].cutDirection]) <= angleToBeDoubleDown) {
                    Validator::s_fails++;
                    return false;                    
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