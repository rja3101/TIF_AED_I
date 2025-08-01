#include "BestFirst.h"
#include "Grafo.h"
#include "Set.h"
#include "Map.h"
#include "MyVector.h"
#include "Utilidades.h"
#include <cmath>

MyVector<long long> BestFirst::buscar(Grafo& grafo, Map<long long, Nodo>& nodos, long long inicio, long long destino) {
    Set<long long> visitado;
    Map<long long, long long> previo;
    Set<std::pair<double, long long>> cola;  // (heurística, nodo)

    Nodo nodoInicio, nodoDestino;
    nodos.get(inicio, nodoInicio);
    nodos.get(destino, nodoDestino);

    cola.insert({heuristica(nodoInicio, nodoDestino), inicio});

    while (!cola.empty()) {
        std::pair<double, long long> parMin = cola.get_min();
        long long actual = parMin.second;
        cola.erase(parMin);

        if (actual == destino) break;
        if (visitado.contains(actual)) continue;

        visitado.insert(actual);

        Vertice* vertice = grafo.buscarVertice(actual);
        Arista* arista = vertice ? vertice->adyacentes : nullptr;

        while (arista != nullptr) {
            long long vecino = arista->destino;
            if (!visitado.contains(vecino)) {
                Nodo nodoVecino;
                nodos.get(vecino, nodoVecino);

                double h = heuristica(nodoVecino, nodoDestino);
                cola.insert({h, vecino});
                if (!previo.contains(vecino)) {
                    previo.insert(vecino, actual);
                }
            }
            arista = arista->siguiente;
        }
    }

    MyVector<long long> inverso;
    if (!previo.contains(destino)) return inverso;

    long long at = destino;
    while (at != inicio) {
        inverso.push_back(at);
        previo.get(at, at);
    }
    inverso.push_back(inicio);

    MyVector<long long> camino;
    for (int i = inverso.size() - 1; i >= 0; --i) {
        camino.push_back(inverso[i]);
    }

    return camino;
}
