#include <gtest/gtest.h>
#include "ray.h"
#include "transformations.h"

/*
Scenario: Creating and querying a ray
  Given origin ← point(1, 2, 3)
    And direction ← vector(4, 5, 6)
  When r ← ray(origin, direction)
  Then r.origin = origin
    And r.direction = direction
*/

TEST(RayTest, CreatingARay) {
    Point origin { 1, 2, 3 };
    Vector direction { 4, 5, 6 };
    Ray r { origin, direction };
    ASSERT_EQ(r.Origin(), origin);
    ASSERT_EQ(r.Direction(), direction);
}

/*
Scenario: Computing a point from a distance
  Given r ← ray(point(2, 3, 4), vector(1, 0, 0))
  Then position(r, 0) = point(2, 3, 4)
    And position(r, 1) = point(3, 3, 4)
    And position(r, -1) = point(1, 3, 4)
    And position(r, 2.5) = point(4.5, 3, 4)
*/

TEST(RayTest, ComputingAPointFromADistance) {
    Point origin { 2, 3, 4 };
    Vector direction { 1, 0, 0 };
    Ray r { origin, direction };
    ASSERT_EQ(r.Position(0), origin);
    Point p1 { 3, 3, 4 };
    ASSERT_EQ(r.Position(1), p1);
    Point p2 { 1, 3, 4 };
    ASSERT_EQ(r.Position(-1), p2);
    Point p3 { 4.5, 3, 4 };
    ASSERT_EQ(r.Position(2.5), p3);
}

/*
Scenario: Translating a ray
  Given r ← ray(point(1, 2, 3), vector(0, 1, 0))
    And m ← translation(3, 4, 5)
  When r2 ← transform(r, m)
  Then r2.origin = point(4, 6, 8)
    And r2.direction = vector(0, 1, 0)
*/

TEST(RayTest, TranslatingARay) {
    Point origin { 1, 2, 3 };
    Vector direction { 0, 1, 0 };
    Ray r { origin, direction };
    double translation[] { 3, 4, 5 };
    Translation transform { 3, translation };
    Ray r2 = r.Transform(transform);
    Point p2 { 4, 6, 8 };
    ASSERT_EQ(r2.Origin(), p2);
    ASSERT_EQ(r2.Direction(), direction);
}

TEST(RayTest, TranslatingARayUsingFluentAPI) {
    Point origin { 1, 2, 3 };
    Vector direction { 0, 1, 0 };
    Ray r { origin, direction };
    Matrix transform = Transformation().Translate(3, 4, 5);
    Ray r2 = r.Transform(transform);
    Point p2 { 4, 6, 8 };
    ASSERT_EQ(r2.Origin(), p2);
    ASSERT_EQ(r2.Direction(), direction);
}

/*
Scenario: Scaling a ray
  Given r ← ray(point(1, 2, 3), vector(0, 1, 0))
    And m ← scaling(2, 3, 4)
  When r2 ← transform(r, m)
  Then r2.origin = point(2, 6, 12)
    And r2.direction = vector(0, 3, 0)
*/

TEST(RayTest, ScalingARay) {
    Point origin { 1, 2, 3 };
    Vector direction { 0, 1, 0 };
    Ray r { origin, direction };
    Matrix transform = Transformation().Scale(2, 3, 4);
    Ray r2 = r.Transform(transform);
    Point p2 { 2, 6, 12 };
    ASSERT_EQ(r2.Origin(), p2);
    Vector v2 { 0, 3, 0 };
    ASSERT_EQ(r2.Direction(), v2);
}