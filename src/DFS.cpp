#include "DFS.h"
#include "Grafo.h"
#include <set>

static bool dfsUtil(Grafo& grafo, long long actual, long long destino, std::set<long long>& visitado, std::vector<long long>& camino) {
    visitado.insert(actual);
    camino.push_back(actual);

    if (actual == destino) return true;

    Vertice* vertice = grafo.buscarVertice(actual);
    Arista* vecino = vertice ? vertice->adyacentes : nullptr;

    while (vecino != nullptr) {
        if (visitado.find(vecino->destino) == visitado.end()) {
            if (dfsUtil(grafo, vecino->destino, destino, visitado, camino))
                return true;
        }
        vecino = vecino->siguiente;
    }

    camino.pop_back(); // backtrack
    return false;
}

std::vector<long long> DFS::buscarDFS(Grafo& grafo, long long inicio, long long destino) {
    std::set<long long> visitado;
    std::vector<long long> camino;
    dfsUtil(grafo, inicio, destino, visitado, camino);
    return camino;
}
