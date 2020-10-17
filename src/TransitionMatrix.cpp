#include "../include/OK/TransitionMatrix.hpp"

namespace OK {

template class TransitionMatrix<int>;
template class TransitionMatrix<bool>;
template class TransitionMatrix<float>;

template<class T>
TransitionMatrix<T>::TransitionMatrix() {
    for (int typeTo = 0; typeTo < c_types; typeTo++) {
        for (int typeFrom = 0; typeFrom < c_types; typeFrom++) {
            for (int dirTo = 0; dirTo < c_cutDirections; dirTo++) {
                for (int dirFrom = 0; dirFrom < c_cutDirections; dirFrom++) {
                    for (int floorTo = 0; floorTo < c_floors; floorTo++) {
                        for (int floorFrom = 0; floorFrom < c_floors; floorFrom++) {
                            for (int laneTo = 0; laneTo < c_lanes; laneTo++) {
                                for (int laneFrom = 0; laneFrom < c_lanes; laneFrom++) {
                                    m_matrix[typeTo][typeFrom][dirTo][dirFrom][floorTo][floorFrom][laneTo][laneFrom] = T();
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

template<class T>
void TransitionMatrix<T>::setNoteTransition(Note n, Note nn, T value) {
    m_matrix[nn.m_type][n.m_type][nn.m_cutDirection][n.m_cutDirection][nn.m_lineLayer][n.m_lineLayer][nn.m_lineIndex][n.m_lineIndex] = value;
}

template<class T>
T TransitionMatrix<T>::getNoteTransition(Note n, Note nn) {
    return m_matrix[nn.m_type][n.m_type][nn.m_cutDirection][n.m_cutDirection][nn.m_lineLayer][n.m_lineLayer][nn.m_lineIndex][n.m_lineIndex];
}

template<class T>
std::vector<std::pair<T, Note>> TransitionMatrix<T>::getTransitionsFromNote(Note n) {
    std::vector<std::pair<T, Note>> vector = std::vector<std::pair<T, Note>>();
    for (int typeTo = 0; typeTo < c_types; typeTo++) {
        for (int dirTo = 0; dirTo < c_cutDirections; dirTo++) {
            for (int floorTo = 0; floorTo < c_floors; floorTo++) {
                for (int laneTo = 0; laneTo < c_lanes; laneTo++) {
                    Note noteTo;
                    noteTo.m_lineIndex = laneTo;
                    noteTo.m_lineLayer = floorTo; 
                    noteTo.m_type = (Type)typeTo; 
                    noteTo.m_cutDirection = (CutDirection)dirTo;
                    vector.push_back(std::pair<T, Note>(m_matrix[typeTo][n.m_type][dirTo][n.m_cutDirection][floorTo][n.m_lineLayer][laneTo][n.m_lineIndex], noteTo)); 
                }
            }
        }
    }
    return vector;
}


template<class T>
int TransitionMatrix<T>::getTransitionCountFromNote(Note n) {
    int count = 0;
    for (int typeTo = 0; typeTo < c_types; typeTo++) {
        for (int dirTo = 0; dirTo < c_cutDirections; dirTo++) {
            for (int floorTo = 0; floorTo < c_floors; floorTo++) {
                for (int laneTo = 0; laneTo < c_lanes; laneTo++) {
                    if (m_matrix[typeTo][n.m_type][dirTo][n.m_cutDirection][floorTo][n.m_lineLayer][laneTo][n.m_lineIndex] != T()) {
                        count++;
                    }
                }
            }
        }
    }
    return count;
}


template<class T>
int TransitionMatrix<T>::getNonZeroCount() {
    int count = 0;
    for (int typeTo = 0; typeTo < c_types; typeTo++) {
        for (int typeFrom = 0; typeFrom < c_types; typeFrom++) {
            for (int dirTo = 0; dirTo < c_cutDirections; dirTo++) {
                for (int dirFrom = 0; dirFrom < c_cutDirections; dirFrom++) {
                    for (int floorTo = 0; floorTo < c_floors; floorTo++) {
                        for (int floorFrom = 0; floorFrom < c_floors; floorFrom++) {
                            for (int laneTo = 0; laneTo < c_lanes; laneTo++) {
                                for (int laneFrom = 0; laneFrom < c_lanes; laneFrom++) {
                                    if (m_matrix[typeTo][typeFrom][dirTo][dirFrom][floorTo][floorFrom][laneTo][laneFrom] != T()) {
                                        count++;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return count;
}

template<class T>
int TransitionMatrix<T>::getTotalCount() {
    return c_types * c_types * c_cutDirections * c_cutDirections * c_floors * c_floors * c_lanes * c_lanes;
}

template<class T>
void TransitionMatrix<T>::normalize() {
    for (int typeFrom = 0; typeFrom < c_types; typeFrom++) {
        for (int dirFrom = 0; dirFrom < c_cutDirections; dirFrom++) {
            for (int floorFrom = 0; floorFrom < c_floors; floorFrom++) {
                for (int laneFrom = 0; laneFrom < c_lanes; laneFrom++) {
                    Note note;
                    note.m_lineIndex = laneFrom;
                    note.m_lineLayer = floorFrom; 
                    note.m_type = (Type)typeFrom; 
                    note.m_cutDirection = (CutDirection)dirFrom;
                    std::vector<std::pair<T, Note>> transitions = getTransitionsFromNote(note);
                    float sum = 0;
                    for (std::pair<T, Note> pair : transitions) {
                        sum += pair.first;
                    }
                    if (sum == 0) { // Can't divide by zero
                        continue;
                    }
                    for (int typeTo = 0; typeTo < c_types; typeTo++) {
                        for (int dirTo = 0; dirTo < c_cutDirections; dirTo++) {
                            for (int floorTo = 0; floorTo < c_floors; floorTo++) {
                                for (int laneTo = 0; laneTo < c_lanes; laneTo++) {
                                    if (m_matrix[typeTo][typeFrom][dirTo][dirFrom][floorTo][floorFrom][laneTo][laneFrom] == 0) {
                                        continue;
                                    }
                                    m_matrix[typeTo][typeFrom][dirTo][dirFrom][floorTo][floorFrom][laneTo][laneFrom] /= sum; 
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

template<class T>
TransitionMatrix<T>::TransitionMatrix(const std::string file) {
loadFromFile(file);
}

template<class T>
bool TransitionMatrix<T>::loadFromFile(const std::string file) {
    std::ifstream in(file);
    if (in.fail()) {
        return false;
    }
    for (int typeTo = 0; typeTo < c_types; typeTo++) {
        for (int typeFrom = 0; typeFrom < c_types; typeFrom++) {
            for (int dirTo = 0; dirTo < c_cutDirections; dirTo++) {
                for (int dirFrom = 0; dirFrom < c_cutDirections; dirFrom++) {
                    for (int floorTo = 0; floorTo < c_floors; floorTo++) {
                        for (int floorFrom = 0; floorFrom < c_floors; floorFrom++) {
                            for (int laneTo = 0; laneTo < c_lanes; laneTo++) {
                                for (int laneFrom = 0; laneFrom < c_lanes; laneFrom++) {
                                    in >> m_matrix[typeTo][typeFrom][dirTo][dirFrom][floorTo][floorFrom][laneTo][laneFrom];
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return true;
}

template<class T>
void TransitionMatrix<T>::saveToFile(const std::string file) {
    std::ofstream out(file);
    for (int typeTo = 0; typeTo < c_types; typeTo++) {
        for (int typeFrom = 0; typeFrom < c_types; typeFrom++) {
            for (int dirTo = 0; dirTo < c_cutDirections; dirTo++) {
                for (int dirFrom = 0; dirFrom < c_cutDirections; dirFrom++) {
                    for (int floorTo = 0; floorTo < c_floors; floorTo++) {
                        for (int floorFrom = 0; floorFrom < c_floors; floorFrom++) {
                            for (int laneTo = 0; laneTo < c_lanes; laneTo++) {
                                for (int laneFrom = 0; laneFrom < c_lanes; laneFrom++) {
                                    out << m_matrix[typeTo][typeFrom][dirTo][dirFrom][floorTo][floorFrom][laneTo][laneFrom] <<  " "; 
                                }
                                out << "\n";    
                            }
                            out << "\n";    
                        }
                    }
                }
            }
        }
    }

    out.close();
}

template<class T>
void TransitionMatrix<T>::print() {
    printf("Matrix:\n");
    for (int typeTo = 0; typeTo < c_types; typeTo++) {
        for (int typeFrom = 0; typeFrom < c_types; typeFrom++) {
            for (int dirTo = 0; dirTo < c_cutDirections; dirTo++) {
                for (int dirFrom = 0; dirFrom < c_cutDirections; dirFrom++) {
                    for (int floorTo = 0; floorTo < c_floors; floorTo++) {
                        for (int floorFrom = 0; floorFrom < c_floors; floorFrom++) {
                            for (int laneTo = 0; laneTo < c_lanes; laneTo++) {
                                for (int laneFrom = 0; laneFrom < c_lanes; laneFrom++) {
                                    printf("%s ", std::to_string(m_matrix[typeTo][typeFrom][dirTo][dirFrom][floorTo][floorFrom][laneTo][laneFrom]).data());  
                                }
                                printf("\n");
                            }
                            printf("\n");
                        }
                    }
                }
            }
        }
    }
}

template<class T>
TransitionMatrix<T> TransitionMatrix<T>::operator+= (TransitionMatrix other) {
    for (int typeTo = 0; typeTo < c_types; typeTo++) {
        for (int typeFrom = 0; typeFrom < c_types; typeFrom++) {
            for (int dirTo = 0; dirTo < c_cutDirections; dirTo++) {
                for (int dirFrom = 0; dirFrom < c_cutDirections; dirFrom++) {
                    for (int floorTo = 0; floorTo < c_floors; floorTo++) {
                        for (int floorFrom = 0; floorFrom < c_floors; floorFrom++) {
                            for (int laneTo = 0; laneTo < c_lanes; laneTo++) {
                                for (int laneFrom = 0; laneFrom < c_lanes; laneFrom++) {
                                    m_matrix[typeTo][typeFrom][dirTo][dirFrom][floorTo][floorFrom][laneTo][laneFrom] += other.m_matrix[typeTo][typeFrom][dirTo][dirFrom][floorTo][floorFrom][laneTo][laneFrom];  
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return *this;
}


} // namespace OK