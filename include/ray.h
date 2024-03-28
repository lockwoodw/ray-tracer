#ifndef RAY_TRACER_RAY_H
#define RAY_TRACER_RAY_H

#include "space.h"
#include "matrix.h"

class Ray {
    Point origin;
    Vector direction;

    public:
        Ray(const Point& p, const Vector& v): origin { p }, direction { v } {}
        const Point Origin() const {  return origin; }
        const Vector Direction() const { return direction; }
        const Point Position(double t) const {
            return origin + direction * t;
        }
        const Ray Transform(const Matrix& transform) const {
            Point p = transform * origin;
            Vector v = transform * direction;
            return Ray { p, v };
        }
};

#endif