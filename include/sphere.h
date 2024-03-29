#ifndef RAY_TRACER_SPHERE_H
#define RAY_TRACER_SPHERE_H

#include <vector>
#include "shape.h"
#include "utils.h"

class Sphere: public Shape {
    double radius;

    public:
        Sphere(const Point& p, double r): Shape { p }, radius { r } {}
        std::vector<double> Intersections(const Ray& r) const;
        const double Radius() { return radius; }
        bool operator==(const Shape& s) const {
            const Sphere* other = dynamic_cast<const Sphere*>(&s);
            if (other == nullptr) { // Shape is not a Sphere?
                return false;
            }
            return origin == other->origin && floating_point_compare(radius, other->radius);
        }
};

#endif