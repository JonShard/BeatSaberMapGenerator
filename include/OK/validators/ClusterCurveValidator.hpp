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
        for (int i = 0; i < map.m_notes.size()-1; ) {   // For every cluster in the map:
            std::vector<Note> cluster = map.getNotesInCluster(i);
            i += cluster.size();

            // Make sure notes of the same color in the cluster is on a line:
            std::vector<Note> clusterBlue = Note::GetNotesOfColorInCluster(cluster, BLUE);
            std::vector<Note> clusterRed = Note::GetNotesOfColorInCluster(cluster, RED);
            for (int c = 0; c < 2; c++) {
                std::vector<Note> clusterColor = (c == 0) ? clusterBlue : clusterRed;                    
                for (Note m : clusterColor) {    
                    for (Note n : clusterColor) {
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
            for (Note m : clusterBlue) {    
                for (Note n : clusterRed) {
                    if (m.isInLine(n) && std::abs(m.m_lineIndex - n.m_lineIndex) < 3) {
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