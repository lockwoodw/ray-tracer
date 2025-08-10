#include <gtest/gtest.h>

#include "plane.h"

#include "space.h"
#include "ray.h"
#include "shape.h"

/*
Scenario: The normal of a plane is constant everywhere
  Given p ← plane()
  When n1 ← local_normal_at(p, point(0, 0, 0))
    And n2 ← local_normal_at(p, point(10, 0, -10))
    And n3 ← local_normal_at(p, point(-5, 0, 150))
  Then n1 = vector(0, 1, 0)
    And n2 = vector(0, 1, 0)
    And n3 = vector(0, 1, 0)
*/

TEST(PlaneTest, ConfirmingTheNormalOfAPlaneIsConstantEverywhere) {
    Plane p {};
    Vector expected { 0, 1, 0 },
           n1 = p.LocalNormalAt(Point { 0, 0, 0 }),
           n2 = p.LocalNormalAt(Point { 10, 0, -10 }),
           n3 = p.LocalNormalAt(Point { -5, 0, 150 });
    ASSERT_EQ(n1, expected);
    ASSERT_EQ(n2, expected);
    ASSERT_EQ(n3, expected);
}

/*
Scenario: Intersect with a ray parallel to the plane
  Given p ← plane()
    And r ← ray(point(0, 10, 0), vector(0, 0, 1))
  When xs ← local_intersect(p, r)
  Then xs is empty
*/

TEST(PlaneTest, IntersectingWithARayParallelToThePlane) {
    Plane p {};
    Ray r { Point { 0, 10, 0 }, Vector { 0, 0, 1 } };
    IntersectionList xs {};
    p.Intersect(xs, r);
    ASSERT_EQ(xs.Size(), 0);
}

/*
Scenario: Intersect with a coplanar ray
  Given p ← plane()
    And r ← ray(point(0, 0, 0), vector(0, 0, 1))
  When xs ← local_intersect(p, r)
  Then xs is empty
*/

TEST(PlaneTest, IntersectingWithACoplanarRay) {
    Plane p {};
    Ray r { Point { 0, 0, 0 }, Vector { 0, 0, 1 } };
    IntersectionList xs {};
    p.Intersect(xs, r);
    ASSERT_EQ(xs.Size(), 0);
}

/*
Scenario: A ray intersecting a plane from above
  Given p ← plane()
    And r ← ray(point(0, 1, 0), vector(0, -1, 0))
  When xs ← local_intersect(p, r)
  Then xs.count = 1
    And xs[0].t = 1
    And xs[0].object = p
*/

TEST(PlaneTest, IntersectingWithARayAboveThePlane) {
    Plane p {};
    Ray r { Point { 0, 1, 0 }, Vector { 0, -1, 0 } };
    IntersectionList xs {};
    p.Intersect(xs, r);
    ASSERT_EQ(xs.Size(), 1);
    const Intersection* i = xs[0];
    ASSERT_DOUBLE_EQ(i->Distance(), 1);
    ASSERT_EQ(p, *i->Object());
}

TEST(PlaneTest, IntersectingWithARayFurtherAboveThePlane) {
    Plane p {};
    Ray r { Point { 0, 3, -3 }, Vector { 0, -1, 1 } };
    IntersectionList xs {};
    p.Intersect(xs, r);
    ASSERT_EQ(xs.Size(), 1);
    const Intersection* i = xs[0];
    ASSERT_DOUBLE_EQ(i->Distance(), 3);
    ASSERT_EQ(p, *i->Object());
}

TEST(PlaneTest, ConfirmingTheIntersectionPoint) {
    Plane p {};
    Ray r { Point { 0, 3, -3 }, Vector { 0, -1, 1 } };
    IntersectionList xs {};
    p.Intersect(xs, r);
    ASSERT_EQ(xs.Size(), 1);
    const Intersection* i = xs[0];
    Point expected { 0, 0, 0 },
          actual = r.Origin() + r.Direction() * i->Distance();
    ASSERT_EQ(expected, actual);
}

/*
Scenario: A ray intersecting a plane from below
  Given p ← plane()
    And r ← ray(point(0, -1, 0), vector(0, 1, 0))
  When xs ← local_intersect(p, r)
  Then xs.count = 1
    And xs[0].t = 1
    And xs[0].object = p
*/

TEST(PlaneTest, IntersectingWithARayBelowThePlane) {
    Plane p {};
    Ray r { Point { 0, -1, 0 }, Vector { 0, 1, 0 } };
    IntersectionList xs {};
    p.Intersect(xs, r);
    ASSERT_EQ(xs.Size(), 1);
    const Intersection* i = xs[0];
    ASSERT_DOUBLE_EQ(i->Distance(), 1);
    ASSERT_EQ(p, *i->Object());
}

/*
Scenario: A plane has a bounding box
  Given shape ← plane()
  When box ← bounds_of(shape)
  Then box.min = point(-infinity, 0, -infinity)
    And box.max = point(infinity, 0, infinity)
*/

TEST(PlaneTest, APlaneHasABoundingBox) {
    Plane p {};
    BoundingBox box = p.BoundsOf();
    Point min { -kInfinity, 0, -kInfinity },
          max { kInfinity, 0, kInfinity };
    ASSERT_EQ(min, box.Min());
    ASSERT_EQ(max, box.Max());
}