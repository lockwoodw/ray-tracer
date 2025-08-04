#ifndef RAY_TRACER_CUBE_H
#define RAY_TRACER_CUBE_H

#include <algorithm> // for max()
#include <array>
#include <limits>    // for infinity
#include <cmath>     // for fabs()
#include "shape.h"
#include "space.h"

class Cube: public Shape {
    static std::array<double, 2> IntersectionsByAxis(const Ray& ray,
        const SpatialTuple::Coordinates axis);

    public:
        static const double kEpsilon;

        Cube(): Shape { Point { 0, 0, 0 } } {}
        Cube(const Cube& p): Shape { p.origin_ } {}

        bool operator==(const Shape& s) const;
        void Intersect(IntersectionList& list, const Ray& ray) const override;
        Vector LocalNormalAt(const Point &object_point) const override;
};

#endif