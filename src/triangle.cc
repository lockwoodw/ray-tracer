#include <vector>
#include <cmath> // for std::fabs

#include "triangle.h"
#include "utils.h"

Triangle::Triangle(const Point &p1, const Point &p2, const Point &p3):
    Shape { Point { 0, 0, 0 } }, p1_ { p1 }, p2_ { p2 }, p3_ { p3 }
{
    bbox_ = BoundsOf();
    edge1_ = p2_ - p1_;
    edge2_ = p3_ - p1_;
    edge3_ = p3_ - p2_;
    normal_ = Vector::CrossProduct(edge2_, edge1_).Normalize();
    area_ = CalculateArea();
    centroid_ = CalculateCentroid();
}

Triangle::Triangle(const Triangle& t): Shape { t.origin_ }, p1_ { t.p1_ },
    p2_ { t.p2_ }, p3_ { t.p3_ }, edge1_ { t.edge1_ }, edge2_ { t.edge2_},
    edge3_ { t.edge3_ }, normal_ { t.normal_ }, area_ { t.area_ },
    centroid_ { t.centroid_ }
{
    bbox_ = BoundsOf();
}

bool Triangle::operator==(const Shape& s) const {
    const Triangle* other = dynamic_cast<const Triangle*>(&s);
    if (other == nullptr) { // Shape is not an Triangle?
        return false;
    }
    return p1_ == other->p1_ && p2_ == other->p2_ && p2_ == other->p2_;
}

bool Triangle::Intersect(IntersectionList& list, const Ray& world_ray) const {
    // Implement the Möller-Trumbore algorithm
    // https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
    Ray ray = world_ray.Transform(inverse_transform_);

    // Ray is parallel to triangle?
    Vector dir_cross_edge2 = Vector::CrossProduct(ray.Direction(), edge2_);
    double determinant = Vector::DotProduct(edge1_, dir_cross_edge2);
    if (std::fabs(determinant) < kEpsilon) {
        return false;
    }

    // Ray passes beyond the p1-p3 edge?
    double f { 1.0 / determinant };
    Vector p1_to_origin = ray.Origin() - p1_;
    double u = f * Vector::DotProduct(p1_to_origin, dir_cross_edge2);
    if (u < 0 || u > 1.0) {
        return false;
    }

    // Ray passes beyond the other edges?
    Vector origin_cross_edge1 = Vector::CrossProduct(p1_to_origin, edge1_);
    double v = f * Vector::DotProduct(ray.Direction(), origin_cross_edge1);
    if (v < 0 || (u + v) > 1) {
        return false;
    }

    // Ray intersects triangle
    double t = f * Vector::DotProduct(edge2_, origin_cross_edge1);
    list.Add(t, this);
    return true;
}

Vector Triangle::LocalNormalAt(const Point &object_point) const {
    return normal_;
}

const BoundingBox Triangle::BoundsOf() const {
    BoundingBox bbox {};
    bbox.Add(p1_);
    bbox.Add(p2_);
    bbox.Add(p3_);
    return bbox;
}

const Point Triangle::CalculateCentroid() const {
    return Point {
        (p1_.X() + p2_.X() + p3_.X()) / 3,
        (p1_.Y() + p2_.Y() + p3_.Y()) / 3,
        (p1_.Z() + p2_.Z() + p3_.Z()) / 3
    };
}

const double Triangle::CalculateArea() const {
    // using Heron's formula
    // area = 0.25 × √( (a + b + c) × (-a + b + c) × (a - b + c) × (a + b - c) )
    double a { edge1_.Magnitude() },
           b { edge2_.Magnitude() },
           c { edge3_.Magnitude() };
    return 0.25 * std::sqrt((a + b + c) * (-a + b + c) * (a - b + c) * (a + b - c));
}