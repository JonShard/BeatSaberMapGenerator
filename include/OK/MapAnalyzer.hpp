#pragma once

#include "Map.hpp"
#include "TransitionMatrix.hpp"

namespace OK {

class MapAnalyzer {
public:
    static TransitionMatrix<bool> AnalyzeMap(Map map); // Records all valid note transitions and stores them in a boolean transition matrix.
};

} // namespace OK
