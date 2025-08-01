#ifndef CARGADOR_OSM_H
#define CARGADOR_OSM_H

#include "Nodo.h"
#include"MyVector.h"
#include <map>

bool cargarMapa(const char* archivo,
                std::map<long long, Nodo>& nodos,
                MyVector<MyVector<long long>>& calles,
                double& minLat, double& maxLat,
                double& minLon, double& maxLon);

#endif
