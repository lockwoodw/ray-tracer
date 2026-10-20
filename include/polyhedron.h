#ifndef RAY_TRACER_POLYHEDRON_H
#define RAY_TRACER_POLYHEDRON_H

#include "group.h"

using Polyhedron = ShapeGroup*;

class PolyhedronFactory {
    std::vector<Polyhedron> objects_;

    public:
        PolyhedronFactory(): objects_ {} {}

        ~PolyhedronFactory();

        Polyhedron Tetrahedron();
        Polyhedron Pyramid();
        Polyhedron Octahedron();
        Polyhedron Pentagon();
        Polyhedron Dodecahedron();
};

#endif