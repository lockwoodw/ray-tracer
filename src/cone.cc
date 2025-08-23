#include "cone.h"

bool Cone::RayIntersectsEnd(const Ray&r, double t, double y) const {
    // Check if the intersection at t is within the radius of the end cap,
    // which for cones is the absolute value of the y value that defines the
    // plane of the cap
    Point o = r.Origin();
    Vector d = r.Direction();
    double x = o.X() + t * d.X(),
           z = o.Z() + t * d.Z();
    return (x*x + z*z) <= (y*y);// std::fabs(y);
}

bool Cone::AddEndIntersects(IntersectionList& list, const Ray& ray) const {
    Vector rd = ray.Direction();
    Point ro = ray.Origin();

    double rdy = rd.Y();

    // Confirm the cone is closed and that the ray could intersect with
    // the cap(s)--i.e., it is not coplanar with the XZ plane (the caps are)
    if (!closed_ || std::fabs(rdy) <= kEpsilon) {
        return false;
    }

    // Check for intersection with lower end cap by intersecting the ray
    // with the plane where y=minimum_;
    double roy = ro.Y(),
           t = (minimum_ - roy) / rdy;
    bool intersected = false;
    if (RayIntersectsEnd(ray, t, minimum_)) {
        list.Add(t, this);
        intersected = true;
    }

    // Check for intersection with upper end cap
    t = (maximum_ - roy) / rdy;
    if (RayIntersectsEnd(ray, t, maximum_)) {
        list.Add(t, this);
        intersected = true;
    }
    return intersected;
}

bool Cone::operator==(const Shape& s) const {
    const Cone* other = dynamic_cast<const Cone*>(&s);
    if (other == nullptr) { // Shape is not an Cone?
        return false;
    }
    return origin_ == other->origin_ && floating_point_compare(minimum_, other->minimum_)
        && floating_point_compare(maximum_, other->maximum_);
}

bool Cone::Intersect(IntersectionList& list, const Ray& world_ray) const {
    Ray ray = world_ray.Transform(inverse_transform_);

    Vector rd = ray.Direction();
    Point ro = ray.Origin();
    double rdx = rd.X(),
           rdy = rd.Y(),
           rdz = rd.Z(),
           rox = ro.X(),
           roy = ro.Y(),
           roz = ro.Z(),
           a = rdx*rdx - rdy*rdy + rdz*rdz,
           b = 2 * (rox * rdx - roy * rdy + roz * rdz),
           c = rox*rox - roy*roy + roz*roz;
    bool intersected = false;

    // Ray is parallel to cone's halves
    // Note that a smaller-than-usual epsilon seems to be required for this test;
    // larger epsilons generate spurious hits
    double epsilon { 1e-7 };
    if (std::fabs(a) < epsilon) {
        if (std::fabs(b) < epsilon) {
            return false;
        }
        // single point of intersection on one half
        list.Add(-c / (2*b), this);
        intersected = true;
    }
    else {
        // The remainder of the analysis is the same as for Cylinders…
        double discriminant = b*b - 4 * a * c;

        // Ray does not intersect Cone
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
        double y0 = roy + t0 * rdy,
               y1 = roy + t1 * rdy;

        // And compare against the min/max for the Cone
        if (minimum_ < y0 && y0 < maximum_) {
            list.Add(t0, this);
            intersected = true;
        }

        if (minimum_ < y1 && y1 < maximum_) {
            list.Add(t1, this);
            intersected = true;
        }
    }

    bool intersected_ends = AddEndIntersects(list, ray);
    return intersected || intersected_ends;
}

Vector Cone::LocalNormalAt(const Point &object_point) const {
    double px = object_point.X(),
    pz = object_point.Z(),
    py = object_point.Y(),
    // the square of the distance from the y axis:
    dist = px*px + pz*pz;

    // Determine if the point is on an end cap, and if so, which one:
    if (dist < (maximum_*maximum_) && py >= maximum_ - kEpsilon) {
        return Vector { 0, 1, 0 };
    }

    if (dist < (minimum_*minimum_) && py <= minimum_ + kEpsilon) {
        return Vector { 0, -1, 0 };
    }

    // Otherwise, take the normal at the point on the Cone body…
    double y = std::sqrt(dist);
    if (object_point.Y() > 0) {
        y = -y;
    }
    return Vector { px, y, pz };
}

const BoundingBox Cone::BoundsOf() const {
    if (closed_) {
        double limit = std::max(std::fabs(minimum_), std::fabs(maximum_));
        return BoundingBox { Point { -limit, minimum_, -limit },
            Point { limit, maximum_, limit } };
    }
    // Open cones extend to infinity in all dimensions
    return BoundingBox { Point { -kInfinity, -kInfinity, -kInfinity },
        Point { kInfinity, kInfinity, kInfinity } };
}