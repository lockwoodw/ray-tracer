#include "sheet.h"

#include <cmath>

void Sheet::Intersect(IntersectionList& list, const Ray& ray) const {
    // Same calculation as for Planes, but we also confirm the intersection
    // point lies within the bounds of the Sheet
    if (std::abs(ray.Direction().Y()) >= kEpsilon) {
        double distance = -ray.Origin().Y() / ray.Direction().Y();
        Point i = ray.Origin() + ray.Direction() * distance;
        double x = i.X(),
               z = i.Z(),
               width = width_ / 2,
               depth = depth_ / 2;
        if (x >= -width && x <= width && z >= -depth && z <= depth) {
            list.Add(distance, this);
        }
    }
}

bool Sheet::operator==(const Shape& s) const {
    const Sheet* other = dynamic_cast<const Sheet*>(&s);
    if (other == nullptr) { // Shape is not an XZPlane?
        return false;
    }
    return origin_ == other->origin_ && floating_point_compare(width_, other->width_)
        && floating_point_compare(depth_, other->depth_);
}