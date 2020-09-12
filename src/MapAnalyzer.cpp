#include "../include/OK/MapAnalyzer.hpp"

namespace OK {

TransitionMatrix<bool> MapAnalyzer::AnalyzeMap(Map map) {
    TransitionMatrix<bool> matrix;

    for (int i = 0; i < map.m_notes.size()-1; i++) {
        //TODO: for all notes on same frame. Util func
        Note n = map.m_notes[i];
        Note nn = map.m_notes[i+1];
        matrix.m_matrix[nn.type][n.type][nn.cutDirection][n.cutDirection][nn.lineLayer][n.lineLayer][nn.lineIndex][n.lineIndex] = true;
    }

    return matrix;
}

} // namespace OK
