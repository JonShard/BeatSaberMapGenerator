#include "../include/OK/TransitionMatrix.hpp"

namespace OK {

template class TransitionMatrix<int>;
template class TransitionMatrix<bool>;
template class TransitionMatrix<float>;

template<class T>
TransitionMatrix<T>::TransitionMatrix() {
    for (int colorTo = 0; colorTo < c_colors; colorTo++) {
        for (int colorFrom = 0; colorFrom < c_colors; colorFrom++) {
            for (int typeTo = 0; typeTo < c_types; typeTo++) {
                for (int typeFrom = 0; typeFrom < c_types; typeFrom++) {
                    for (int floorTo = 0; floorTo < c_floors; floorTo++) {
                        for (int floorFrom = 0; floorFrom < c_floors; floorFrom++) {
                            for (int laneTo = 0; laneTo < c_lanes; laneTo++) {
                                for (int laneFrom = 0; laneFrom < c_lanes; laneFrom++) {
                                    m_matrix[colorTo][colorFrom][typeTo][typeFrom][floorTo][floorFrom][laneTo][laneFrom] = T();
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
int TransitionMatrix<T>::getTransitionCountFromNote(Note n) {
    int count = 0;
    for (int colorTo = 0; colorTo < c_colors; colorTo++) {
        for (int typeTo = 0; typeTo < c_types; typeTo++) {
            for (int floorTo = 0; floorTo < c_floors; floorTo++) {
                for (int laneTo = 0; laneTo < c_lanes; laneTo++) {
                    if (m_matrix[colorTo][n.m_type][typeTo][n.m_cutDirection][floorTo][n.m_lineLayer][laneTo][n.m_lineIndex] != T()) {
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
    for (int colorTo = 0; colorTo < c_colors; colorTo++) {
        for (int colorFrom = 0; colorFrom < c_colors; colorFrom++) {
            for (int typeTo = 0; typeTo < c_types; typeTo++) {
                for (int typeFrom = 0; typeFrom < c_types; typeFrom++) {
                    for (int floorTo = 0; floorTo < c_floors; floorTo++) {
                        for (int floorFrom = 0; floorFrom < c_floors; floorFrom++) {
                            for (int laneTo = 0; laneTo < c_lanes; laneTo++) {
                                for (int laneFrom = 0; laneFrom < c_lanes; laneFrom++) {
                                    if (m_matrix[colorTo][colorFrom][typeTo][typeFrom][floorTo][floorFrom][laneTo][laneFrom] != T()) {
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
TransitionMatrix<T>::TransitionMatrix(const std::string file) {
loadFromFile(file);
}

template<class T>
bool TransitionMatrix<T>::loadFromFile(const std::string file) {
    std::ifstream in(file);
    if (in.fail()) {
        return false;
    }
    for (int colorTo = 0; colorTo < c_colors; colorTo++) {
        for (int colorFrom = 0; colorFrom < c_colors; colorFrom++) {
            for (int typeTo = 0; typeTo < c_types; typeTo++) {
                for (int typeFrom = 0; typeFrom < c_types; typeFrom++) {
                    for (int floorTo = 0; floorTo < c_floors; floorTo++) {
                        for (int floorFrom = 0; floorFrom < c_floors; floorFrom++) {
                            for (int laneTo = 0; laneTo < c_lanes; laneTo++) {
                                for (int laneFrom = 0; laneFrom < c_lanes; laneFrom++) {
                                    in >> m_matrix[colorTo][colorFrom][typeTo][typeFrom][floorTo][floorFrom][laneTo][laneFrom];
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
    for (int colorTo = 0; colorTo < c_colors; colorTo++) {
        for (int colorFrom = 0; colorFrom < c_colors; colorFrom++) {
            for (int typeTo = 0; typeTo < c_types; typeTo++) {
                for (int typeFrom = 0; typeFrom < c_types; typeFrom++) {
                    for (int floorTo = 0; floorTo < c_floors; floorTo++) {
                        for (int floorFrom = 0; floorFrom < c_floors; floorFrom++) {
                            for (int laneTo = 0; laneTo < c_lanes; laneTo++) {
                                for (int laneFrom = 0; laneFrom < c_lanes; laneFrom++) {
                                    out << m_matrix[colorTo][colorFrom][typeTo][typeFrom][floorTo][floorFrom][laneTo][laneFrom] <<  " "; 
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
    for (int colorTo = 0; colorTo < c_colors; colorTo++) {
        for (int colorFrom = 0; colorFrom < c_colors; colorFrom++) {
            for (int typeTo = 0; typeTo < c_types; typeTo++) {
                for (int typeFrom = 0; typeFrom < c_types; typeFrom++) {
                    for (int floorTo = 0; floorTo < c_floors; floorTo++) {
                        for (int floorFrom = 0; floorFrom < c_floors; floorFrom++) {
                            for (int laneTo = 0; laneTo < c_lanes; laneTo++) {
                                for (int laneFrom = 0; laneFrom < c_lanes; laneFrom++) {
                                    printf("%s ", std::to_string(m_matrix[colorTo][colorFrom][typeTo][typeFrom][floorTo][floorFrom][laneTo][laneFrom]).data());  
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
    for (int colorTo = 0; colorTo < c_colors; colorTo++) {
        for (int colorFrom = 0; colorFrom < c_colors; colorFrom++) {
            for (int typeTo = 0; typeTo < c_types; typeTo++) {
                for (int typeFrom = 0; typeFrom < c_types; typeFrom++) {
                    for (int floorTo = 0; floorTo < c_floors; floorTo++) {
                        for (int floorFrom = 0; floorFrom < c_floors; floorFrom++) {
                            for (int laneTo = 0; laneTo < c_lanes; laneTo++) {
                                for (int laneFrom = 0; laneFrom < c_lanes; laneFrom++) {
                                    m_matrix[colorTo][colorFrom][typeTo][typeFrom][floorTo][floorFrom][laneTo][laneFrom] += other.m_matrix[colorTo][colorFrom][typeTo][typeFrom][floorTo][floorFrom][laneTo][laneFrom];  
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