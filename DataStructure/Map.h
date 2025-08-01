#ifndef MAP_H
#define MAP_H

#define MAP_CAPACIDAD 1009
#include "MyVector.h"
#include "Nodo.h"

template <typename K, typename V>
struct Par {
    K key;
    V value;
    Par* next;

    // Constructor por defecto requerido por MyVector
    Par() : key(), value(), next(nullptr) {}

    // Constructor con parámetros
    Par(K k, V v) : key(k), value(v), next(nullptr) {}
};

template <typename K, typename V>
class Map {
private:
    Par<K, V>** tabla;

    int hash(K key) const {
        return (static_cast<int>(key) % MAP_CAPACIDAD + MAP_CAPACIDAD) % MAP_CAPACIDAD;
    }

public:
    Map() {
        tabla = new Par<K, V>*[MAP_CAPACIDAD];
        for (int i = 0; i < MAP_CAPACIDAD; ++i)
            tabla[i] = nullptr;
    }

    ~Map() {
        for (int i = 0; i < MAP_CAPACIDAD; ++i) {
            Par<K, V>* actual = tabla[i];
            while (actual) {
                Par<K, V>* temp = actual;
                actual = actual->next;
                delete temp;
            }
        }
        delete[] tabla;
    }

    void insert(K key, V value) {
        int idx = hash(key);
        Par<K, V>* actual = tabla[idx];
        while (actual) {
            if (actual->key == key) {
                actual->value = value;
                return;
            }
            actual = actual->next;
        }
        Par<K, V>* nuevo = new Par<K, V>(key, value);
        nuevo->next = tabla[idx];
        tabla[idx] = nuevo;
    }

    bool get(K key, V& value) const {
        int idx = hash(key);
        Par<K, V>* actual = tabla[idx];
        while (actual) {
            if (actual->key == key) {
                value = actual->value;
                return true;
            }
            actual = actual->next;
        }
        return false;
    }

    bool contains(K key) const {
        int idx = hash(key);
        Par<K, V>* actual = tabla[idx];
        while (actual) {
            if (actual->key == key)
                return true;
            actual = actual->next;
        }
        return false;
    }

    Par<K, V>* getBucket(int idx) const {
        if (idx < 0 || idx >= MAP_CAPACIDAD) return nullptr;
        return tabla[idx];
    }

    MyVector<Par<K, V>> entries() const {
        MyVector<Par<K, V>> resultado;
        for (int i = 0; i < MAP_CAPACIDAD; ++i) {
            Par<K, V>* actual = tabla[i];
            while (actual) {
                resultado.push_back(*actual);  // copia el Par
                actual = actual->next;
            }
        }
        return resultado;
    }
};

#endif
