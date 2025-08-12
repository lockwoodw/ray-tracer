#ifndef RAY_TRACER_SHEET_H
#define RAY_TRACER_SHEET_H

#include "shape.h"

#include "space.h"
#include "utils.h"

class Sheet: public Shape {
    // A finite plane

    double width_; // the extent of the sheet in x
    double depth_; // the extent of the sheet in z

    public:
        Sheet(): Shape { Point { 0, 0, 0 } }, width_ { 1.0 }, depth_ { 1.0 } { bbox_ = BoundsOf(); }
        Sheet(const Sheet& s): Shape { s.origin_ }, width_ { s.width_ },
            depth_ { s.depth_ } { bbox_ = BoundsOf(); }

        bool operator==(const Shape& s) const;
        bool Intersect(IntersectionList& list, const Ray& ray) const override;
        Vector LocalNormalAt(const Point &object_point) const override {
            // Normal for x-z plane
            return Vector { 0, 1, 0 };
        }
        const BoundingBox BoundsOf() const override;
        void Divide(int) override { /* do nothing: shape primitives are not divisible */ }
};

#endif