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
        if (!m_matrix.loadFromFile(Config::generator.validator.matrix.binaryMatrixFilePath)) {
            printf("\nWarning: unable to load binary matrix file at configured path: %s\n\n", 
                 Config::generator.validator.matrix.binaryMatrixFilePath.data());
        }
    }

    virtual bool validate(Map map) {
        for (int i = 0; i < map.m_notes.size()-1; i++) {
            //TODO: for all notes on same frame. Util func
            std::vector<Note> cluster = map.getNotesInCluster(i);
            // If the last two or more notes are in a cluster, return because the next note(s) doesn't exist.
            if (i + cluster.size() > map.m_notes.size() -1) {
                return true;
            }

            std::vector<Note> clusterNext = map.getNotesInCluster(i + cluster.size());
            if (clusterNext[0].time - cluster[0].time > Config::generator.validator.validateTimeAfterNote) {
                continue;
            }
            if (m_matrix.getNoteTransition(cluster[0], clusterNext[0]) == false) {
                Validator::s_fails++;
                return false;
            }
        }
        Validator::s_passes++;
        return true;
    }
};

} // Namespace OK