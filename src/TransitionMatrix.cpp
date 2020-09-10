#include "../include/OK/TransitionMatrix.hpp"

namespace OK {

template class TransitionMatrix<int>;
template class TransitionMatrix<bool>;
template class TransitionMatrix<float>;

template<class T>
TransitionMatrix<T>::TransitionMatrix() {
    for (int c = 0; c < c_colors; c++) {
        for (int t = 0; t < c_types; t++) {
            for (int f = 0; f < c_floors; f++) {
                for (int lt = 0; lt < c_lanes; lt++) {
                    for (int lf = 0; lf < c_lanes; lf++) {
                        m_matrix[c][t][f][lt][lf] = T();
                    }
                }
            }
        }
    }
    print();
}

template<class T>
TransitionMatrix<T>::TransitionMatrix(std::string file) {
    printf("hello");
}

template<class T>
void TransitionMatrix<T>::loadFromFile() {

}

template<class T>
void TransitionMatrix<T>::print() {
    printf("Matrix:\n ");
    for (int c = 0; c < c_colors; c++) {
        for (int t = 0; t < c_types; t++) {
            for (int f = 0; f < c_floors; f++) {
                for (int lt = 0; lt < c_lanes; lt++) {
                    for (int lf = 0; lf < c_lanes; lf++) {
                        printf(" %s", std::to_string(m_matrix[c][t][f][lf][lt]).data());  
                    }
                    printf("\n");
                }
                printf("\n\n\n");
            }
        }
    }
}

} // namespace OK