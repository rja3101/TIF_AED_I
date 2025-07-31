#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <vector>
#include <map>
#include "Nodo.h" 

class Grafo;

class Dijkstra {
public:
    static std::vector<long long> buscar(Grafo& grafo, std::map<long long, Nodo>& nodos, long long inicio, long long destino);
};

#endif
