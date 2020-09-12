// Matrix Validator checks that the transition from each note to the next 
// of the same color is legal. 
#pragma once

#include "Validator.hpp"
#include "../TransitionMatrix.hpp"

namespace OK {

class MatrixValidator : public Validator {
private:
    TransitionMatrix<bool> m_matrix;
public:
    MatrixValidator() {
        loadConfig();
    }

    virtual void loadConfig() {
        m_matrix.loadFromFile("binaryTransitionMatrix.data");
    }

    virtual bool validate(Map map) {
        for (int i = 0; i < map.m_notes.size()-1; i++) {
            //TODO: for all notes on same frame. Util func
            Note n = map.m_notes[i];
            Note nn = map.m_notes[i+1];
            if (m_matrix.m_matrix[n.type][n.cutDirection][n.lineLayer][nn.lineIndex][n.lineIndex] == false) {
                return false;
            }
        }
        return true;
    }
};

} // Namespace OK