// MapAnalyzer parses a map and populates a TransitionMatrix with every note to note transition the map made.
#pragma once

#include "Map.hpp"
#include "TransitionMatrix.hpp"

namespace OK {

class MapAnalyzer {
public:
    static TransitionMatrix<bool> AnalyzeMap(Map map); // Records all note transitions and stores them in a boolean transition matrix.
};

} // namespace OK
