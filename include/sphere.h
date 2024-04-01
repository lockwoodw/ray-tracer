#ifndef RAY_TRACER_SPHERE_H
#define RAY_TRACER_SPHERE_H

#include "shape.h"

class Sphere: public Shape {
    double radius_;

    public:
        Sphere(): Shape { Point { 0, 0, 0 } }, radius_ { 1.0 } {}
        Sphere(const Point& p, double r): Shape { p }, radius_ { r } {}
        void AddIntersections(IntersectionList& i, const Ray& r) const override;
        const double Radius() { return radius_; }
        bool operator==(const Shape& s) const override;
        Vector NormalAt(const Point &world_point) const override;
};

#endif