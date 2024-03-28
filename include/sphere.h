#ifndef RAY_TRACER_SPHERE_H
#define RAY_TRACER_SPHERE_H

#include <vector>
#include "shape.h"

class Sphere: public Shape {
    double radius;

    public:
        Sphere(const Point& p, double r): Shape { p }, radius { r } {}
        std::vector<double> Intersections(const Ray& r) const override;
        const double Radius() { return radius; }
};

#endif