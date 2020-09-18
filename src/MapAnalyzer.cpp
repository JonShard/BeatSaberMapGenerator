#include "../include/OK/MapAnalyzer.hpp"

namespace OK {

TransitionMatrix<bool> MapAnalyzer::AnalyzeMap(Map map) {
    TransitionMatrix<bool> matrix;

    for (int i = 0; i < map.m_notes.size() - 1; i++) {
        Note n = map.m_notes[i];
        Note nn = map.m_notes[i+1];
        std::vector<Note> cluster = map.getNotesInCluster(i);
        std::vector<Note> nextCluster = map.getNotesInCluster(i + cluster.size());
        // If two notes are far enough apart, it's not a transition because the player gets time to "reset" their pose.
        if (nn.time - n.time > Config::generator.validator.validateTimeAfterNote) {
            continue;
        }

        // If transitioning from cluster to another cluster. 
        if (cluster.size() > 1 && nextCluster.size() > 1) {
            printf("Many(%ld) to Many(%ld): \n", cluster.size(), nextCluster.size());
            // If both clusters have both colors, seperate transitions by color, blue -> blue and red -> red.
            if (Map::IsClusterMultiColor(cluster) && Map::IsClusterMultiColor(nextCluster)) {
                std::vector<Note> clusterBlue = Map::GetNotesOfColorInCluster(cluster, BLUE);
                std::vector<Note> clusterRed = Map::GetNotesOfColorInCluster(cluster, RED);
                std::vector<Note> nextClusterBlue = Map::GetNotesOfColorInCluster(nextCluster, BLUE);
                std::vector<Note> nextClusterRed = Map::GetNotesOfColorInCluster(nextCluster, RED);
                for (Note cn : clusterBlue) {
                    for (Note cnn : nextClusterBlue) {
                        matrix.m_matrix[cnn.type][cn.type][cnn.cutDirection][cn.cutDirection][cnn.lineLayer][cn.lineLayer][cnn.lineIndex][cn.lineIndex] = true;
                    }
                }
                for (Note cn : clusterRed) {
                    for (Note cnn : nextClusterRed) {
                        matrix.m_matrix[cnn.type][cn.type][cnn.cutDirection][cn.cutDirection][cnn.lineLayer][cn.lineLayer][cnn.lineIndex][cn.lineIndex] = true;
                    }
                }
            }
            else {
                for (Note cn : cluster) {
                    for (Note cnn : nextCluster) {
                        matrix.m_matrix[cnn.type][cn.type][cnn.cutDirection][cn.cutDirection][cnn.lineLayer][cn.lineLayer][cnn.lineIndex][cn.lineIndex] = true;
                    }
                }
            }
            i += cluster.size();
            continue;
        }

        // If transitioning from a single note to a cluster.
        if (nextCluster.size() > 1) {
            printf("One to many(%ld): %d -> ", nextCluster.size(), i);
            for (int p = i+1; p < i + nextCluster.size()+1; p++) printf ("%d, ", p);
            for (Note cn : nextCluster) {
                matrix.m_matrix[cn.type][n.type][cn.cutDirection][n.cutDirection][cn.lineLayer][n.lineLayer][cn.lineIndex][n.lineIndex] = true;
            }
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
                matrix.m_matrix[cnn.type][cn.type][cnn.cutDirection][cn.cutDirection][cnn.lineLayer][cn.lineLayer][cnn.lineIndex][cn.lineIndex] = true;
            }
            i = firstAfterCluster - 1;
            printf(" -> %d\n", i);
            continue;
        }

        printf("One to one: %d -> %d\n", i, i + 1);
        matrix.m_matrix[nn.type][n.type][nn.cutDirection][n.cutDirection][nn.lineLayer][n.lineLayer][nn.lineIndex][n.lineIndex] = true;
    }

    return matrix;
}

} // namespace OK
