#include "Dijkstra.h"
#include "Grafo.h"
#include <map>
#include <set>
#include <limits>
#include <cmath>

double distanciaEntre(Nodo& a, Nodo& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

std::vector<long long> Dijkstra::buscar(Grafo& grafo, std::map<long long, Nodo>& nodos, long long inicio, long long destino) {
    std::map<long long, double> distancia;
    std::map<long long, long long> previo;
    std::set<std::pair<double, long long>> cola; // (distancia, nodo)

    for (Vertice* v = grafo.obtenerListaVertices(); v != nullptr; v = v->siguiente) {
        distancia[v->id] = std::numeric_limits<double>::infinity();
    }

    distancia[inicio] = 0;
    cola.insert({0, inicio});

    while (!cola.empty()) {
        long long actual = cola.begin()->second;
        cola.erase(cola.begin());

        if (actual == destino) break;

        Vertice* vertice = grafo.buscarVertice(actual);
        Arista* arista = vertice ? vertice->adyacentes : nullptr;

        while (arista != nullptr) {
            long long vecino = arista->destino;
            double peso = distanciaEntre(nodos[actual], nodos[vecino]);

            if (distancia[actual] + peso < distancia[vecino]) {
                cola.erase({distancia[vecino], vecino});
                distancia[vecino] = distancia[actual] + peso;
                previo[vecino] = actual;
                cola.insert({distancia[vecino], vecino});
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
