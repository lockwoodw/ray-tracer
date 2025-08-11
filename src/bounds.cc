#include "bounds.h"

const int BoundingBox::kIndices[3] { Point::Coordinates::kX, Point::Coordinates::kY,
    Point::Coordinates::kZ  };



void BoundingBox::Add(const Point& p) {
    // resize bounding box so its min/max contain the given point
    for (auto index: kIndices) {
        double coord = p.At(index);
        if (coord < min_[index]) {
            min_[index] = coord;
        }
        if (coord > max_[index]) {
            max_[index] = coord;
        }
    }
}

void BoundingBox::Add(const BoundingBox& b) {
    // resize bounding box so its min/max contain the given box
    for (auto index: kIndices) {
        double min_coord = b.min_.At(index),
               max_coord = b.max_.At(index);
        if (min_coord < min_[index]) {
            min_[index] = min_coord;
        }
        if (max_coord > max_[index]) {
            max_[index] = max_coord;
        }
    }
}

bool BoundingBox::Contains(const Point& p) {
    for (auto index: kIndices) {
        double coord = p.At(index);
        if (coord < min_[index] || coord > max_[index]) {
            return false;
        }
    }
    return true;
}

bool BoundingBox::Contains(const BoundingBox& b) {
    for (auto index: kIndices) {
        double min_coord = b.min_.At(index),
               max_coord = b.max_.At(index);
        if (min_coord < min_[index] || max_coord > max_[index]) {
            return false;
        }
    }
    return true;
}

const BoundingBox BoundingBox::Transform(const Matrix& m) const {
    // Apply given transform to each corner of the bounding box and return the
    // result
    BoundingBox transformed {};
    std::array<Point, 8> corners = {
        min_,
        { min_.X(), min_.Y(), max_.Z() },
        { min_.X(), max_.Y(), min_.Z() },
        { min_.X(), max_.Y(), max_.Z() },
        { max_.X(), min_.Y(), min_.Z() },
        { max_.X(), min_.Y(), max_.Z() },
        { max_.X(), max_.Y(), min_.Z() },
        max_
    };

    for (auto corner: corners) {
        transformed.Add(m * corner);
    }

    return transformed;
}

std::array<double, 2> BoundingBox::IntersectionsByAxis(const Ray& ray,
        const SpatialTuple::Coordinates axis) const {
    // This is essentially the same as Cube::IntersectionsByAxis(), but replaces
    // the unit offsets for the numerators with the actual the minimum and
    // maximum extents of the bounding box for the given axis

    double origin = ray.Origin().At(axis),
           direction = ray.Direction().At(axis);

    double tmin_numerator = min_.At(axis) - origin,
           tmax_numerator = max_.At(axis) - origin;

    std::array<double, 2> t {};

    if (std::fabs(direction) >= kEpsilon) {
        t[0] = tmin_numerator / direction;
        t[1] = tmax_numerator / direction;
    }
    else {
        double infinity = std::numeric_limits<double>::infinity();
        t[0] = tmin_numerator * kBBInfinity;
        t[1] = tmax_numerator * kBBInfinity;
    }

    if (t[0] > t[1]) {
        double temp = t[0];
        t[0] = t[1];
        t[1] = temp;
    }
    return t;
}

const bool BoundingBox::Intersects(const Ray& ray) const {
    // This is essentially the same as Cube::Intersects()

    std::array<double, 2> xt = IntersectionsByAxis(ray, SpatialTuple::Coordinates::kX),
                          yt = IntersectionsByAxis(ray, SpatialTuple::Coordinates::kY);

    if (xt[0] > yt[1] || yt[0] > xt[1]) {
        return false;
    }

    double tmin = std::max(xt[0], yt[0]),
           tmax = std::min(xt[1], yt[1]);

    std::array<double, 2> zt = IntersectionsByAxis(ray, SpatialTuple::Coordinates::kZ);

    if (zt[0] > tmax || zt[1] < tmin) {
        return false;
    }

    tmin = std::max(tmin, zt[0]);
    tmax = std::min(tmax, zt[1]);

    return tmax > tmin;
}