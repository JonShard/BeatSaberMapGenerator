#pragma once

#include "Validator.hpp"
#include "../Config.hpp"

namespace OK {

// ClusterCurveValidator ensures that all notes in a cluster of the same color is in a line, no, curving the sabers.
class ClusterCurveValidator : public Validator {
public:
    ClusterCurveValidator() {
        m_passes = 0;
        m_fails = 0;
    }

    virtual std::string getName() { return "ClusterCurveValidator"; }

    virtual bool validate(Map map) {
        for (Cluster cluster : map.m_clusters) {   // For every cluster in the map:
            // Make sure notes of the same color in the cluster is on a line:
            std::vector<Cluster> colorClusters = std::vector<Cluster>(); 
            colorClusters.push_back(cluster.getNotesOfType(BLUE));
            colorClusters.push_back(cluster.getNotesOfType(RED));
            for (Cluster c : colorClusters) {
                for (Note m : c.m_notes) {    
                    for (Note n : c.m_notes) {
                        if (m == n) {
                            continue;
                        }
                        if (!m.isInLine(n)) {
                            m_fails++;
                            Validator::s_totalFails++;
                            return false;                        
                        }
                    }
                }
            }
            // Make sure notes of the opposite color is not in a line unless on left and right edges:  
            for (Note m : colorClusters.back().m_notes) {    
                for (Note n : colorClusters.front().m_notes) {
                    if (m.isOnSamePlane(n) && std::abs(m.m_lineIndex - n.m_lineIndex) < 3) {
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