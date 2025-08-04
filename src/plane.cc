#include "plane.h"

#include <cmath>

const double Plane::kEpsilon = 1e-5;

void Plane::Intersect(IntersectionList& list, const Ray& ray) const {
    if (std::abs(ray.Direction().Y()) >= Plane::kEpsilon) {
        // For explanation of this calculation for t,
        // see https://www.scratchapixel.com/lessons/3d-basic-rendering/…
        //   minimal-ray-tracer-rendering-simple-shapes/…
        //   ray-plane-and-ray-disk-intersection.html
        double distance = -ray.Origin().Y() / ray.Direction().Y();
        list.Add(distance, this);
    }
}

bool Plane::operator==(const Shape& s) const {
    const Plane* other = dynamic_cast<const Plane*>(&s);
    if (other == nullptr) { // Shape is not an XZPlane?
        return false;
    }
    return origin_ == other->origin_;
}