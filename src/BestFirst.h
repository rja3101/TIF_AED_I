#ifndef BEST_FIRST_H
#define BEST_FIRST_H

#include <vector>
#include <map>
#include "Nodo.h"

class Grafo;

class BestFirst {
public:
    static std::vector<long long> buscar(Grafo& grafo, std::map<long long, Nodo>& nodos, long long inicio, long long destino);
};

#endif
