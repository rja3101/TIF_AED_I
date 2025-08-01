#include "Dijkstra.h"
#include "Grafo.h"
#include "Map.h"
#include "Set.h"
#include "Nodo.h"
#include "MyVector.h"
#include <cmath>  // ← sí puedes usar esto, es estándar pero no STL
#include "MathUtils.h"

// Calcula la distancia euclidiana entre dos nodos
double distanciaEntre(Nodo& a, Nodo& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

MyVector<long long> Dijkstra::buscar(Grafo& grafo, Map<long long, Nodo>& nodos, long long inicio, long long destino) {
    Map<long long, double> distancia;
    Map<long long, long long> previo;
    Set<std::pair<double, long long>> cola;

    for (Vertice* v = grafo.obtenerListaVertices(); v != nullptr; v = v->siguiente) {
        distancia.insert(v->id, std::numeric_limits<double>::infinity());
    }

    distancia.insert(inicio, 0);
    cola.insert({0, inicio});

    while (!cola.empty()) {
        std::pair<double, long long> parMin = cola.get_min();
        long long actual = parMin.second;
        cola.erase(parMin);

        if (actual == destino) break;

        Vertice* vertice = grafo.buscarVertice(actual);
        Arista* arista = vertice ? vertice->adyacentes : nullptr;

        while (arista != nullptr) {
            long long vecino = arista->destino;

            Nodo nodoActual, nodoVecino;
            nodos.get(actual, nodoActual);
            nodos.get(vecino, nodoVecino);

            double peso = distanciaEntre(nodoActual, nodoVecino);
            double distActual;
            distancia.get(actual, distActual);

            double nuevaDistancia = distActual + peso;

            double distVecino;
            distancia.get(vecino, distVecino);

            if (nuevaDistancia < distVecino) {
                cola.erase({distVecino, vecino});
                distancia.insert(vecino, nuevaDistancia);
                previo.insert(vecino, actual);
                cola.insert({nuevaDistancia, vecino});
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
