#include <gtest/gtest.h>

#include "triangle.h"
#include "ray.h"

/*
Scenario: Constructing a triangle
  Given p1 ← point(0, 1, 0)
    And p2 ← point(-1, 0, 0)
    And p3 ← point(1, 0, 0)
    And t ← triangle(p1, p2, p3)
  Then t.p1 = p1
    And t.p2 = p2
    And t.p3 = p3
    And t.e1 = vector(-1, -1, 0)
    And t.e2 = vector(1, -1, 0)
    And t.normal = vector(0, 0, -1)
*/

TEST(TriangleTest, ConstructingATriangle) {
    Point p1 { 0, 1, 0 }, p2 { -1, 0, 0 }, p3 { 1, 0, 0 };
    Triangle t { p1, p2, p3 };
    ASSERT_EQ(p1, t.P1());
}

/*
Scenario: Intersecting a ray parallel to the triangle
  Given t ← triangle(point(0, 1, 0), point(-1, 0, 0), point(1, 0, 0))
    And r ← ray(point(0, -1, -2), vector(0, 1, 0))
  When xs ← local_intersect(t, r)
  Then xs is empty
*/

TEST(TriangleTest, IntersectingARayParallelToTheTriangle) {
    Point p1 { 0, 1, 0 }, p2 { -1, 0, 0 }, p3 { 1, 0, 0 };
    Triangle t { p1, p2, p3 };
    Ray ray { Point(0, -1, -2), Vector(0, 1, 0) };
    IntersectionList xs {};
    t.Intersect(xs, ray);
    ASSERT_EQ(xs.Size(), 0);
}

/*
Scenario: A ray misses the p1-p3 edge
  Given t ← triangle(point(0, 1, 0), point(-1, 0, 0), point(1, 0, 0))
    And r ← ray(point(1, 1, -2), vector(0, 0, 1))
  When xs ← local_intersect(t, r)
  Then xs is empty
*/

TEST(TriangleTest, RayMissesTheP1P3Edge) {
    Point p1 { 0, 1, 0 }, p2 { -1, 0, 0 }, p3 { 1, 0, 0 };
    Triangle t { p1, p2, p3 };
    Ray ray { Point(1, 1, -2), Vector(0, 0, 1) };
    IntersectionList xs {};
    t.Intersect(xs, ray);
    ASSERT_EQ(xs.Size(), 0);
}

/*
Scenario: A ray misses the p1-p2 edge
  Given t ← triangle(point(0, 1, 0), point(-1, 0, 0), point(1, 0, 0))
    And r ← ray(point(-1, 1, -2), vector(0, 0, 1))
  When xs ← local_intersect(t, r)
  Then xs is empty
*/

TEST(TriangleTest, RayMissesTheP1P2Edge) {
    Point p1 { 0, 1, 0 }, p2 { -1, 0, 0 }, p3 { 1, 0, 0 };
    Triangle t { p1, p2, p3 };
    Ray ray { Point(-1, 1, -2), Vector(0, 0, 1) };
    IntersectionList xs {};
    t.Intersect(xs, ray);
    ASSERT_EQ(xs.Size(), 0);
}

/*
Scenario: A ray misses the p2-p3 edge
  Given t ← triangle(point(0, 1, 0), point(-1, 0, 0), point(1, 0, 0))
    And r ← ray(point(0, -1, -2), vector(0, 0, 1))
  When xs ← local_intersect(t, r)
  Then xs is empty
*/

TEST(TriangleTest, RayMissesTheP2P3Edge) {
    Point p1 { 0, 1, 0 }, p2 { -1, 0, 0 }, p3 { 1, 0, 0 };
    Triangle t { p1, p2, p3 };
    Ray ray { Point(0, -1, -2), Vector(0, 0, 1) };
    IntersectionList xs {};
    t.Intersect(xs, ray);
    ASSERT_EQ(xs.Size(), 0);
}

/*
Scenario: A ray strikes a triangle
  Given t ← triangle(point(0, 1, 0), point(-1, 0, 0), point(1, 0, 0))
    And r ← ray(point(0, 0.5, -2), vector(0, 0, 1))
  When xs ← local_intersect(t, r)
  Then xs.count = 1
    And xs[0].t = 2
*/

TEST(TriangleTest, RayStrikesATriangle) {
    Point p1 { 0, 1, 0 }, p2 { -1, 0, 0 }, p3 { 1, 0, 0 };
    Triangle t { p1, p2, p3 };
    Ray ray { Point(0, 0.5, -2), Vector(0, 0, 1) };
    IntersectionList xs {};
    t.Intersect(xs, ray);
    ASSERT_EQ(xs.Size(), 1);
    ASSERT_EQ(xs.Hit()->Distance(), 2);
}

/*
Scenario: Finding the normal on a triangle
  Given t ← triangle(point(0, 1, 0), point(-1, 0, 0), point(1, 0, 0))
  When n1 ← local_normal_at(t, point(0, 0.5, 0))
    And n2 ← local_normal_at(t, point(-0.5, 0.75, 0))
    And n3 ← local_normal_at(t, point(0.5, 0.25, 0))
  Then n1 = t.normal
    And n2 = t.normal
    And n3 = t.normal
*/

TEST(TriangleTest, FindingTheNormalOnATriangle) {
    Point p1 { 0, 1, 0 }, p2 { -1, 0, 0 }, p3 { 1, 0, 0 };
    Triangle t { p1, p2, p3 };
    Vector n1 = t.LocalNormalAt(Point(0, 0.5, 0)),
           n2 = t.LocalNormalAt(Point(-0.5, 0.75, 0)),
           n3 = t.LocalNormalAt(Point(0.5, 0.25, 0));
    ASSERT_EQ(n1, t.Normal());
    ASSERT_EQ(n2, t.Normal());
    ASSERT_EQ(n3, t.Normal());
}

// Test Centroid method
TEST(TriangleTest, FindingTheCentroid) {
    Point p1 { 1, 1, 0 }, p2 { 3, 4, 0 }, p3 { 4, 5, 0 },
          expected { 8 / 3.0, 10 / 3.0, 0 };
    Triangle t { p1, p2, p3 };
    ASSERT_EQ(t.Centroid(), expected);
}
