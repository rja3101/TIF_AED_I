// Grafo.cpp
#include "Grafo.h"
#include <iostream>
#include <cmath>

Grafo::Grafo() {
    listaVertices = nullptr;
}

Grafo::~Grafo() {
    liberar();
}

void Grafo::agregarVertice(long long id) {
    if (buscarVertice(id)) return;
    Vertice* nuevo = new Vertice{id, nullptr, listaVertices};
    listaVertices = nuevo;
}

void Grafo::agregarArista(long long origen, long long destino) {
    Vertice* vOrigen = buscarVertice(origen);
    Vertice* vDestino = buscarVertice(destino);
    if (!vOrigen || !vDestino) return;

    Arista* nueva = new Arista{static_cast<int>(destino), vOrigen->adyacentes};
    vOrigen->adyacentes = nueva;
}

Vertice* Grafo::buscarVertice(long long id) {
    Vertice* actual = listaVertices;
    while (actual) {
        if (actual->id == id) return actual;
        actual = actual->siguiente;
    }
    return nullptr;
}

Vertice* Grafo::obtenerListaVertices() {
    return listaVertices;
}

void Grafo::liberar() {
    while (listaVertices) {
        Vertice* tempV = listaVertices;
        Arista* arista = tempV->adyacentes;
        while (arista) {
            Arista* tempA = arista;
            arista = arista->siguiente;
            delete tempA;
        }
        listaVertices = listaVertices->siguiente;
        delete tempV;
    }
}
