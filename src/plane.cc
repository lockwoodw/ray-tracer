#include "plane.h"

#include <cmath>

bool Plane::Intersect(IntersectionList& list, const Ray& world_ray) const {
    Ray ray = world_ray.Transform(inverse_transform_);

    if (std::fabs(ray.Direction().Y()) >= kEpsilon) {
        // For explanation of this calculation for t,
        // see https://www.scratchapixel.com/lessons/3d-basic-rendering/…
        //   minimal-ray-tracer-rendering-simple-shapes/…
        //   ray-plane-and-ray-disk-intersection.html
        double distance = -ray.Origin().Y() / ray.Direction().Y();
        list.Add(distance, this);
        return true;
    }
    return false;
}

bool Plane::operator==(const Shape& s) const {
    const Plane* other = dynamic_cast<const Plane*>(&s);
    if (other == nullptr) { // Shape is not an Plane?
        return false;
    }
    return origin_ == other->origin_;
}

const BoundingBox Plane::BoundsOf() const {
    return BoundingBox { Point { -kInfinity, 0, -kInfinity }, Point { kInfinity, 0, kInfinity } };
}