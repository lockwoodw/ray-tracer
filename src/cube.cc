#include "cube.h"

const double Cube::kEpsilon = 1e-5;

std::array<double, 2> Cube::IntersectionsByAxis(const Ray& ray, const SpatialTuple::Coordinates axis) {
    double origin = ray.Origin().At(axis),
           direction = ray.Direction().At(axis);
    // This is similar to the calculation of intersect "t" to a plane, but there
    // are two "planes" for each axis of the cube
    double tmin_numerator = -1 - origin, // intersect to the leftmost side of the cube (if this is the X axis)
           tmax_numerator = 1 - origin;  // intersect to the rightmost side of the cube
    std::array<double, 2> t {};

    if (abs(direction) >= Cube::kEpsilon) {
        t[0] = tmin_numerator / direction;
        t[1] = tmax_numerator / direction;
    }
    else {
        // avoid divide by "zero" (or so small as to be zero) by just setting
        // the t values to +/- infinity
        double infinity = std::numeric_limits<double>::infinity();
        t[0] = tmin_numerator * infinity;
        t[1] = tmax_numerator * infinity;
    }

    if (t[0] > t[1]) {
        // swap
        double temp = t[0];
        t[0] = t[1];
        t[1] = temp;
    }
    return t;
}

void Cube::Intersect(IntersectionList& list, const Ray& ray) const {
    std::array<double, 2> xt = IntersectionsByAxis(ray, SpatialTuple::Coordinates::kX),
                          yt = IntersectionsByAxis(ray, SpatialTuple::Coordinates::kY),
                          zt = IntersectionsByAxis(ray, SpatialTuple::Coordinates::kZ);

    double tmin = std::max(std::max(xt[0], yt[0]), zt[0]), // the max of the minimums
           tmax = std::min(std::min(xt[1], yt[1]), zt[1]); // the min of the maximums

    // if tmin > tmax then the max minimum is further from the ray's origin
    // than the min maximum--a contradiction that indicates a miss
    if (tmax > tmin) {
        list.Add(tmin, this);
        list.Add(tmax, this);
    }
}

bool Cube::operator==(const Shape& s) const {
    const Cube* other = dynamic_cast<const Cube*>(&s);
    if (other == nullptr) { // Shape is not an Cube?
        return false;
    }
    return origin_ == other->origin_;
}

Vector Cube::LocalNormalAt(const Point &object_point) const {
    // The normal is the vector pointing in the direction of coordinate with
    // the largest absolute value.
    double x = object_point.X(),
           y = object_point.Y(),
           z = object_point.Z(),
           abs_x = abs(x),
           abs_y = abs(y),
           abs_z = abs(z);
    double max_coord = std::max(std::max(abs_x, abs_y), abs_z);

    if (max_coord == abs_x) {
        return Vector(x, 0, 0);
    }
    else if (max_coord == abs_y) {
        return Vector(0, y, 0);
    }
    return Vector(0, 0, z);
}