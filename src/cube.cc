#include "cube.h"

std::array<double, 2> Cube::IntersectionsByAxis(const Ray& ray, const SpatialTuple::Coordinates axis) {
    double origin = ray.Origin().At(axis),
           direction = ray.Direction().At(axis);

    // This is similar to the calculation of intersect "t" to a plane, but there
    // are two "planes" for each axis of the cube
    double tmin_numerator = -1 - origin, // intersect to the leftmost side of the cube (if this is the X axis)
           tmax_numerator = 1 - origin;  // intersect to the rightmost side of the cube

    std::array<double, 2> t {};

    if (std::fabs(direction) >= kEpsilon) {
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
        // the algorithm requires t[0] < t[1]; depending on the ray's direction,
        // we may need to swap the t values
        double temp = t[0];
        t[0] = t[1];
        t[1] = temp;
    }
    return t;
}

bool Cube::Intersect(IntersectionList& list, const Ray& world_ray) const {
    Ray ray = world_ray.Transform(inverse_transform_);

    std::array<double, 2> xt = IntersectionsByAxis(ray, SpatialTuple::Coordinates::kX),
                          yt = IntersectionsByAxis(ray, SpatialTuple::Coordinates::kY);

    if (xt[0] > yt[1] || yt[0] > xt[1]) {
        // These cases are misses: the ray intersects the planes defined by the
        // X and Y axes but does not intersect the box
        return false;
    }

    double tmin = std::max(xt[0], yt[0]), // the max of the minimums
           tmax = std::min(xt[1], yt[1]); // the min of the maximums

    std::array<double, 2> zt = IntersectionsByAxis(ray, SpatialTuple::Coordinates::kZ);

    if (zt[0] > tmax || zt[1] < tmin) {
        // Also misses
        return false;
    }

    tmin = std::max(tmin, zt[0]);
    tmax = std::min(tmax, zt[1]);

    // if tmin > tmax then the max minimum is further from the ray's origin
    // than the min maximum--a contradiction that indicates a miss
    if (tmax > tmin) {
        list.Add(tmin, this);
        list.Add(tmax, this);
        return true;
    }
    return false;
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
           abs_x = std::fabs(x),
           abs_y = std::fabs(y),
           abs_z = std::fabs(z);
    double max_coord = std::max(std::max(abs_x, abs_y), abs_z);

    if (max_coord == abs_x) {
        return Vector(x, 0, 0);
    }
    else if (max_coord == abs_y) {
        return Vector(0, y, 0);
    }
    return Vector(0, 0, z);
}

const BoundingBox Cube::BoundsOf() const {
    return BoundingBox { Point { -1, -1, -1 }, Point { 1, 1, 1 } };
}
