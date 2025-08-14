#ifndef RAY_TRACER_HEMISPHERE_H
#define RAY_TRACER_HEMISPHERE_H

#include "sphere.h"

class Hemisphere: public Sphere {
    bool closed_;

    public:
        Hemisphere(): Sphere {}, closed_ { true } {}
        Hemisphere(const Point& p, double r): Sphere { p, r }, closed_ { true } {}
        Hemisphere(const Hemisphere& s): Sphere { s }, closed_ { true } {}

        bool operator==(const Shape& s) const override;
        bool Intersect(IntersectionList& list, const Ray& ray) const override;
        Vector LocalNormalAt(const Point &object_point) const override;
        const BoundingBox BoundsOf() const override;
        void Divide(int) override { /* do nothing: shape primitives are not divisible */ }

        void Closed(bool closed) { closed_ = closed; }
        bool Closed() const { return closed_; }
};

#endif