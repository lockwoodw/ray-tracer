#include "porous-sheet.h"

bool PorousSheet::Intersect(IntersectionList& list, const Ray& world_ray) const {
    // Same calculation as for Sheets, but evaluate the intersection point using
    // the pass function
    Ray ray = world_ray.Transform(inverse_transform_);
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
            if (pass_fn_(intersection)) {
                list.Add(distance, this);
                return true;
            }
        }
    }
    return false;
}

bool PorousSheet::operator==(const Shape& s) const {
    const PorousSheet* other = dynamic_cast<const PorousSheet*>(&s);
    if (other == nullptr) { // Shape is not an PorousSheet?
        return false;
    }
    return origin_ == other->origin_ && floating_point_compare(width_, other->width_)
        && floating_point_compare(depth_, other->depth_)
        && pass_fn_ == other->pass_fn_;
}
