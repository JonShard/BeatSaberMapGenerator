#include "../include/OK/MapAnalyzer.hpp"

namespace OK {

TransitionMatrix<bool> MapAnalyzer::RegisterBinaryTransitionsInMap(Map map) {
    TransitionMatrix<bool> matrix;

    for (int i = 0; i < map.m_clusters.size() - 2; i++) {
        Cluster cluster = map.m_clusters[i];
        Cluster clusterNext = map.m_clusters[i+1];

        if (cluster.m_notes.size() == 0 || clusterNext.m_notes.size() == 0) {
            printf("\nWarning MatrixValidator encountered an empty cluster\n");
            return matrix;
        }

        // Skip cluster if the notes are separated enough from the last.
        if (std::abs(clusterNext.m_notes.front().m_time - cluster.m_notes.front().m_time) > Config::generator.validator.validateTimeAfterNote) {
            continue;
        }

            // If transitioning from cluster to another cluster. 
            if (cluster.m_notes.size() > 1 && clusterNext.m_notes.size() > 1) {
                printf("Many(%ld) to many(%ld)\n", cluster.m_notes.size(), clusterNext.m_notes.size());
                // If both clusters have both colors, seperate transitions by color, blue -> blue and red -> red.
                if (cluster.isMultiColor() && clusterNext.isMultiColor()) {
                    Cluster clusterBlue = cluster.getNotesOfType(BLUE);
                    Cluster clusterRed = cluster.getNotesOfType(RED);
                    Cluster nextClusterBlue = clusterNext.getNotesOfType(BLUE);
                    Cluster nextClusterRed = clusterNext.getNotesOfType(RED);
                for (Note cn : clusterBlue.m_notes) {
                    for (Note cnn : nextClusterBlue.m_notes) {
                        matrix.setNoteTransition(cn, cnn, true);
                    }
                }
                for (Note cn : clusterRed.m_notes) {
                    for (Note cnn : nextClusterRed.m_notes) {
                        matrix.setNoteTransition(cn, cnn, true);
                    }
                }
            }
            else {
                for (Note cn : cluster.m_notes) {
                    for (Note cnn : clusterNext.m_notes) {
                        matrix.setNoteTransition(cn, cnn, true);
                    }
                }
            }
            continue;
        }

        // If transitioning from a single note to a cluster.
        if (cluster.m_notes.size() == 1 && clusterNext.m_notes.size() > 1) {
            printf("One to many(%ld)\n", clusterNext.m_notes.size());
            for (Note cnn : clusterNext.m_notes) {
                matrix.setNoteTransition(cluster.m_notes.front(), cnn, true);
            }
            continue;
        }

        // If transitioning from inside a cluster, consider each node in cluster as a transition to the next note:
        if (cluster.m_notes.size() > 1 && clusterNext.m_notes.size() == 1) {
            printf("Many(%ld) to one\n", cluster.m_notes.size()); 
            for (Note cn : cluster.m_notes) {
                matrix.setNoteTransition(cn, clusterNext.m_notes.front(), true);
            }
            continue;
        }

        // Single to single:
        printf("One to one\n");
        matrix.setNoteTransition(cluster.m_notes.front(), clusterNext.m_notes.front(), true);
    }

    return matrix;
}



TransitionMatrix<float> MapAnalyzer::RegisterMarkovTransitionsInMap(Map map) {
    TransitionMatrix<float> markovMatrix;

    for (int i = 0; i < map.m_clusters.size() - 2; i++) {
        Cluster cluster = map.m_clusters[i];
        Cluster clusterNext = map.m_clusters[i+1];

        if (cluster.m_notes.size() == 0 || clusterNext.m_notes.size() == 0) {
            printf("\nWarning MatrixValidator encountered an empty cluster\n");
            return markovMatrix;
        }

        // Skip cluster if the notes are separated enough from the last.
        if (std::abs(clusterNext.m_notes.front().m_time - cluster.m_notes.front().m_time) > Config::generator.validator.validateTimeAfterNote) {
            continue;
        }

            // If transitioning from cluster to another cluster. 
            if (cluster.m_notes.size() > 1 && clusterNext.m_notes.size() > 1) {
                printf("Many(%ld) to many(%ld)\n", cluster.m_notes.size(), clusterNext.m_notes.size());
                // If both clusters have both colors, seperate transitions by color, blue -> blue and red -> red.
                if (cluster.isMultiColor() && clusterNext.isMultiColor()) {
                    Cluster clusterBlue = cluster.getNotesOfType(BLUE);
                    Cluster clusterRed = cluster.getNotesOfType(RED);
                    Cluster nextClusterBlue = clusterNext.getNotesOfType(BLUE);
                    Cluster nextClusterRed = clusterNext.getNotesOfType(RED);
                for (Note cn : clusterBlue.m_notes) {
                    for (Note cnn : nextClusterBlue.m_notes) {
                        markovMatrix.setNoteTransition(cn, cnn, markovMatrix.getNoteTransition(cn, cnn) + 1);
                    }
                }
                for (Note cn : clusterRed.m_notes) {
                    for (Note cnn : nextClusterRed.m_notes) {
                        markovMatrix.setNoteTransition(cn, cnn, markovMatrix.getNoteTransition(cn, cnn) + 1);
                    }
                }
            }
            else {
                for (Note cn : cluster.m_notes) {
                    for (Note cnn : clusterNext.m_notes) {
                        markovMatrix.setNoteTransition(cn, cnn, markovMatrix.getNoteTransition(cn, cnn) + 1);
                    }
                }
            }
            continue;
        }

        // If transitioning from a single note to a cluster.
        if (cluster.m_notes.size() == 1 && clusterNext.m_notes.size() > 1) {
            printf("One to many(%ld)\n", clusterNext.m_notes.size());
            for (Note cnn : clusterNext.m_notes) {
                markovMatrix.setNoteTransition(cluster.m_notes.front(), cnn, markovMatrix.getNoteTransition(cluster.m_notes.front(), cnn) + 1);
            }
            continue;
        }

        // If transitioning from inside a cluster, consider each node in cluster as a transition to the next note:
        if (cluster.m_notes.size() > 1 && clusterNext.m_notes.size() == 1) {
            printf("Many(%ld) to one\n", cluster.m_notes.size()); 
            for (Note cn : cluster.m_notes) {
                markovMatrix.setNoteTransition(cn, clusterNext.m_notes.front(), markovMatrix.getNoteTransition(cn, clusterNext.m_notes.front()) + 1);
            }
            continue;
        }

        // Single to single:
        printf("One to one\n");
        markovMatrix.setNoteTransition(cluster.m_notes.front(), clusterNext.m_notes.front(), markovMatrix.getNoteTransition(cluster.m_notes.front(), clusterNext.m_notes.front()) + 1);
    }
    
    markovMatrix.normalize();
    return markovMatrix;
}
} // namespace OK
