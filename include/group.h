#ifndef RAY_TRACER_GROUP_H
#define RAY_TRACER_GROUP_H

#include <vector>

#include "shape.h"

#include "space.h"

class ShapeGroup: public Shape {
    std::vector<Shape*> shapes_;
    std::vector<Shape*> subgroups_;

    public:
        ShapeGroup(): Shape { Point { 0, 0, 0 } }, shapes_ {}, subgroups_ {} {}
        ~ShapeGroup() {
            for (auto s: subgroups_) {
                delete s;
            }
        }

        const bool IsEmpty() const { return shapes_.size() == 0; }
        void Add(Shape* s);
        ShapeGroup& operator<<(Shape *s);
        bool Contains(Shape* s) const;
        const std::array<std::vector<Shape *>, 2> Partition();
        void AddSubgroup(std::vector<Shape *> shapes);
        const size_t Size() const { return shapes_.size(); }
        Shape* operator[](int i);

        bool operator==(const Shape& s) const;
        bool Intersect(IntersectionList& list, const Ray& ray) const override;
        Vector LocalNormalAt(const Point &object_point) const override;
        const BoundingBox BoundsOf() const override;
        void Divide(int) override;
};

#endif