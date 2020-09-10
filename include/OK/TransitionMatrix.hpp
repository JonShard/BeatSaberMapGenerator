#pragma once

#include "Map.hpp"

namespace OK {

const int c_lanes = 4;
const int c_floors = 3;
const int c_types = 9; 
const int c_colors = 2;

template <class T>
class TransitionMatrix {
private:
    //                                     LaneFrom  LaneTo
    T m_matrix[c_colors][c_types][c_floors][c_lanes][c_lanes];
public:
    TransitionMatrix();
    TransitionMatrix(std::string file);

    void loadFromFile();
    void print();
};

} // namespace OK