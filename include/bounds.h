#ifndef RAY_TRACER_BOUNDS_H
#define RAY_TRACER_BOUNDS_H

#include <limits>
#include <array>

#include "space.h"
#include "matrix.h"

const static double kBBInfinity { std::numeric_limits<double>::infinity() };

class BoundingBox {
    Point min_;
    Point max_;

    public:
        static const int kIndices[3];

        BoundingBox():  min_ { Point { kBBInfinity, kBBInfinity, kBBInfinity } },
            max_ { Point { -kBBInfinity, -kBBInfinity, -kBBInfinity } } {}
        BoundingBox(const Point& min, const Point& max): min_ { min },
            max_ { max } {}

        const Point Min() const { return min_; }
        const Point Max() const { return max_; }
        void Add(const Point& p);
        void Add(const BoundingBox& b);
        bool Contains(const Point& p);
        bool Contains(const BoundingBox& b);
        const BoundingBox Transform(const Matrix& m) const;
};

#endif