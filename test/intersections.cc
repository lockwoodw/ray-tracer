#include <gtest/gtest.h>
#include <vector>
#include "space.h"
#include "shape.h"
#include "sphere.h"
#include "ray.h"

/*
Scenario: An intersection encapsulates t and object
  Given s ← sphere()
  When i ← intersection(3.5, s)
  Then i.t = 3.5
    And i.object = s
*/

TEST(IntersectionsTest, EncapsulatingADistanceAndAnObject) {
    Point origin { 0, 0, 0 };
    Sphere s { origin, 1.0 };
    double distance { 3.5 };
    Intersection i { distance, &s };
    ASSERT_EQ(i.Distance(), distance);
    ASSERT_EQ(*i.Object(), s);
}

/*
Scenario: Precomputing the state of an intersection
  Given r ← ray(point(0, 0, -5), vector(0, 0, 1))
    And shape ← sphere()
    And i ← intersection(4, shape)
  When comps ← prepare_computations(i, r)
  Then comps.t = i.t
    And comps.object = i.object
    And comps.point = point(0, 0, -1)
    And comps.eyev = vector(0, 0, -1)
    And comps.normalv = vector(0, 0, -1)
*/

TEST(IntersectionsTest, PrecomputingTheStateOfAnIntersection) {
    Point point { 0, 0, -5 };
    Vector vector { 0, 0, 1 };
    Ray r { point, vector };
    Sphere sphere {};
    Intersection i { 4, &sphere };
    IntersectionComputation comps { i, r };
    ASSERT_DOUBLE_EQ(comps.Distance(), i.Distance());
    ASSERT_EQ(comps.Object(), i.Object());
    Point expected_point { 0, 0, -1 };
    ASSERT_EQ(comps.WorldPoint(), expected_point);
    Vector expected_eye_vector { 0, 0, -1 };
    ASSERT_EQ(comps.EyeVector(), expected_eye_vector);
    Vector expected_normal_vector { 0, 0, -1 };
    ASSERT_EQ(comps.NormalVector(), expected_normal_vector);
}

/*
Scenario: Precomputing the reflection vector
  Given shape ← plane()
    And r ← ray(point(0, 1, -1), vector(0, -√2/2, √2/2))
    And i ← intersection(√2, shape)
  When comps ← prepare_computations(i, r)
  Then comps.reflectv = vector(0, √2/2, √2/2)
*/

/*
Scenario: The hit, when an intersection occurs on the outside
  Given r ← ray(point(0, 0, -5), vector(0, 0, 1))
    And shape ← sphere()
    And i ← intersection(4, shape)
  When comps ← prepare_computations(i, r)
  Then comps.inside = false
*/

TEST(IntersectionsTest, PrecomputingTheStateOfAnIntersectionOnTheOutside) {
    Point point { 0, 0, -5 };
    Vector vector { 0, 0, 1 };
    Ray r { point, vector };
    Sphere sphere {};
    Intersection i { 4, &sphere };
    IntersectionComputation comps { i, r };
    ASSERT_FALSE(comps.Inside());
}

/*
Scenario: The hit, when an intersection occurs on the inside
  Given r ← ray(point(0, 0, 0), vector(0, 0, 1))
    And shape ← sphere()
    And i ← intersection(1, shape)
  When comps ← prepare_computations(i, r)
  Then comps.point = point(0, 0, 1)
    And comps.eyev = vector(0, 0, -1)
    And comps.inside = true
      # normal would have been (0, 0, 1), but is inverted!
    And comps.normalv = vector(0, 0, -1)
*/

TEST(IntersectionsTest, PrecomputingTheStateOfAnIntersectionOnTheInside) {
    Point point { 0, 0, 0 };
    Vector vector { 0, 0, 1 };
    Ray r { point, vector };
    Sphere sphere {};
    Intersection i { 1, &sphere };
    IntersectionComputation comps { i, r };
    Point expected_point { 0, 0, 1 };
    ASSERT_EQ(comps.WorldPoint(), expected_point);
    Vector expected_eye_vector { 0, 0, -1 };
    ASSERT_EQ(comps.EyeVector(), expected_eye_vector);
    ASSERT_TRUE(comps.Inside());
    Vector expected_normal_vector { 0, 0, -1 };
    ASSERT_EQ(comps.NormalVector(), expected_normal_vector);
}

/*
Scenario: The hit should offset the point
  Given r ← ray(point(0, 0, -5), vector(0, 0, 1))
    And shape ← sphere() with:
      | transform | translation(0, 0, 1) |
    And i ← intersection(5, shape)
  When comps ← prepare_computations(i, r)
  Then comps.over_point.z < -EPSILON/2
    And comps.point.z > comps.over_point.z
*/

/*
Scenario: The under point is offset below the surface
  Given r ← ray(point(0, 0, -5), vector(0, 0, 1))
    And shape ← glass_sphere() with:
      | transform | translation(0, 0, 1) |
    And i ← intersection(5, shape)
    And xs ← intersections(i)
  When comps ← prepare_computations(i, r, xs)
  Then comps.under_point.z > EPSILON/2
    And comps.point.z < comps.under_point.z
*/

/*
Scenario: Aggregating intersections
  Given s ← sphere()
    And i1 ← intersection(1, s)
    And i2 ← intersection(2, s)
  When xs ← intersections(i1, i2)
  Then xs.count = 2
    And xs[0].t = 1
    And xs[1].t = 2
*/

TEST(IntersectionsTest, AggregatingIntersections) {
    Point origin { 0, 0, 0 };
    Sphere s { origin, 1.0 };
    double d1 { 1 }, d2 { 2 };
    Intersection* i1 = new Intersection { d1, &s };
    Intersection* i2 = new Intersection { d2, &s };
    IntersectionList xs {};
    xs << i1 << i2;
    ASSERT_EQ(xs.Size(), 2);
    ASSERT_EQ(xs[0]->Distance(), d1);
    ASSERT_EQ(xs[1]->Distance(), d2);
}

/*
Scenario: The hit, when all intersections have positive t
  Given s ← sphere()
    And i1 ← intersection(1, s)
    And i2 ← intersection(2, s)
    And xs ← intersections(i2, i1)
  When i ← hit(xs)
  Then i = i1
*/

TEST(IntersectionsTest, FindingTheHitForPositiveDistances) {
    Point origin { 0, 0, 0 };
    Sphere s { origin, 1.0 };
    Intersection i1 { 1, &s };
    Intersection i2 { 2, &s };
    IntersectionList xs {};
    xs.Add(i1);
    xs.Add(i2);
    ASSERT_EQ(*xs.Hit(), i1);
}

/*
Scenario: The hit, when some intersections have negative t
  Given s ← sphere()
    And i1 ← intersection(-1, s)
    And i2 ← intersection(1, s)
    And xs ← intersections(i2, i1)
  When i ← hit(xs)
  Then i = i2
*/

TEST(IntersectionsTest, FindingTheHitForMixedDistances) {
    Point origin { 0, 0, 0 };
    Sphere s { origin, 1.0 };
    Intersection i1 { -1, &s };
    Intersection i2 { 1, &s };
    IntersectionList xs {};
    xs.Add(i1);
    xs.Add(i2);
    ASSERT_EQ(*xs.Hit(), i2);
}

/*
Scenario: The hit, when all intersections have negative t
  Given s ← sphere()
    And i1 ← intersection(-2, s)
    And i2 ← intersection(-1, s)
    And xs ← intersections(i2, i1)
  When i ← hit(xs)
  Then i is nothing
*/

TEST(IntersectionsTest, FindingTheHitForNegativeDistances) {
    Point origin { 0, 0, 0 };
    Sphere s { origin, 1.0 };
    Intersection i1 { -2, &s };
    Intersection i2 { -1, &s };
    IntersectionList xs {};
    xs.Add(i1);
    xs.Add(i2);
    ASSERT_TRUE(xs.Hit() == nullptr);
}

/*
Scenario: The hit is always the lowest nonnegative intersection
  Given s ← sphere()
  And i1 ← intersection(5, s)
  And i2 ← intersection(7, s)
  And i3 ← intersection(-3, s)
  And i4 ← intersection(2, s)
  And xs ← intersections(i1, i2, i3, i4)
When i ← hit(xs)
Then i = i4
*/

TEST(IntersectionsTest, FindingTheHitForUnorderedMixedDistances) {
    Point origin { 0, 0, 0 };
    Sphere s { origin, 1.0 };
    Intersection i1 { 5, &s },
                 i2 { 7, &s },
                 i3 { -3, &s },
                 i4 { 2, &s };
    IntersectionList xs {};
    xs << i1 << i2 << i3 << i4;
    ASSERT_EQ(*xs.Hit(), i4);
}

/*
Scenario Outline: Finding n1 and n2 at various intersections
  Given A ← glass_sphere() with:
      | transform                 | scaling(2, 2, 2) |
      | material.refractive_index | 1.5              |
    And B ← glass_sphere() with:
      | transform                 | translation(0, 0, -0.25) |
      | material.refractive_index | 2.0                      |
    And C ← glass_sphere() with:
      | transform                 | translation(0, 0, 0.25) |
      | material.refractive_index | 2.5                     |
    And r ← ray(point(0, 0, -4), vector(0, 0, 1))
    And xs ← intersections(2:A, 2.75:B, 3.25:C, 4.75:B, 5.25:C, 6:A)
  When comps ← prepare_computations(xs[<index>], r, xs)
  Then comps.n1 = <n1>
    And comps.n2 = <n2>

  Examples:
    | index | n1  | n2  |
    | 0     | 1.0 | 1.5 |
    | 1     | 1.5 | 2.0 |
    | 2     | 2.0 | 2.5 |
    | 3     | 2.5 | 2.5 |
    | 4     | 2.5 | 1.5 |
    | 5     | 1.5 | 1.0 |
*/

/*
Scenario: The Schlick approximation under total internal reflection
  Given shape ← glass_sphere()
    And r ← ray(point(0, 0, √2/2), vector(0, 1, 0))
    And xs ← intersections(-√2/2:shape, √2/2:shape)
  When comps ← prepare_computations(xs[1], r, xs)
    And reflectance ← schlick(comps)
  Then reflectance = 1.0
*/

/*
Scenario: The Schlick approximation with a perpendicular viewing angle
  Given shape ← glass_sphere()
    And r ← ray(point(0, 0, 0), vector(0, 1, 0))
    And xs ← intersections(-1:shape, 1:shape)
  When comps ← prepare_computations(xs[1], r, xs)
    And reflectance ← schlick(comps)
  Then reflectance = 0.04
*/

/*
Scenario: The Schlick approximation with small angle and n2 > n1
  Given shape ← glass_sphere()
    And r ← ray(point(0, 0.99, -2), vector(0, 0, 1))
    And xs ← intersections(1.8589:shape)
  When comps ← prepare_computations(xs[0], r, xs)
    And reflectance ← schlick(comps)
  Then reflectance = 0.48873
*/

/*
Scenario: An intersection can encapsulate `u` and `v`
  Given s ← triangle(point(0, 1, 0), point(-1, 0, 0), point(1, 0, 0))
  When i ← intersection_with_uv(3.5, s, 0.2, 0.4)
  Then i.u = 0.2
    And i.v = 0.4
*/