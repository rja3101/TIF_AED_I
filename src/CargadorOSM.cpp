#include "CargadorOSM.h"
#include <tinyxml2.h>
#include <iostream>
using namespace tinyxml2;

bool cargarMapa(const char* archivo,
                std::map<long long, Nodo>& nodos,
                std::vector<std::vector<long long>>& calles,
                double& minLat, double& maxLat,
                double& minLon, double& maxLon) {

    XMLDocument doc;
    if (doc.LoadFile(archivo) != XML_SUCCESS) {
        std::cerr << "[ERROR] No se pudo cargar el archivo OSM.\n";
        return false;
    }

    XMLElement* root = doc.FirstChildElement("osm");
    if (!root) return false;

    minLat = 999; maxLat = -999;
    minLon = 999; maxLon = -999;

    // Leer nodos
    for (XMLElement* e = root->FirstChildElement("node"); e; e = e->NextSiblingElement("node")) {
        long long id = e->Int64Attribute("id");
        double lat = e->DoubleAttribute("lat");
        double lon = e->DoubleAttribute("lon");

        Nodo nodo = {id, lat, lon, 0, 0};   
        nodos[id] = nodo;

        if (lat < minLat) minLat = lat;
        if (lat > maxLat) maxLat = lat;
        if (lon < minLon) minLon = lon;
        if (lon > maxLon) maxLon = lon;
    }

    // Leer calles (solo highways)
    for (XMLElement* e = root->FirstChildElement("way"); e; e = e->NextSiblingElement("way")) {
        std::vector<long long> nodosWay;
        bool esCalle = false;

        for (XMLElement* n = e->FirstChildElement(); n != nullptr; n = n->NextSiblingElement()) {
            if (strcmp(n->Name(), "nd") == 0) {
                long long ref = n->Int64Attribute("ref");
                nodosWay.push_back(ref);
            }
            else if (strcmp(n->Name(), "tag") == 0) {
                const char* k = n->Attribute("k");
                if (k && std::string(k) == "highway") {
                    esCalle = true;
                }
            }
        }

        if (esCalle && nodosWay.size() > 1) {
            calles.push_back(nodosWay);
        }
    }

    return true;
}
