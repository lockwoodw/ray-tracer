#ifndef RAY_TRACER_CYLINDER_H
#define RAY_TRACER_CYLINDER_H

#include <limits>
#include <cmath>

#include "shape.h"

#include "space.h"
#include "utils.h"

class Cylinder: public Shape {
    double minimum_;
    double maximum_;
    bool closed_;

    bool RayIntersectsEnd(const Ray&r, double t) const;
    bool AddEndIntersects(IntersectionList& list, const Ray& ray) const;

    public:
        Cylinder(): Shape { Point { 0, 0, 0 } },
            minimum_ { -std::numeric_limits<double>::infinity() },
            maximum_ { std::numeric_limits<double>::infinity() },
            closed_ { false } {}
        Cylinder(const Cylinder& c): Shape { c.origin_ }, minimum_ { c.minimum_ },
            maximum_ { c.maximum_ }, closed_ { c.closed_ } {}
        Cylinder(double min, double max, bool closed): Shape { Point { 0, 0, 0} },
            minimum_ { min }, maximum_ { max }, closed_ { closed } {}
        Cylinder(Point origin, double min, double max, bool closed): Shape { origin },
            minimum_ { min }, maximum_ { max }, closed_ { closed } {}

        bool operator==(const Shape& s) const;
        bool Intersect(IntersectionList& list, const Ray& ray) const override;
        Vector LocalNormalAt(const Point &object_point) const override;
        const BoundingBox BoundsOf() const override;

        void Minimum(double m) { minimum_ = m; }
        void Maximum(double m) { maximum_ = m; }
        double Minimum() const { return minimum_; }
        double Maximum() const { return maximum_; }
        void Closed(bool c) { closed_ = c; }
        bool Closed() const { return closed_; }
};

#endif