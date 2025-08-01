#ifndef ASTAR_H
#define ASTAR_H

#include"MyVector.h"
#include"Map.h"
#include "Nodo.h"

class Grafo;

class AStar {
public: 
    static MyVector<long long> buscar(Grafo& grafo, Map<long long, Nodo>& nodos, long long inicio, long long destino);
};

#endif
