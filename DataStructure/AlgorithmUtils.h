#ifndef ALGORITHM_UTILS_H
#define ALGORITHM_UTILS_H

// swap sin std::swap
template <typename T>
inline void swapd(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

// min sin std::min
template <typename T>
inline T mind(T a, T b) {
    return (a < b) ? a : b;
}

// max sin std::max
template <typename T>
inline T maxd(T a, T b) {
    return (a > b) ? a : b;
}

// simple bubble sort (puedes mejorarla si necesitas eficiencia)
template <typename T>
void bubbleSort(T* arr, size_t n) {
    for (size_t i = 0; i < n - 1; ++i) {
        for (size_t j = 0; j < n - i - 1; ++j) {
            if (arr[j + 1] < arr[j]) {
                swapd(arr[j], arr[j + 1]);
            }
        }
    }
}

// búsqueda secuencial (como std::find)
template <typename T>
int findIndex(const T* arr, size_t n, const T& value) {
    for (size_t i = 0; i < n; ++i) {
        if (arr[i] == value) return static_cast<int>(i);
    }
    return -1;
}

#endif
