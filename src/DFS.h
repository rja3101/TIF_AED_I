#ifndef DFS_H
#define DFS_H

#include <vector>
#include <map>

class Grafo;

class DFS {
public:
    static std::vector<long long> buscarDFS(Grafo& grafo, long long inicio, long long destino);
};

#endif
