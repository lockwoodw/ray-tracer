#ifndef RAY_TRACER_DISC_H
#define RAY_TRACER_DISC_H

#include "shape.h"
#include "utils.h"

class Disc: public Shape {
    double radius_;

    public:
        Disc(): Shape { Point { 0, 0, 0 } }, radius_ { 1.0 } { bbox_ = BoundsOf(); }
        Disc(const Point& p, double r): Shape { p }, radius_ { r } { bbox_ = BoundsOf(); }
        Disc(const Disc& s): Shape { s.origin_ }, radius_ { s.radius_ } { bbox_ = BoundsOf(); }

        const double Radius() { return radius_; }

        bool operator==(const Shape& s) const override;
        bool Intersect(IntersectionList& list, const Ray& ray) const override;
        Vector LocalNormalAt(const Point &object_point) const override {
            // Normal for x-z plane
            return Vector { 0, 1, 0 };
        }
        const BoundingBox BoundsOf() const override;
        void Divide(int) override { /* do nothing: shape primitives are not divisible */ }
};

#endif