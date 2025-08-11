#include "sheet.h"

#include <cmath>

bool Sheet::Intersect(IntersectionList& list, const Ray& world_ray) const {
    Ray ray = world_ray.Transform(inverse_transform_);

    // Same calculation as for Planes, but we also confirm the intersection
    // point lies within the bounds of the Sheet
    if (std::abs(ray.Direction().Y()) >= kEpsilon) {
        Point ro = ray.Origin();
        Vector rd = ray.Direction();
        double distance = -ro.Y() / rd.Y();
        Point intersection = ro + rd * distance;
        double x = intersection.X(),
               z = intersection.Z(),
               width = width_ / 2,
               depth = depth_ / 2;
        if (x >= -width && x <= width && z >= -depth && z <= depth) {
            list.Add(distance, this);
            return true;
        }
    }
    return false;
}

bool Sheet::operator==(const Shape& s) const {
    const Sheet* other = dynamic_cast<const Sheet*>(&s);
    if (other == nullptr) { // Shape is not an Sheet?
        return false;
    }
    return origin_ == other->origin_ && floating_point_compare(width_, other->width_)
        && floating_point_compare(depth_, other->depth_);
}

const BoundingBox Sheet::BoundsOf() const {
    double width = width_ / 2,
           depth = depth_ / 2;
    return BoundingBox { Point { -width, 0, -depth }, Point { width, 0, depth } };
}