#include "BestFirst.h"
#include "Grafo.h"
#include <set>
#include <cmath>
#include <limits>

double heuristica(Nodo& a, Nodo& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

std::vector<long long> BestFirst::buscar(Grafo& grafo, std::map<long long, Nodo>& nodos, long long inicio, long long destino) {
    std::set<long long> visitado;
    std::map<long long, long long> previo;
    std::set<std::pair<double, long long>> cola;  // (heurística, nodo)

    cola.insert({heuristica(nodos[inicio], nodos[destino]), inicio});

    while (!cola.empty()) {
        long long actual = cola.begin()->second;
        cola.erase(cola.begin());

        if (actual == destino) break;
        if (visitado.count(actual)) continue;

        visitado.insert(actual);

        Vertice* vertice = grafo.buscarVertice(actual);
        Arista* arista = vertice ? vertice->adyacentes : nullptr;

        while (arista != nullptr) {
            long long vecino = arista->destino;
            if (!visitado.count(vecino)) {
                double h = heuristica(nodos[vecino], nodos[destino]);
                cola.insert({h, vecino});
                if (!previo.count(vecino)) {
                    previo[vecino] = actual;
                }
            }
            arista = arista->siguiente;
        }
    }

    std::vector<long long> camino;
    if (previo.find(destino) == previo.end()) return camino;

    for (long long at = destino; at != inicio; at = previo[at])
        camino.insert(camino.begin(), at);
    camino.insert(camino.begin(), inicio);

    return camino;
}
