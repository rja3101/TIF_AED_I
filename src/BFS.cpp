#include "BFS.h"
#include <queue>
#include <map>
#include <algorithm>

std::vector<long long> bfs(Grafo& grafo, long long inicio, long long destino) {
    std::queue<long long> cola;
    std::map<long long, long long> padre;
    std::map<long long, bool> visitado;

    cola.push(inicio);
    visitado[inicio] = true;

    while (!cola.empty()) {
        long long actual = cola.front();
        cola.pop();

        if (actual == destino) break;

        auto vertice = grafo.buscarVertice(actual);
        if (!vertice) continue;

        for (Arista* a = vertice->adyacentes; a != nullptr; a = a->siguiente) {
            long long vecino = a->destino;
            if (!visitado[vecino]) {
                visitado[vecino] = true;
                padre[vecino] = actual;
                cola.push(vecino);
            }
        }
    }

    std::vector<long long> camino;
    if (!visitado[destino]) return camino;

    for (long long v = destino; v != inicio; v = padre[v])
        camino.push_back(v);
    camino.push_back(inicio);
    std::reverse(camino.begin(), camino.end());
    return camino;
}
