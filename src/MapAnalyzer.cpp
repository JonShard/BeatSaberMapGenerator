#include "../include/OK/MapAnalyzer.hpp"

namespace OK {

TransitionMatrix<bool> MapAnalyzer::AnalyzeMap(Map map) {
    TransitionMatrix<bool> matrix;

    for (int i = 0; i < map.m_notes.size()-1; i++) {
        Note n = map.m_notes[i];
        Note nn = map.m_notes[i+1];
        // If two notes are far enough apart, it's not a transition because the player gets time to "reset" their pose.
        if (nn.time - n.time > Config::generator.validator.validateTimeAfterNote) {
            continue;
        }
        matrix.m_matrix[nn.type][n.type][nn.cutDirection][n.cutDirection][nn.lineLayer][n.lineLayer][nn.lineIndex][n.lineIndex] = true;
    }

    return matrix;
}

} // namespace OK
