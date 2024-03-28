#ifndef RAY_TRACER_SHAPE_H
#define RAY_TRACER_SHAPE_H

#include <vector>
#include "space.h"
#include "ray.h"

class Shape {
    protected:
        Point origin;

    public:
        Shape(const Point& p): origin { p } {}
        ~Shape() {}
        virtual std::vector<double> Intersections(const Ray& r) const = 0;
        const Point Origin() const { return origin; }
};

#endif