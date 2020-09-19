// Matrix Validator ensures that the transition from each note to the next 
// of the same color is legal. 
#pragma once

#include "Validator.hpp"
#include "../TransitionMatrix.hpp"
#include "../Config.hpp"

namespace OK {

class MatrixValidator : public Validator {
private:
    TransitionMatrix<bool> m_matrix;
public:
    MatrixValidator() {
        m_matrix.loadFromFile("binaryTransitionMatrix.data");
    }

    virtual bool validate(Map map) {
        for (int i = 0; i < map.m_notes.size()-1; i++) {
            //TODO: for all notes on same frame. Util func
            std::vector<Note> cluster = map.getNotesInCluster(i);
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