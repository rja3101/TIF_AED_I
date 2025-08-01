#ifndef UTILIDADES_H
#define UTILIDADES_H

#include "Nodo.h"
#include<cmath>
// Versión original, pasando el Nodo completo
inline void convertirCoordenadas(Nodo& n, int w, int h,
                                 double minLat, double maxLat,
                                 double minLon, double maxLon) {
    n.x = static_cast<float>((n.lon - minLon) / (maxLon - minLon) * w);
    n.y = static_cast<float>((1.0 - (n.lat - minLat) / (maxLat - minLat)) * h);
}
// Función heurística reutilizable (inline para evitar redefiniciones)
inline double heuristica(Nodo& a, Nodo& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

#endif
