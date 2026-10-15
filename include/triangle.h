#ifndef RAY_TRACER_TRIANGLE_H
#define RAY_TRACER_TRIANGLE_H

#include "shape.h"

class Triangle: public Shape {
    Point p1_, p2_, p3_;
    Vector edge1_, edge2_, normal_;

    public:
        Triangle(const Point &p1, const Point &p2, const Point &p3);
        Triangle(const Triangle& t);

        const Point P1() const { return p1_; }
        const Point P2() const { return p2_; }
        const Point P3() const { return p3_; }
        const Vector Edge1() const { return edge1_; }
        const Vector Edge2() const { return edge2_; }
        const Vector Normal() const { return normal_; }

        const Point Centroid() const;

        bool operator==(const Shape& s) const;
        bool Intersect(IntersectionList& list, const Ray& ray) const override;
        Vector LocalNormalAt(const Point &object_point) const override;
        const BoundingBox BoundsOf() const override;
        void Divide(int) override { /* do nothing: shape primitives are not divisible */ }
};

#endif