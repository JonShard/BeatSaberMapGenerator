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
        printf("Loaded binary transition matrix containing transitions: %d\n", m_matrix.getNonZeroCount());
    }

    virtual std::string getName() { return "MatrixValidator"; }

    virtual bool validate(Map map) {
        for (int i = 0; i < map.m_clusters.size()-2; i++) { // For every cluster in the map except the last one:
            Cluster cluster = map.m_clusters[i];
            Cluster clusterNext = map.m_clusters[i+1];

            if (cluster.m_notes.size() == 0 || clusterNext.m_notes.size() == 0) {
                printf("\nWarning MatrixValidator encountered an empty cluster\n");
                return false;
            }

            // Skip cluster if the notes are separated enough from the last.
            if (std::abs(clusterNext.m_notes[0].m_time - cluster.m_notes[0].m_time) > Config::generator.validator.validateTimeAfterNote) {
                continue;
            }

            // If transitioning from cluster to another cluster. 
            if (cluster.m_notes.size() > 1 && clusterNext.m_notes.size() > 1) {
                // If both clusters have both colors, seperate transitions by color, blue -> blue and red -> red.
                if (cluster.isMultiColor() && clusterNext.isMultiColor()) {
                    Cluster clusterBlue = cluster.getNotesOfType(BLUE);
                    Cluster clusterRed = cluster.getNotesOfType(RED);
                    Cluster nextClusterBlue = clusterNext.getNotesOfType(BLUE);
                    Cluster nextClusterRed = clusterNext.getNotesOfType(RED);
                    for (Note cn : clusterBlue.m_notes) {
                        for (Note cnn : nextClusterBlue.m_notes) {
                            if (!m_matrix.getNoteTransition(cn, cnn)) {
                                m_fails++;
                                Validator::s_totalFails++;
                                return false;
                            }
                        }
                    }
                    for (Note cn : clusterRed.m_notes) {
                        for (Note cnn : nextClusterRed.m_notes) {
                            if (!m_matrix.getNoteTransition(cn, cnn)) {
                                m_fails++;
                                Validator::s_totalFails++;
                                return false;
                            }                        
                        }
                    }
                }
                else {
                    for (Note cn : cluster.m_notes) {
                        for (Note cnn : clusterNext.m_notes) {
                            if (!m_matrix.getNoteTransition(cn, cnn)) {
                                m_fails++;
                                Validator::s_totalFails++;
                                return false;
                            }
                        }
                    }
                }
                continue;
            }

            // If transitioning from a single note to a cluster.
            if (cluster.m_notes.size() == 1 && clusterNext.m_notes.size() > 1) {
                for (Note cnn : clusterNext.m_notes) {
                    if (!m_matrix.getNoteTransition(cluster.m_notes[0], cnn)) {
                        m_fails++;
                        Validator::s_totalFails++;
                        return false;
                    }                
                }
                continue;
            }

            // If transitioning from cluster to single note, consider each node in cluster as a transition to the next note:
            if (cluster.m_notes.size() > 1 && clusterNext.m_notes.size() == 1) {
                for (Note cn : cluster.m_notes) {
                    if (!m_matrix.getNoteTransition(cn, clusterNext.m_notes.back())) {
                        m_fails++;
                        Validator::s_totalFails++;
                        return false;
                    }                
                }
                continue;
            }

            // Single to single:
            if (m_matrix.getNoteTransition(cluster.m_notes[0], clusterNext.m_notes[0]) == false) {
                m_fails++;
                Validator::s_totalFails++;
                return false;
            }

            int transitionsFromNext = m_matrix.getTransitionCountFromNote(clusterNext.m_notes[0]);
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