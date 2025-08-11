#include "disc.h"

bool Disc::operator==(const Shape& s) const {
    const Disc* other = dynamic_cast<const Disc*>(&s);
    if (other == nullptr) { // Shape is not a Disc?
        return false;
    }
    return origin_ == other->origin_ && floating_point_compare(radius_, other->radius_);
}

bool Disc::Intersect(IntersectionList& list, const Ray& world_ray) const {
    Ray ray = world_ray.Transform(inverse_transform_);

    // Check if ray intersects the plane that the disc lies on
    if (std::fabs(ray.Direction().Y()) >= kEpsilon) {
        // If the ray intersects the plane, check if the point of intersection
        // is within the disc's radius, measured from the origin
        Point ro = ray.Origin();
        Vector rd = ray.Direction();
        double distance = -ro.Y() / rd.Y();
        Point intersection = ro + rd * distance;

        // Use optimized method from www.scratchapixel.com to check if
        // intersection is within the radius
        // License: https://creativecommons.org/licenses/by-nc-nd/4.0/

        // Vector from center of disc to intersection
        Vector to_intersection = intersection - origin_;

        // Squared distance of same
        double squared = Vector::DotProduct(to_intersection, to_intersection);

        // Compare squared distances
        if (squared <= radius_*radius_) {
            list.Add(distance, this);
            return true;
        }
    }
    return false;
}

const BoundingBox Disc::BoundsOf() const {
    return BoundingBox { Point { -radius_, 0, -radius_ }, Point { radius_, 0, radius_ } };
}