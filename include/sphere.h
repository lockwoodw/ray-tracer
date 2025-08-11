#ifndef RAY_TRACER_SPHERE_H
#define RAY_TRACER_SPHERE_H

#include "shape.h"

class Sphere: public Shape {
    double radius_;

    public:
        Sphere(): Shape { Point { 0, 0, 0 } }, radius_ { 1.0 } {}
        Sphere(const Point& p, double r): Shape { p }, radius_ { r } {}
        Sphere(const Sphere& s): Shape { s.origin_ }, radius_ { s.radius_ } {}

        const double Radius() { return radius_; }

        bool operator==(const Shape& s) const override;
        bool Intersect(IntersectionList& list, const Ray& ray) const override;
        Vector LocalNormalAt(const Point &object_point) const override;
        const BoundingBox BoundsOf() const override;
};

Sphere GlassySphere();

#endif