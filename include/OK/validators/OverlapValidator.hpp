#pragma once

#include "Validator.hpp"
#include "../Config.hpp"

namespace OK {

// OverlapValidator ensures that no notes are inside eachother.
class OverlapValidator : public Validator {
public:
    OverlapValidator() {
        m_passes = 0;
        m_fails = 0;
    }

    virtual std::string getName() { return "OverlapValidator"; }

    virtual bool validate(Map map) {
        std::vector<Note> notes = map.getNotes();
        for (int i = notes.size()-1; i >= 0; i--) { // For every note in the map starting at the back:
            for (int j = i -1; j >= 0; j--) {
                if (notes[i].m_lineIndex == notes[j].m_lineIndex && 
                    notes[i].m_lineLayer == notes[j].m_lineLayer) {
                    m_fails++;
                    Validator::s_totalFails++;
                    return false;                        
                }
                // Stop looking if the notes are separated enough.
                if (notes[i].m_time - notes[j].m_time > Config::generator.noteClusterTime) {
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