#ifndef RAY_TRACER_PLANE_H
#define RAY_TRACER_PLANE_H

#include "shape.h"

#include "space.h"

class Plane: public Shape {
    public:
        Plane(): Shape { Point { 0, 0, 0 } } {}
        Plane(const Plane& p): Shape { p.origin_ } {}

        bool operator==(const Shape& s) const;
        bool Intersect(IntersectionList& list, const Ray& ray) const override;
        Vector LocalNormalAt(const Point &object_point) const override {
            // Normal for x-z plane
            return Vector { 0, 1, 0 };
        }
        const BoundingBox BoundsOf() const override;
};

#endif