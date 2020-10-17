#include "../include/OK/MapAnalyzer.hpp"

namespace OK {

TransitionMatrix<bool> MapAnalyzer::RegisterBinaryTransitionsInMap(Map map) {
    TransitionMatrix<bool> matrix;

    for (int i = 0; i < map.m_notes.size() - 1; i++) {
        std::vector<Note> cluster = map.getNotesInCluster(i);
        // If the last two or more notes are in a cluster, exit loop and return because the next note(s) doesn't exist.
        if (i + cluster.size() > map.m_notes.size() -1) {
            break;
        }
        std::vector<Note> clusterNext = map.getNotesInCluster(i + cluster.size());
        // If two notes are far enough apart, it's not a transition because the player gets time to "reset" their pose.
        if (std::abs(cluster[0].m_time - clusterNext[0].m_time) > Config::generator.validator.validateTimeAfterNote) {
            continue;
        }

        // If transitioning from cluster to another cluster. 
        if (cluster.size() > 1 && clusterNext.size() > 1) {
            printf("Many(%ld) to Many(%ld): \n", cluster.size(), clusterNext.size());
            // If both clusters have both colors, seperate transitions by color, blue -> blue and red -> red.
            if (Map::IsClusterMultiColor(cluster) && Map::IsClusterMultiColor(clusterNext)) {
                std::vector<Note> clusterBlue = Map::GetNotesOfColorInCluster(cluster, BLUE);
                std::vector<Note> clusterRed = Map::GetNotesOfColorInCluster(cluster, RED);
                std::vector<Note> nextClusterBlue = Map::GetNotesOfColorInCluster(clusterNext, BLUE);
                std::vector<Note> nextClusterRed = Map::GetNotesOfColorInCluster(clusterNext, RED);
                for (Note cn : clusterBlue) {
                    for (Note cnn : nextClusterBlue) {
                        matrix.setNoteTransition(cn, cnn, true);
                    }
                }
                for (Note cn : clusterRed) {
                    for (Note cnn : nextClusterRed) {
                        matrix.setNoteTransition(cn, cnn, true);
                    }
                }
            }
            else {
                for (Note cn : cluster) {
                    for (Note cnn : clusterNext) {
                        matrix.setNoteTransition(cn, cnn, true);
                    }
                }
            }
            i += cluster.size();
            continue;
        }

        // If transitioning from a single note to a cluster.
        if (clusterNext.size() > 1) {
            printf("One to many(%ld): %d -> ", clusterNext.size(), i);
            for (Note cn : clusterNext) {
                matrix.setNoteTransition(cluster[0], cn, true);
            }
            for (int p = i+1; p <= i + clusterNext.size(); p++) printf ("%d, ", p);
            printf("\n");
            continue;
        }

        // If transitioning from inside a cluster, consider each node in cluster as a transition to the next note:
        if (cluster.size() > 1) {
            int firstAfterCluster = i + cluster.size(); 
            printf("Many(%ld) to one: ", cluster.size());
            for (int j = i; j < firstAfterCluster; j++) {
                printf ("%d, ", j);
                Note cn = map.m_notes[j];
                Note cnn = map.m_notes[firstAfterCluster];
                matrix.setNoteTransition(cn, cnn, true);
            }
            i = firstAfterCluster - 1;
            printf(" -> %d\n", i);
            continue;
        }

        // If transitioning from one to one:
        printf("One to one: %d -> %d\n", i, i + 1);
        matrix.setNoteTransition(cluster[0], clusterNext[0], true);

        // Get list all transitions too B
        // If A is in list
    }

    return matrix;
}



TransitionMatrix<float> MapAnalyzer::RegisterMarkovTransitionsInMap(Map map) {
    TransitionMatrix<float> markovMatrix;

    for (int i = 0; i < map.m_notes.size() - 1; i++) {
        std::vector<Note> cluster = map.getNotesInCluster(i);
        // If the last two or more notes are in a cluster, exit loop and return because the next note(s) doesn't exist.
        if (i + cluster.size() > map.m_notes.size() -1) {
            break;
        }
        std::vector<Note> clusterNext = map.getNotesInCluster(i + cluster.size());
        // If two notes are far enough apart, it's not a transition because the player gets time to "reset" their pose.
        if (std::abs(cluster[0].m_time - clusterNext[0].m_time) > Config::generator.validator.validateTimeAfterNote) {
            continue;
        }

        // If transitioning from cluster to another cluster. 
        if (cluster.size() > 1 && clusterNext.size() > 1) {
            printf("Many(%ld) to Many(%ld): \n", cluster.size(), clusterNext.size());
            // If both clusters have both colors, seperate transitions by color, blue -> blue and red -> red.
            if (Map::IsClusterMultiColor(cluster) && Map::IsClusterMultiColor(clusterNext)) {
                std::vector<Note> clusterBlue = Map::GetNotesOfColorInCluster(cluster, BLUE);
                std::vector<Note> clusterRed = Map::GetNotesOfColorInCluster(cluster, RED);
                std::vector<Note> nextClusterBlue = Map::GetNotesOfColorInCluster(clusterNext, BLUE);
                std::vector<Note> nextClusterRed = Map::GetNotesOfColorInCluster(clusterNext, RED);
                for (Note cn : clusterBlue) {
                    for (Note cnn : nextClusterBlue) {
                        markovMatrix.setNoteTransition(cn, cnn, markovMatrix.getNoteTransition(cn, cnn) + 1);
                    }
                }
                for (Note cn : clusterRed) {
                    for (Note cnn : nextClusterRed) {
                        markovMatrix.setNoteTransition(cn, cnn, markovMatrix.getNoteTransition(cn, cnn) + 1);
                    }
                }
            }
            else {
                for (Note cn : cluster) {
                    for (Note cnn : clusterNext) {
                        markovMatrix.setNoteTransition(cn, cnn, markovMatrix.getNoteTransition(cn, cnn) + 1);
                    }
                }
            }
            i += cluster.size();
            continue;
        }

        // If transitioning from a single note to a cluster.
        if (clusterNext.size() > 1) {
            printf("One to many(%ld): %d -> ", clusterNext.size(), i);
            for (Note cn : clusterNext) {
                markovMatrix.setNoteTransition(cluster[0], cn, markovMatrix.getNoteTransition(cluster[0], cn) + 1);
            }
            for (int p = i+1; p <= i + clusterNext.size(); p++) printf ("%d, ", p);
            printf("\n");
            continue;
        }

        // If transitioning from inside a cluster, consider each node in cluster as a transition to the next note:
        if (cluster.size() > 1) {
            int firstAfterCluster = i + cluster.size(); 
            printf("Many(%ld) to one: ", cluster.size());
            for (int j = i; j < firstAfterCluster; j++) {
                printf ("%d, ", j);
                Note cn = map.m_notes[j];
                Note cnn = map.m_notes[firstAfterCluster];
                markovMatrix.setNoteTransition(cn, cnn, markovMatrix.getNoteTransition(cn, cnn) + 1);
            }
            i = firstAfterCluster - 1;
            printf(" -> %d\n", i);
            continue;
        }

        // If transitioning from one to one:
        printf("One to one: %d -> %d\n", i, i + 1);
        markovMatrix.setNoteTransition(cluster[0], clusterNext[0], markovMatrix.getNoteTransition(cluster[0], clusterNext[0]) + 1);
    }
    markovMatrix.normalize();
    return markovMatrix;
}
} // namespace OK
