#pragma once

#include "Validator.hpp"
#include "../TransitionMatrix.hpp"
#include "../Config.hpp"

namespace OK {

// Matrix Validator ensures that the transition from each note to the next 
// of the same color is legal. 
class MatrixValidator : public Validator {
private:
    TransitionMatrix<bool> m_matrix;
public:
    MatrixValidator() {
        m_passes = 0;
        m_fails = 0;
        if (!m_matrix.loadFromFile(Config::generator.validator.matrix.binaryMatrixFilePath)) {
            printf("\nWarning: unable to load binary matrix file at configured path: %s\n\n", 
                 Config::generator.validator.matrix.binaryMatrixFilePath.data());
        }
        printf("Loaded binary transition matrix containing transitions: %d", m_matrix.getNonZeroCount());
    }

    virtual std::string getName() { return "MatrixValidator"; }


    virtual bool validate(Map map) {
        for (int i = 0; i < map.m_notes.size()-1; i++) {
            std::vector<Note> cluster = map.getNotesInCluster(i);
            // If the last two or more notes are in a cluster, return because the next note(s) doesn't exist.
            if (i + cluster.size() > map.m_notes.size() -1) {
                return true;
            }

            std::vector<Note> clusterNext = map.getNotesInCluster(i + cluster.size());
            if (clusterNext[0].m_time - cluster[0].m_time > Config::generator.validator.validateTimeAfterNote) {
                continue;
            }

            // If transitioning from cluster to another cluster. 
            if (cluster.size() > 1 && clusterNext.size() > 1) {
                // If both clusters have both colors, seperate transitions by color, blue -> blue and red -> red.
                if (Map::IsClusterMultiColor(cluster) && Map::IsClusterMultiColor(clusterNext)) {
                    std::vector<Note> clusterBlue = Map::GetNotesOfColorInCluster(cluster, BLUE);
                    std::vector<Note> clusterRed = Map::GetNotesOfColorInCluster(cluster, RED);
                    std::vector<Note> nextClusterBlue = Map::GetNotesOfColorInCluster(clusterNext, BLUE);
                    std::vector<Note> nextClusterRed = Map::GetNotesOfColorInCluster(clusterNext, RED);
                    for (Note cn : clusterBlue) {
                        for (Note cnn : nextClusterBlue) {
                            if (!m_matrix.getNoteTransition(cn, cnn)) {
                                m_fails++;
                                Validator::s_totalFails++;
                                return false;
                            }
                        }
                    }
                    for (Note cn : clusterRed) {
                        for (Note cnn : nextClusterRed) {
                            if (!m_matrix.getNoteTransition(cn, cnn)) {
                                m_fails++;
                                Validator::s_totalFails++;
                                return false;
                            }                        
                        }
                    }
                }
                else {
                    for (Note cn : cluster) {
                        for (Note cnn : clusterNext) {
                            if (!m_matrix.getNoteTransition(cn, cnn)) {
                                m_fails++;
                                Validator::s_totalFails++;
                                return false;
                            }
                        }
                    }
                }
                i += cluster.size();
                continue;
            }

            // If transitioning from a single note to a cluster.
            if (clusterNext.size() > 1) {
                for (Note cn : clusterNext) {
                    if (!m_matrix.getNoteTransition(cluster[0], cn)) {
                        m_fails++;
                        Validator::s_totalFails++;
                        return false;
                    }                
                }
                continue;
            }

            // If transitioning from inside a cluster, consider each node in cluster as a transition to the next note:
            if (cluster.size() > 1) {
                int firstAfterCluster = i + cluster.size(); 
                for (int j = i; j < firstAfterCluster; j++) {
                    Note cn = map.m_notes[j];
                    Note cnn = map.m_notes[firstAfterCluster];
                    if (!m_matrix.getNoteTransition(cn, cnn)) {
                        m_fails++;
                        Validator::s_totalFails++;
                        return false;
                    }                
                }
                i = firstAfterCluster - 1;
                continue;
            }


            if (m_matrix.getNoteTransition(cluster[0], clusterNext[0]) == false) {
                m_fails++;
                Validator::s_totalFails++;
                return false;
            }

            int transitionsFromNext = m_matrix.getTransitionCountFromNote(clusterNext[0]);
            if (transitionsFromNext == 0) {
                printf("MatrixValidator: No possible ways to transition from the next node. Will become absorbing.\n");
                
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