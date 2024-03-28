#include <cmath>
#include "sphere.h"

std::vector<double> Sphere::Intersections(const Ray& r) const {
    Vector sphere_to_ray = r.Origin() - origin;
    double a = Vector::DotProduct(r.Direction(), r.Direction()),
           b = 2 * Vector::DotProduct(r.Direction(), sphere_to_ray),
           c = Vector::DotProduct(sphere_to_ray, sphere_to_ray) - radius * radius,
           discriminant = b * b - 4 * a * c;
    std::vector<double> intersections;
    // Use alternate approach from www.scratchapixel.com to avoid "catastrophic cancellation"
    // License: https://creativecommons.org/licenses/by-nc-nd/4.0/
    if (discriminant == 0) {
        intersections.push_back(-0.5 * b / a);
    }
    else if (discriminant > 0) {
        double q = -0.5 * ((b > 0) ? (b + sqrt(discriminant)) : (b - sqrt(discriminant))),
               t1 = q / a,
               t2 = c / q;
        if (t1 > t2) {
            intersections.push_back(t2);
            intersections.push_back(t1);
        }
        else {
            intersections.push_back(t1);
            intersections.push_back(t2);
        }
    }
    return intersections;
}