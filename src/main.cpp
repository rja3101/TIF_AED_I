#define SFML_STATIC
#include <SFML/Graphics.hpp>
#include <tinyxml2.h>
#include <iostream>

#include "MyVector.h"

#include "Nodo.h"
#include "Utilidades.h"
#include "CargadorOSM.h"
#include "Grafo.h"
#include "BFS.h"
#include "DFS.h"
#include "Dijkstra.h"
#include "BestFirst.h"
#include "AStar.h"
#include"Map.h"
#include <chrono>

using namespace tinyxml2;

enum AlgoritmoBusqueda {
    BFS_ALG = 1,
    DFS_ALG,
    DIJKSTRA_ALG,
    BEST_FIRST_ALG,
    ASTAR_ALG,
    TODOS
};

int algoritmoActual = BFS_ALG;


std::map<long long, Nodo> nodos; // SOLO VISUAL

// DESPUES LOS DEMAS -------------------------------------------
Map<long long, Nodo> nodosMapAStar;
Map<long long, Nodo> nodosMapBest; 
Map<long long, Nodo> nodosMapDij; 

MyVector<MyVector<long long>> calles;



Grafo grafo;

//Vector sin std::vector
MyVector<long long> caminoAStar;
MyVector<long long> caminoBest;
MyVector<long long> caminoBFS;
MyVector<long long> caminoDFS;
MyVector<long long> caminoDijkstra;





double minLat = 999, maxLat = -999;
double minLon = 999, maxLon = -999;

long long nodoInicio = -1;
long long nodoDestino = -1;
const float radioSeleccion = 8.0f;

long long encontrarNodoCercano(float mouseX, float mouseY) {
    long long idMasCercano = -1;
    float distanciaMin = 1e9;

    for (const auto& [id, nodo] : nodos) {
        bool tieneConexion = false;
        for (const auto& calle : calles) {
            for (auto n : calle) {
                if (n == id) {
                    tieneConexion = true;
                    break;
                }
            }
            if (tieneConexion) break;
        }
        if (!tieneConexion) continue;

        float dx = nodo.x - mouseX;
        float dy = nodo.y - mouseY;
        float distancia = std::sqrt(dx * dx + dy * dy);

        if (distancia < distanciaMin && distancia <= radioSeleccion) {
            distanciaMin = distancia;
            idMasCercano = id;
        }
    }

    return idMasCercano;
}

int main() {

    Map<long long, const char*> etiquetasNodos;

    etiquetasNodos.insert(17, "Catedral de AQP");
    etiquetasNodos.insert(170, "Plaza de Armas");
    etiquetasNodos.insert(613, "UTP");
    etiquetasNodos.insert(567, "San Pablo");
    etiquetasNodos.insert(705, "Outlet Arauco");
    etiquetasNodos.insert(411, "Monasterio de Sta. Catalina");
    etiquetasNodos.insert(330, "U. Catolica de Sta. María");
    etiquetasNodos.insert(204, "UNSA");
    etiquetasNodos.insert(125, "Honorio Delgado");
    etiquetasNodos.insert(811, "UNSA Medicina");

//VISUAL -------------------------------------------------------------------------------------------------------
    const int WIDTH = 1000, HEIGHT = 700;

    if (!cargarMapa("src/map.osm", nodos, calles, minLat, maxLat, minLon, maxLon)) {
        return -1;
    }

    for (auto& par : nodos)
        convertirCoordenadas(par.second, WIDTH, HEIGHT, minLat, maxLat, minLon, maxLon);

    for (const auto& [id, nodo] : nodos)
        grafo.agregarVertice(id);

    for (const auto& calle : calles) {
        for (size_t i = 1; i < calle.size(); ++i) {
            long long from = calle[i - 1];
            long long to = calle[i];

            if (nodos.count(from) && nodos.count(to) && from != to) {
                // Verificar si los nodos están suficientemente cerca para conectar
                float dx = nodos[from].x - nodos[to].x;
                float dy = nodos[from].y - nodos[to].y;
                float distancia = std::sqrt(dx * dx + dy * dy);

                if (distancia < 1000.0f) { // Ajusta este valor según tu escala visual
                    grafo.agregarArista(from, to);
                    grafo.agregarArista(to, from);
                }
            }
        }
    }


   


    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Mapa de Arequipa - Calles");
    sf::Font fuente;
    if (!fuente.loadFromFile("arial.ttf")) {
        std::cerr << "No se pudo cargar la fuente.\n";
    }

//----------------------------------------------------------------------
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Num1) algoritmoActual = BFS_ALG;
                if (event.key.code == sf::Keyboard::Num2) algoritmoActual = DFS_ALG;
                if (event.key.code == sf::Keyboard::Num3) algoritmoActual = DIJKSTRA_ALG;
                if (event.key.code == sf::Keyboard::Num4) algoritmoActual = BEST_FIRST_ALG;
                if (event.key.code == sf::Keyboard::Num5) algoritmoActual = ASTAR_ALG;
                if (event.key.code == sf::Keyboard::Num6) algoritmoActual = TODOS;
                std::cout << "Algoritmo actual cambiado a: " << algoritmoActual << "\n";
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                float mouseX = static_cast<float>(event.mouseButton.x);
                float mouseY = static_cast<float>(event.mouseButton.y);
                long long seleccionado = encontrarNodoCercano(mouseX, mouseY);

                if (seleccionado != -1) {
                    if (nodoInicio == -1) {
                        nodoInicio = seleccionado;
                        std::cout << "Nodo A seleccionado: " << nodoInicio << "\n";
                    } else if (nodoDestino == -1 && seleccionado != nodoInicio) {
                        nodoDestino = seleccionado;
                        std::cout << "Nodo B seleccionado: " << nodoDestino << "\n";

                        if (algoritmoActual == BFS_ALG || algoritmoActual == TODOS) {
                            auto inicio = std::chrono::high_resolution_clock::now();
                            caminoBFS = bfs(grafo, nodoInicio, nodoDestino);
                            auto fin = std::chrono::high_resolution_clock::now();
                            std::chrono::duration<double, std::milli> duracion = fin - inicio;
                            std::cout << "BFS tomo: " << duracion.count() << " ms\n";
                        } else {
                            caminoBFS.clear();
                        }

                        if (algoritmoActual == DFS_ALG || algoritmoActual == TODOS) {
                            auto inicio = std::chrono::high_resolution_clock::now();
                            caminoDFS = DFS::buscarDFS(grafo, nodoInicio, nodoDestino);
                            auto fin = std::chrono::high_resolution_clock::now();
                            std::chrono::duration<double, std::milli> duracion = fin - inicio;
                            std::cout << "DFS tomo: " << duracion.count() << " ms\n";
                        } else {
                            caminoDFS.clear();
                        }

                        if (algoritmoActual == DIJKSTRA_ALG || algoritmoActual == TODOS) {
                            auto inicio = std::chrono::high_resolution_clock::now();
                            caminoDijkstra = Dijkstra::buscar(grafo, nodosMapDij, nodoInicio, nodoDestino);
                            auto fin = std::chrono::high_resolution_clock::now();
                            std::chrono::duration<double, std::milli> duracion = fin - inicio;
                            std::cout << "Dijkstra tomo: " << duracion.count() << " ms\n";
                        } else {
                            caminoDijkstra.clear();
                        }

                        if (algoritmoActual == BEST_FIRST_ALG || algoritmoActual == TODOS) {
                            auto inicio = std::chrono::high_resolution_clock::now();
                            caminoBest = BestFirst::buscar(grafo, nodosMapBest, nodoInicio, nodoDestino);
                            auto fin = std::chrono::high_resolution_clock::now();
                            std::chrono::duration<double, std::milli> duracion = fin - inicio;
                            std::cout << "Best-First Search tomo: " << duracion.count() << " ms\n";
                        } else {
                            caminoBest.clear();
                        }

                        if (algoritmoActual == ASTAR_ALG || algoritmoActual == TODOS) {
                            auto inicio = std::chrono::high_resolution_clock::now();
                            caminoAStar = AStar::buscar(grafo, nodosMapAStar, nodoInicio, nodoDestino);
                            auto fin = std::chrono::high_resolution_clock::now();
                            std::chrono::duration<double, std::milli> duracion = fin - inicio;
                            std::cout << "A* tomo: " << duracion.count() << " ms\n";
                        } else {
                            caminoAStar.clear(); 
                        }



                    } else {
                        nodoInicio = seleccionado;
                        nodoDestino = -1;
                        caminoBFS.clear();
                        caminoDFS.clear();
                        caminoDijkstra.clear();
                        caminoBest.clear();
                        caminoAStar.clear();
                        std::cout << "Nodo A cambiado a: " << nodoInicio << "\n";
                    }
                }
            }
        }

        window.clear(sf::Color::White);

        sf::Color azulClaro(30, 120, 255);
        for (const auto& calle : calles) {
            for (size_t i = 1; i < calle.size(); ++i) {
                long long id1 = calle[i - 1], id2 = calle[i];
                if (nodos.count(id1) && nodos.count(id2)) {
                    sf::Vector2f p1(nodos[id1].x, nodos[id1].y);
                    sf::Vector2f p2(nodos[id2].x, nodos[id2].y);
                    sf::Vector2f dir = p2 - p1;
                    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

                    if (len > 0.1f) {
                        sf::RectangleShape linea(sf::Vector2f(len, 1.2f));
                        linea.setFillColor(azulClaro);
                        linea.setPosition(p1);
                        linea.setRotation(std::atan2(dir.y, dir.x) * 180 / 3.14159265f);
                        window.draw(linea);
                    }
                }
            }
        }

        if (!caminoBFS.empty()) {
            for (size_t i = 1; i < caminoBFS.size(); ++i) {
                long long id1 = caminoBFS[i - 1];
                long long id2 = caminoBFS[i];
                if (nodos.count(id1) && nodos.count(id2)) {
                    sf::Vector2f p1(nodos[id1].x, nodos[id1].y);
                    sf::Vector2f p2(nodos[id2].x, nodos[id2].y);
                    sf::Vector2f dir = p2 - p1;
                    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

                    if (len > 0.1f) {
                        sf::RectangleShape linea(sf::Vector2f(len, 2.5f));
                        linea.setOrigin(0, 1.25f);
                        linea.setFillColor(sf::Color::Red);
                        linea.setPosition(p1);
                        linea.setRotation(std::atan2(dir.y, dir.x) * 180 / 3.14159265f);
                        window.draw(linea);
                    }
                }
            }
            
            // Dibujar puntos rojos sobre el camino
            for (auto id : caminoBFS) {
                if (nodos.count(id)) {
                    sf::CircleShape punto(2.5f);
                    punto.setFillColor(sf::Color::Red);
                    punto.setOrigin(2.5f, 2.5f);
                    punto.setPosition(nodos[id].x, nodos[id].y);
                    window.draw(punto);
                }
            }
        }
        if (!caminoDFS.empty()) {
            for (size_t i = 1; i < caminoDFS.size(); ++i) {
                long long id1 = caminoDFS[i - 1];
                long long id2 = caminoDFS[i];
                if (nodos.count(id1) && nodos.count(id2)) {
                    sf::Vector2f p1(nodos[id1].x, nodos[id1].y);
                    sf::Vector2f p2(nodos[id2].x, nodos[id2].y);
                    sf::Vector2f dir = p2 - p1;
                    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);

                    if (len > 0.1f) {
                        sf::RectangleShape linea(sf::Vector2f(len, 2.5f));
                        linea.setOrigin(0, 1.25f);
                        linea.setFillColor(sf::Color::Red); // Distinto color para DFS
                        linea.setPosition(p1);
                        linea.setRotation(std::atan2(dir.y, dir.x) * 180 / 3.14159265f);
                        window.draw(linea);
                    }
                }
            }

            for (auto id : caminoDFS) {
                if (nodos.count(id)) {
                    sf::CircleShape punto(2.5f);
                    punto.setFillColor(sf::Color::Red);
                    punto.setOrigin(2.5f, 2.5f);
                    punto.setPosition(nodos[id].x, nodos[id].y);
                    window.draw(punto);
                }
            }
        }

        if (!caminoDijkstra.empty()) {
            for (size_t i = 1; i < caminoDijkstra.size(); ++i) {
                long long id1 = caminoDijkstra[i - 1];
                long long id2 = caminoDijkstra[i];
                if (nodos.count(id1) && nodos.count(id2)) {
                    sf::Vector2f p1(nodos[id1].x, nodos[id1].y);
                    sf::Vector2f p2(nodos[id2].x, nodos[id2].y);
                    float len = std::sqrt((p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)*(p2.y - p1.y));
                    if (len > 0.1f) {
                        sf::RectangleShape linea(sf::Vector2f(len, 2.5f));
                        linea.setOrigin(0, 1.25f);
                        linea.setFillColor(sf::Color::Green);
                        linea.setPosition(p1);
                        linea.setRotation(std::atan2(p2.y - p1.y, p2.x - p1.x) * 180 / 3.14159265f);
                        window.draw(linea);
                    }
                }
            }

            for (auto id : caminoDijkstra) {
                if (nodos.count(id)) {
                    sf::CircleShape punto(2.5f);
                    punto.setFillColor(sf::Color::Green);
                    punto.setOrigin(2.5f, 2.5f);
                    punto.setPosition(nodos[id].x, nodos[id].y);
                    window.draw(punto);
                }
            }
        }

        if (!caminoBest.empty()) {
            for (size_t i = 1; i < caminoBest.size(); ++i) {
                long long id1 = caminoBest[i - 1];
                long long id2 = caminoBest[i];
                if (nodos.count(id1) && nodos.count(id2)) {
                    sf::Vector2f p1(nodos[id1].x, nodos[id1].y);
                    sf::Vector2f p2(nodos[id2].x, nodos[id2].y);
                    float len = std::sqrt((p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)*(p2.y - p1.y));
                    if (len > 0.1f) {
                        sf::RectangleShape linea(sf::Vector2f(len, 2.5f));
                        linea.setOrigin(0, 1.25f);
                        linea.setFillColor(sf::Color::Yellow);
                        linea.setPosition(p1);
                        linea.setRotation(std::atan2(p2.y - p1.y, p2.x - p1.x) * 180 / 3.14159265f);
                        window.draw(linea);
                    }
                }
            }

            for (size_t i = 0; i < caminoBest.size(); ++i) {
                long long id = caminoBest[i];
                if (nodos.count(id)) {
                    sf::CircleShape punto(2.5f);
                    punto.setFillColor(sf::Color::Yellow);
                    punto.setOrigin(2.5f, 2.5f);
                    punto.setPosition(nodos[id].x, nodos[id].y);
                    window.draw(punto);
                }
            }
        }

        //A*
        if (!caminoAStar.empty()) {
            for (size_t i = 1; i < caminoAStar.size(); ++i) {
                long long id1 = caminoAStar[i - 1];
                long long id2 = caminoAStar[i];
                if (nodos.count(id1) && nodos.count(id2)) {
                    sf::Vector2f p1(nodos[id1].x, nodos[id1].y);
                    sf::Vector2f p2(nodos[id2].x, nodos[id2].y);
                    float len = std::sqrt((p2.x - p1.x)*(p2.x - p1.x) + (p2.y - p1.y)*(p2.y - p1.y));
                    if (len > 0.1f) {
                        sf::RectangleShape linea(sf::Vector2f(len, 2.5f));
                        linea.setOrigin(0, 1.25f);
                        linea.setFillColor(sf::Color(255, 0, 255)); // Magenta
                        linea.setPosition(p1);
                        linea.setRotation(std::atan2(p2.y - p1.y, p2.x - p1.x) * 180 / 3.14159265f);
                        window.draw(linea);
                    }
                }
            }

            for (auto id : caminoAStar) {
                if (nodos.count(id)) {
                    sf::CircleShape punto(2.5f);
                    punto.setFillColor(sf::Color(255, 0, 255));
                    punto.setOrigin(2.5f, 2.5f);
                    punto.setPosition(nodos[id].x, nodos[id].y);
                    window.draw(punto);
                }
            }
        }


        // Dibujar solo los nodos que forman parte de alguna calle (way con highway)
        Set<long long> nodosEnCalles;
        for (const auto& calle : calles) {
            for (auto id : calle) {
                nodosEnCalles.insert(id);
            }
        }

        nodosEnCalles.for_each([&](const long long& id) {
            if (!nodos.count(id)) return;

            const Nodo& nodo = nodos[id];
            sf::CircleShape punto;

            if (id == nodoInicio) {
                punto = sf::CircleShape(4);
                punto.setFillColor(sf::Color::Green);
            } else if (id == nodoDestino) {
                punto = sf::CircleShape(4);
                punto.setFillColor(sf::Color::Red);
            } else {
                punto = sf::CircleShape(3);
                punto.setFillColor(sf::Color(128, 0, 128)); // Morado
            }

            punto.setOrigin(punto.getRadius(), punto.getRadius());
            punto.setPosition(nodo.x, nodo.y);
            window.draw(punto);
        });

        for (int i = 0; i < MAP_CAPACIDAD; ++i) {
            Par<long long, const char*>* actual = etiquetasNodos.getBucket(i);
            while (actual) {
                long long id = actual->key;
                const char* nombre = actual->value;

                if (nodos.count(id)) {
                    sf::Text texto;
                    texto.setFont(fuente);
                    texto.setString(nombre);  // const char* funciona con setString
                    texto.setCharacterSize(10);
                    texto.setFillColor(sf::Color::Black);
                    texto.setPosition(nodos[id].x + 5, nodos[id].y + 5);
                    window.draw(texto);
                }

                actual = actual->next;
            }
        }


        window.display();
    }

    return 0;
}
