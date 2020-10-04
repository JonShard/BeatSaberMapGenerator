#pragma once

#include "Validator.hpp"
#include "../TransitionMatrix.hpp"
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
        for (int i = 0; i < map.m_notes.size()-1; ) {
            std::vector<Note> cluster = map.getNotesInCluster(i);
            i += cluster.size();

            for (int m = 0; m < cluster.size(); m++) {
                for (int n = 0; n < cluster.size(); n++) {  // Are two different notes in the cluster in the same position?
                    if (m == n) {
                        continue;
                    }
                    if (cluster[m].m_lineIndex == cluster[n].m_lineIndex && cluster[m].m_lineLayer == cluster[n].m_lineLayer) {
                        m_fails++;
                        Validator::s_totalFails++;
                        return false;                        
                    }
                }
            }

        }
        m_passes++;
        Validator::s_totalPasses++;
        return true;
    }
};

} // Namespace OK