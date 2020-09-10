// Matrix Validator checks that the transition from each note to the next 
// of the same color is legal. 
#pragma once

#include "Validator.hpp"
#include "../TransitionMatrix.hpp"

namespace OK {

// template <class T>
// class TransitionMatrix;

class MatrixValidator : public Validator {
private:
    TransitionMatrix<bool> m_matrix;
public:
    MatrixValidator() {
    }

    virtual void loadConfig() {
        
    }

    virtual bool validate(Map map) {

        return false;
    }
};

} // Namespace OK