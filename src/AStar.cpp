#include "AStar.h"
#include "Grafo.h"
#include <set>
#include <map>
#include <cmath>
#include <limits>
#include "Utilidades.h"



std::vector<long long> AStar::buscar(Grafo& grafo, std::map<long long, Nodo>& nodos, long long inicio, long long destino) {
    std::map<long long, double> gScore;
    std::map<long long, double> fScore;
    std::map<long long, long long> previo;
    std::set<std::pair<double, long long>> openSet; // (fScore, nodo)

    for (Vertice* v = grafo.obtenerListaVertices(); v != nullptr; v = v->siguiente) {
        gScore[v->id] = std::numeric_limits<double>::infinity();
        fScore[v->id] = std::numeric_limits<double>::infinity();
    }

    gScore[inicio] = 0;
    fScore[inicio] = heuristica(nodos[inicio], nodos[destino]);
    openSet.insert({fScore[inicio], inicio});

    while (!openSet.empty()) {
        long long actual = openSet.begin()->second;
        openSet.erase(openSet.begin());

        if (actual == destino) break;

        Vertice* vertice = grafo.buscarVertice(actual);
        Arista* arista = vertice ? vertice->adyacentes : nullptr;

        while (arista != nullptr) {
            long long vecino = arista->destino;
            double tentative_gScore = gScore[actual] + heuristica(nodos[actual], nodos[vecino]);

            if (tentative_gScore < gScore[vecino]) {
                previo[vecino] = actual;
                gScore[vecino] = tentative_gScore;
                fScore[vecino] = gScore[vecino] + heuristica(nodos[vecino], nodos[destino]);
                openSet.insert({fScore[vecino], vecino});
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
