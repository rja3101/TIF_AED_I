#ifndef SET_H
#define SET_H

#include <utility>     // para std::pair
#include <functional>  // para std::function en for_each
#include <type_traits> // para std::is_same
#include <stdexcept> // necesario para std::runtime_error

#define SET_CAPACIDAD 1009

template <typename T>
struct NodoSet {
    T valor;
    NodoSet* siguiente;
    NodoSet(T v) : valor(v), siguiente(nullptr) {}
};

template <typename T>
class Set {
private:
    NodoSet<T>** tabla;

    int hash(const T& val) const {
        if constexpr (std::is_same<T, std::pair<double, long long>>::value) {
            return static_cast<int>(val.second) % SET_CAPACIDAD;
        } else if constexpr (std::is_same<T, long long>::value) {
            return static_cast<int>(val % SET_CAPACIDAD);
        } else {
            return (std::hash<T>()(val) % SET_CAPACIDAD + SET_CAPACIDAD) % SET_CAPACIDAD;
        }
    }

public:
    Set() {
        tabla = new NodoSet<T>*[SET_CAPACIDAD];
        for (int i = 0; i < SET_CAPACIDAD; ++i)
            tabla[i] = nullptr;
    }

    ~Set() {
        for (int i = 0; i < SET_CAPACIDAD; ++i) {
            NodoSet<T>* actual = tabla[i];
            while (actual) {
                NodoSet<T>* temp = actual;
                actual = actual->siguiente;
                delete temp;
            }
        }
        delete[] tabla;
    }

    void insert(const T& val) {
        if (contains(val)) return;
        int idx = hash(val);
        NodoSet<T>* nuevo = new NodoSet<T>(val);
        nuevo->siguiente = tabla[idx];
        tabla[idx] = nuevo;
    }

    void erase(const T& val) {
        int idx = hash(val);
        NodoSet<T>* actual = tabla[idx];
        NodoSet<T>* anterior = nullptr;
        while (actual) {
            if (actual->valor == val) {
                if (anterior) anterior->siguiente = actual->siguiente;
                else tabla[idx] = actual->siguiente;
                delete actual;
                return;
            }
            anterior = actual;
            actual = actual->siguiente;
        }
    }

    bool contains(const T& val) const {
        int idx = hash(val);
        NodoSet<T>* actual = tabla[idx];
        while (actual) {
            if (actual->valor == val) return true;
            actual = actual->siguiente;
        }
        return false;
    }

    bool empty() const {
        for (int i = 0; i < SET_CAPACIDAD; ++i)
            if (tabla[i] != nullptr)
                return false;
        return true;
    }

    T get_min() const {
        T min_val{};
        bool found = false;
        for (int i = 0; i < SET_CAPACIDAD; ++i) {
            NodoSet<T>* actual = tabla[i];
            while (actual) {
                if (!found || actual->valor < min_val) {
                    min_val = actual->valor;
                    found = true;
                }
                actual = actual->siguiente;
            }
        }
        if (!found) throw std::runtime_error("Set vacío, no hay mínimo");
        return min_val;
    }

    void for_each(const std::function<void(const T&)>& func) const {
        for (int i = 0; i < SET_CAPACIDAD; ++i) {
            NodoSet<T>* actual = tabla[i];
            while (actual) {
                func(actual->valor);
                actual = actual->siguiente;
            }
        }
    }
};

#endif
