#include "cylinder.h"

bool Cylinder::RayIntersectsEnd(const Ray&r, double t) const {
    // Check if the intersection at t is within the radius of the end cap (=1)
    // from the Y axis
    Point o = r.Origin();
    Vector d = r.Direction();
    double x = o.X() + t * d.X(),
           z = o.Z() + t * d.Z();
    return (x*x + z*z) <= 1.0;
}

bool Cylinder::AddEndIntersects(IntersectionList& list, const Ray& ray) const {
    Vector rd = ray.Direction();
    Point ro = ray.Origin();

    double rdy = rd.Y();

    // Confirm the cylinder is closed and that the ray could intersect with
    // the cap(s)--i.e., it is not coplanar with the XZ plane (the caps are)
    if (!closed_ || std::fabs(rdy) <= kEpsilon) {
        return false;
    }

    // Check for intersection with lower end cap by intersecting the ray
    // with the plane where y=minimum_;
    double roy = ro.Y(),
           t = (minimum_ - roy) / rdy;
    bool intersected = false;
    if (RayIntersectsEnd(ray, t)) {
        list.Add(t, this);
        intersected = true;
    }

    // Check for intersection with upper end cap
    t = (maximum_ - roy) / rdy;
    if (RayIntersectsEnd(ray, t)) {
        list.Add(t, this);
        intersected = true;
    }
    return intersected;
}

bool Cylinder::operator==(const Shape& s) const {
    const Cylinder* other = dynamic_cast<const Cylinder*>(&s);
    if (other == nullptr) { // Shape is not an Cylinder?
        return false;
    }
    return origin_ == other->origin_ && floating_point_compare(minimum_, other->minimum_)
        && floating_point_compare(maximum_, other->maximum_);
}

bool Cylinder::Intersect(IntersectionList& list, const Ray& world_ray) const {
    Ray ray = world_ray.Transform(inverse_transform_);

    Vector rd = ray.Direction();
    double rdx = rd.X(),
           rdz = rd.Z();

    double a = rdx*rdx + rdz*rdz;

    // Ray is parallel to y axis and misses cylinder
    if (std::fabs(a) < kEpsilon) {
        return AddEndIntersects(list, ray); // but might hit end caps
    }

    Point ro = ray.Origin();
    double rox = ro.X(),
           roz = ro.Z(),
           b = 2 * (rox * rdx +  roz * rdz),
           c = rox*rox + roz*roz - 1,
           discriminant = b*b - 4 * a * c;

    // Ray does not intersect cylinder
    if (discriminant < 0) {
        return false;
    }

    double disc_sqrt = std::sqrt(discriminant),
           double_a = 2 * a,
           t0 = (-b - disc_sqrt) / double_a,
           t1 = (-b + disc_sqrt) / double_a;

    if (t0 > t1) {
        // Swap
        double temp = t0;
        t0 = t1;
        t1 = temp;
    }

    // Calculate Y coordinate at intersection point
    double roy = ro.Y(),
           rdy = rd.Y(),
           y0 = roy + t0 * rdy,
           y1 = roy + t1 * rdy;

    // And compare against the min/max for the cylinder
    bool intersected = false;
    if (minimum_ < y0 && y0 < maximum_) {
        list.Add(t0, this);
        intersected = true;
    }

    if (minimum_ < y1 && y1 < maximum_) {
        list.Add(t1, this);
        intersected = true;
    }

    bool intersected_ends = AddEndIntersects(list, ray);
    return intersected || intersected_ends;
}

Vector Cylinder::LocalNormalAt(const Point &object_point) const {
    double px = object_point.X(),
    pz = object_point.Z(),
    py = object_point.Y(),
    // the square of the distance from the y axis:
    dist = px*px + pz*pz;

    // Determine if the point is on an end cap, and if so, which one:
    if (dist < 1 && py >= maximum_ - kEpsilon) {
        return Vector { 0, 1, 0 };
    }

    if (dist < 1 && py <= minimum_ + kEpsilon) {
        return Vector { 0, -1, 0 };
    }

    // Otherwise, take the normal at the point on the cylinder body…
    // To calculate the normal of the point on the cylinder body,
    // zero-out its Y component
    return Vector { px, 0, pz };
}

const BoundingBox Cylinder::BoundsOf() const {
    if (closed_) {
      return BoundingBox { Point { -1, minimum_, -1 }, Point { 1, maximum_, 1 } };
    }
    return BoundingBox { Point { -1, -kInfinity, -1 }, Point { 1, kInfinity, 1 } };
}