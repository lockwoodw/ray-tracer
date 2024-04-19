#include <cmath>
#include "sphere.h"
#include "utils.h"

void Sphere::Intersect(IntersectionList& list, const Ray& ray, const Ray& local_ray) const {
    Vector sphere_to_ray = local_ray.Origin() - origin_,
           direction = local_ray.Direction();
    double a = Vector::DotProduct(direction, direction),
           b = 2 * Vector::DotProduct(direction, sphere_to_ray),
           c = Vector::DotProduct(sphere_to_ray, sphere_to_ray) - radius_ * radius_,
           discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return;
    }

    // Use alternate approach from www.scratchapixel.com to avoid "catastrophic cancellation"
    // License: https://creativecommons.org/licenses/by-nc-nd/4.0/
    double t1 {}, t2 {};
    if (discriminant == 0) {
        // Add duplicate to "help later when determining object overlaps"
        t1 = t2 = -0.5 * b / a;
    }
    else if (discriminant > 0) {
        double q = -0.5 * ((b > 0) ? (b + sqrt(discriminant)) : (b - sqrt(discriminant)));
        t1 = q / a;
        t2 = c / q;
    }

    list.Add(t1, this);
    list.Add(t2, this);
}

bool Sphere::operator==(const Shape& s) const {
    const Sphere* other = dynamic_cast<const Sphere*>(&s);
    if (other == nullptr) { // Shape is not a Sphere?
        return false;
    }
    return origin_ == other->origin_ && floating_point_compare(radius_, other->radius_);
}

Vector Sphere::LocalNormalAt(const Point &object_point) const {
    return object_point - origin_;
}