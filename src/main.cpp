#define SFML_STATIC
#include <SFML/Graphics.hpp>
#include <tinyxml2.h>
#include <iostream>
#include <cmath>
#include <map>
#include <set>
#include <vector>
#include "Nodo.h"
#include "Utilidades.h"
#include "CargadorOSM.h"
#include "Grafo.h"
#include "BFS.h"

using namespace tinyxml2;

std::map<long long, Nodo> nodos;
std::vector<std::vector<long long>> calles;
Grafo grafo;
std::vector<long long> caminoBFS;

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
    const int WIDTH = 1000, HEIGHT = 700;

    if (!cargarMapa("src/grafo_multilinea_conectado.osm", nodos, calles, minLat, maxLat, minLon, maxLon)) {
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

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

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

                        caminoBFS = bfs(grafo, nodoInicio, nodoDestino);
                        std::cout << "Camino BFS tiene " << caminoBFS.size() << " nodos\n";
                        for (auto id : caminoBFS) std::cout << id << " ";
                        std::cout << "\n";
                    } else {
                        nodoInicio = seleccionado;
                        nodoDestino = -1;
                        caminoBFS.clear();
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

        // Dibujar solo los nodos que forman parte de alguna calle (way con highway)
        std::set<long long> nodosEnCalles;
        for (const auto& calle : calles) {
            for (auto id : calle) {
                nodosEnCalles.insert(id);
            }
        }

        for (long long id : nodosEnCalles) {
            if (!nodos.count(id)) continue;

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
        }

        window.display();
    }

    return 0;
}
