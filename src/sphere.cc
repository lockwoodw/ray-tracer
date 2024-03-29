#include <cmath>
#include "sphere.h"
#include "utils.h"

IntersectionList Sphere::Intersections(const Ray& r) const {
    IntersectionList list {};

    Vector sphere_to_ray = r.Origin() - origin_;
    double a = Vector::DotProduct(r.Direction(), r.Direction()),
           b = 2 * Vector::DotProduct(r.Direction(), sphere_to_ray),
           c = Vector::DotProduct(sphere_to_ray, sphere_to_ray) - radius_ * radius_,
           discriminant = b * b - 4 * a * c;

    if (discriminant < 0) {
        return list;
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

    Intersection i1 { t1, *this }, i2 { t2, *this };
    list.Add(i1);
    list.Add(i2);
    return list;
}

bool Sphere::operator==(const Shape& s) const {
    const Sphere* other = dynamic_cast<const Sphere*>(&s);
    if (other == nullptr) { // Shape is not a Sphere?
        return false;
    }
    return origin_ == other->origin_ && floating_point_compare(radius_, other->radius_);
}