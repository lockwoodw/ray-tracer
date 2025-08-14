#include <cmath>
#include <vector>

#include "hemisphere.h"
#include "utils.h"

bool Hemisphere::Intersect(IntersectionList& list, const Ray& world_ray) const {
    IntersectionList local_list {};
    if (Sphere::Intersect(local_list, world_ray) && local_list.Size() == 2) {
        // local hit points on object
        std::vector<const Intersection*> hits {};
        for (auto i: local_list) {
            hits.push_back(i);
        }
        Ray ray = world_ray.Transform(inverse_transform_);
        Point origin = ray.Origin();
        Vector direction = ray.Direction();
        Point hit_0 = origin + direction * hits[0]->Distance(),
              hit_1 = origin + direction * hits[1]->Distance();

        double x0 = hit_0.X(),
               x1 = hit_1.X(),
               distance_to_zy = -origin.X() / direction.X(),
               d0 = hits[0]->Distance(),
               d1 = hits[1]->Distance();
        if (x0 > 0) {
            if (x1 > 0) {
                // ray passes through the right hemisphere only
                return false;
            }
            list.Add(d1, this);
            if (closed_) {
                // Move hit point back to intersection of ray and ZY plane
                list.Add(distance_to_zy, this);
            }
            else {
                list.Add(d1, this);
            }
        }
        else if (x1 > 0) {
            list.Add(d0, this);
            if (closed_) {
                list.Add(distance_to_zy, this);
            }
            else {
                list.Add(d0, this);
            }
        }
        else {
            // ray passes through left hemisphere only
            list.Add(d0, this);
            list.Add(d1, this);
        }
        return true;
    }
    return false;
}

bool Hemisphere::operator==(const Shape& s) const {
    const Hemisphere* other = dynamic_cast<const Hemisphere*>(&s);
    if (other == nullptr) { // Shape is not a Hemisphere?
        return false;
    }
    return Sphere::operator==(s);
}

Vector Hemisphere::LocalNormalAt(const Point &object_point) const {
    if (object_point.X() + kEpsilon > 0) {
        return Vector { 1, 0, 0 }; // Normal for ZY plane
    }
    return object_point - origin_;
}

const BoundingBox Hemisphere::BoundsOf() const {
    return BoundingBox { Point { -radius_, -radius_, -radius_ }, Point { 0, radius_, radius_ } };
}
