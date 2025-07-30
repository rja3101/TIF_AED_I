#ifndef CARGADOR_OSM_H
#define CARGADOR_OSM_H

#include "Nodo.h"
#include <vector>
#include <map>

bool cargarMapa(const char* archivo,
                std::map<long long, Nodo>& nodos,
                std::vector<std::vector<long long>>& calles,
                double& minLat, double& maxLat,
                double& minLon, double& maxLon);

#endif
