#include "DFS.h"
#include "Grafo.h"
#include "Set.h"
#include "MyVector.h"

static bool dfsUtil(Grafo& grafo, long long actual, long long destino, Set<long long>& visitado, MyVector<long long>& camino) {
    visitado.insert(actual);
    camino.push_back(actual);

    if (actual == destino) return true;

    Vertice* vertice = grafo.buscarVertice(actual);
    Arista* vecino = vertice ? vertice->adyacentes : nullptr;

    while (vecino != nullptr) {
        if (!visitado.contains(vecino->destino)) {
            if (dfsUtil(grafo, vecino->destino, destino, visitado, camino))
                return true;
        }
        vecino = vecino->siguiente;
    }

    camino.pop_back(); 
    return false;
}

MyVector<long long> DFS::buscarDFS(Grafo& grafo, long long inicio, long long destino) {
    Set<long long> visitado;
    MyVector<long long> camino;
    dfsUtil(grafo, inicio, destino, visitado, camino);
    return camino;
}
