#ifndef DFS_H
#define DFS_H

#include "Set.h"
#include "MyVector.h"

class Grafo;

class DFS {
public:
    static MyVector<long long> buscarDFS(Grafo& grafo, long long inicio, long long destino);
};

#endif
