#include "BFS.h"
#include "Queue.h"
#include "Map.h"
#include "MyVector.h"

MyVector<long long> bfs(Grafo& grafo, long long inicio, long long destino) {
    Queue<long long> cola;
    Map<long long, long long> padre;
    Map<long long, bool> visitado;

    cola.push(inicio);
    visitado.insert(inicio, true);

    while (!cola.empty()) {
        long long actual = cola.front();
        cola.pop();

        if (actual == destino) break;

        auto vertice = grafo.buscarVertice(actual);
        if (!vertice) continue;

        for (Arista* a = vertice->adyacentes; a != nullptr; a = a->siguiente) {
            long long vecino = a->destino;
            bool yaVisitado = false;
            visitado.get(vecino, yaVisitado);
            if (!yaVisitado) {
                visitado.insert(vecino, true);
                padre.insert(vecino, actual);
                cola.push(vecino);
            }
        }
    }

    MyVector<long long> camino;
    bool encontrado = false;
    visitado.get(destino, encontrado);
    if (!encontrado) return camino;

    long long v = destino;
    while (v != inicio) {
        camino.push_back(v);
        padre.get(v, v);
    }
    camino.push_back(inicio);

    // Revertir camino manualmente
    for (int i = 0, j = camino.size() - 1; i < j; ++i, --j) {
        long long temp = camino[i];
        camino[i] = camino[j];
        camino[j] = temp;
    }

    return camino;
}
