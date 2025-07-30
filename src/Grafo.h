// Grafo.h
#ifndef GRAFO_H
#define GRAFO_H

struct Arista {
    long long destino;
    Arista* siguiente;
};

struct Vertice {
    long long id;
    Arista* adyacentes;
    Vertice* siguiente;
};

class Grafo {
private:
    Vertice* listaVertices;

public:
    Grafo();
    ~Grafo();

    void agregarVertice(long long id);
    void agregarArista(long long origen, long long destino);
    Vertice* buscarVertice(long long id);
    Vertice* obtenerListaVertices(); // <-- NUEVO: para recorrer en BFS
    void liberar();
};

#endif
