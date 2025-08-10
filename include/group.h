#ifndef RAY_TRACER_GROUP_H
#define RAY_TRACER_GROUP_H

#include <vector>

#include "shape.h"

#include "space.h"

class ShapeGroup: public Shape {
    std::vector<Shape*> shapes_;

    public:
        ShapeGroup(): Shape { Point { 0, 0, 0 } }, shapes_ {} {}

        const bool IsEmpty() const { return shapes_.size() == 0; }
        void AddShape(Shape* s);
        ShapeGroup& operator<<(Shape *s);
        bool Contains(Shape* s) const;

        bool operator==(const Shape& s) const;
        void Intersect(IntersectionList& list, const Ray& ray) const override;
        Vector LocalNormalAt(const Point &object_point) const override;
        const BoundingBox BoundsOf() const override;
};

#endif