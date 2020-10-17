#pragma once

#include "Validator.hpp"
#include "../Config.hpp"

namespace OK {

// ForceValidator ensures the a psuedo 'force' required to put on the player body to complete each cluster is wihin a set tolerance.
class ForceValidator : public Validator {
private:
const float cfg_maxForce = 100;
public:
    ForceValidator() {
        m_passes = 0;
        m_fails = 0;
    }

    virtual std::string getName() { return "ForceValidator"; }

    virtual bool validate(Map map) {
        for (int i = 0; i < map.m_notes.size()-1; i++) {
            std::vector<Note> cluster = map.getNotesInCluster(i);
            // If the last two or more notes are in a cluster, return because the next note(s) doesn't exist.
            if (i + cluster.size() > map.m_notes.size() -1) {
                return true;
            }

            std::vector<Note> clusterNext = map.getNotesInCluster(i + cluster.size());
            if (std::abs(clusterNext[0].m_time - cluster[0].m_time) > Config::generator.validator.validateTimeAfterNote) {
                continue;
            }

            if (!(cluster.size() > 1 && clusterNext.size() > 1)) { // If one of the cluster is a single note, continue
                continue;
            }
            
            float force = 0;

            // Calculate force
            

            if (force >= cfg_maxForce) {
                m_fails++;
                Validator::s_totalFails++;
                return false;
            }
            i += cluster.size();
        }
        m_passes++;
        Validator::s_totalPasses++;
        return true;
    }
};

} // Namespace OK