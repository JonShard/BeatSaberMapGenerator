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
            Note n = map.m_notes[i];
            Note nn = map.m_notes[i+1];
            if (nn.time - n.time > Config::generator.validator.validateTimeAfterNote) {
                Validator::s_passes++;
                return true;
            }
            if (m_matrix.m_matrix[nn.type][n.type][nn.cutDirection][n.cutDirection][nn.lineLayer][n.lineLayer][nn.lineIndex][n.lineIndex] == false) {
                Validator::s_fails++;
                return false;
            }
        }
        Validator::s_passes++;
        return true;
    }
};

} // Namespace OK