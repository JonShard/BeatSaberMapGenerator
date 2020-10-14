#pragma once

#include "Map.hpp"
#include "Notation.hpp"
#include "TransitionMatrix.hpp"

namespace OK {

// MapAnalyzer parses a map and populates a TransitionMatrix with every note to note transition the map made.
class MapAnalyzer {
public:
    static TransitionMatrix<bool> RegisterTransitionsInMap(Map map); // Records all note transitions and stores them in a boolean transition matrix.
};

} // namespace OK
