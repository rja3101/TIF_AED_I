#include "AStar.h"
#include "Grafo.h"
#include "Map.h"
#include "Set.h"
#include "MyVector.h"
#include "Utilidades.h"
#include <cmath>

MyVector<long long> AStar::buscar(Grafo& grafo, Map<long long, Nodo>& nodos, long long inicio, long long destino) {
    Map<long long, double> gScore;

    Map<long long, double> fScore;
    Map<long long, long long> previo;
    Set<std::pair<double, long long>> openSet;

    // Inicializar gScore y fScore
    for (Vertice* v = grafo.obtenerListaVertices(); v != nullptr; v = v->siguiente) {
        gScore.insert(v->id, std::numeric_limits<double>::infinity()); //Solo cmath por infinito
        fScore.insert(v->id, std::numeric_limits<double>::infinity());
    }

    Nodo nodoInicio, nodoDestino;
    nodos.get(inicio, nodoInicio);
    nodos.get(destino, nodoDestino);

    gScore.insert(inicio, 0);
    fScore.insert(inicio, heuristica(nodoInicio, nodoDestino));
    openSet.insert({heuristica(nodoInicio, nodoDestino), inicio});

    while (!openSet.empty()) {
        std::pair<double, long long> minPar = openSet.get_min();
        long long actual = minPar.second;
        openSet.erase(minPar);

        if (actual == destino) break;

        Vertice* vertice = grafo.buscarVertice(actual);
        Arista* arista = vertice ? vertice->adyacentes : nullptr;

        while (arista != nullptr) {
            long long vecino = arista->destino;

            double g_actual;
            gScore.get(actual, g_actual);

            Nodo nodoActual, nodoVecino;
            nodos.get(actual, nodoActual);
            nodos.get(vecino, nodoVecino);

            double tentative_gScore = g_actual + heuristica(nodoActual, nodoVecino);

            double g_vecino;
            gScore.get(vecino, g_vecino);

            if (tentative_gScore < g_vecino) {
                previo.insert(vecino, actual);
                gScore.insert(vecino, tentative_gScore);
                double f = tentative_gScore + heuristica(nodoVecino, nodoDestino);
                fScore.insert(vecino, f);
                openSet.insert({f, vecino});
            }

            arista = arista->siguiente;
        }
    }

    MyVector<long long> camino;
    if (!previo.contains(destino)) return camino;

    long long at = destino;
    MyVector<long long> inverso;
    while (at != inicio) {
        inverso.push_back(at);
        previo.get(at, at);
    }
    inverso.push_back(inicio);

    // Revertir el vector
    for (int i = inverso.size() - 1; i >= 0; --i) {
        camino.push_back(inverso[i]);
    }

    return camino;
}