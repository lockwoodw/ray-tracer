#include <gtest/gtest.h>
#include <vector>
#include <array>
#include <cmath>
#include "space.h"
#include "shape.h"
#include "sphere.h"
#include "ray.h"
#include "transformations.h"
#include "plane.h"
#include "utils.h"

static double kSqrt2 = std::sqrt(2);
static double kHalfSqrt2 = kSqrt2 / 2;

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

TEST(IntersectionsTest, PrecomputingTheReflectionVector) {
    Plane p {};
    // The vector originates at the point and slants 45 degrees
    // downwards--confirm using https://www.vcalc.com/wiki/vCalc/V3+-+Vector+Rotation
    Ray r { Point(0, 1, -1), Vector(0, -kHalfSqrt2, kHalfSqrt2) };
    Intersection i { kSqrt2, &p };
    IntersectionComputation comps { i, r };
    ASSERT_EQ(comps.ReflectionVector(), Vector(0, kHalfSqrt2, kHalfSqrt2));
}

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

TEST(IntersectionsTest, ConfirmingTheHitOffsetsThePoint) {
    Point point { 0, 0, -5 };
    Vector vector { 0, 0, 1 };
    Ray r { point, vector };
    Sphere sphere {};
    sphere.SetTransform(Transformation().Translate(0, 0, 1));
    Intersection i { 5, &sphere };
    IntersectionComputation comps { i, r };
    double over_point_z = comps.OverPoint().Z();
    ASSERT_LT(over_point_z, -IntersectionComputation::kEpsilon / 2);
    ASSERT_GT(comps.WorldPoint().Z(), over_point_z);
}

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

TEST(IntersectionsTest, ConfirmingUnderPointIsBelowSurface) {
    Ray r { Point { 0, 0, -5 }, Vector { 0, 0, 1 }};
    Sphere s = GlassySphere();
    Transformation t = Transformation().Translate(0, 0, 1);
    s.SetTransform(t);
    Intersection i { 5, &s };
    IntersectionList intersections {};
    intersections.Add(i);
    IntersectionComputation comps { i, r, &intersections };
    double under_point = comps.UnderPoint().Z();
    ASSERT_GT(under_point, IntersectionComputation::kEpsilon / 2);
    ASSERT_LT(comps.WorldPoint().Z(), under_point);
}

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

TEST(IntersectionsTest, FindingN1AndN2AtVariousIntersections) {
    Sphere a = GlassySphere();
    Transformation at = Transformation().Scale(2, 2, 2);
    Material am {};
    am.RefractiveIndex(1.5);
    a.SetTransform(at);
    a.SetMaterial(am);

    Sphere b = GlassySphere();
    Transformation bt = Transformation().Translate(0, 0, -0.25);
    Material bm {};
    bm.RefractiveIndex(2);
    b.SetTransform(bt);
    b.SetMaterial(bm);

    Sphere c = GlassySphere();
    Transformation ct = Transformation().Translate(0, 0, 0.25);
    Material cm {};
    cm.RefractiveIndex(2.5);
    c.SetTransform(ct);
    c.SetMaterial(cm);

    const Ray r { Point {0, 0, -4}, Vector {0, 0, 1} };

    IntersectionList intersections {};
    intersections.Add(Intersection { 2, &a });
    intersections.Add(Intersection { 2.75, &b });
    intersections.Add(Intersection { 3.25, &c });
    intersections.Add(Intersection { 4.75, &b });
    intersections.Add(Intersection { 5.25, &c });
    intersections.Add(Intersection { 6, &a });

    std::vector<std::array<double, 2>> indices {};
    indices.emplace_back(std::array<double, 2> { 1.0, 1.5 });
    indices.emplace_back(std::array<double, 2> { 1.5, 2.0 });
    indices.emplace_back(std::array<double, 2> { 2.0, 2.5 });
    indices.emplace_back(std::array<double, 2> { 2.5, 2.5 });
    indices.emplace_back(std::array<double, 2> { 2.5, 1.5 });
    indices.emplace_back(std::array<double, 2> { 1.5, 1.0 });

    for (int i = 0; i < indices.size(); i++) {
        const Intersection* x = intersections[i];
        IntersectionComputation ic { *x, r, &intersections };
        ASSERT_DOUBLE_EQ(indices[i][0], ic.N1());
        ASSERT_DOUBLE_EQ(indices[i][1], ic.N2());
    }
}

/*
Scenario: The Schlick approximation under total internal reflection
  Given shape ← glass_sphere()
    And r ← ray(point(0, 0, √2/2), vector(0, 1, 0))
    And xs ← intersections(-√2/2:shape, √2/2:shape)
  When comps ← prepare_computations(xs[1], r, xs)
    And reflectance ← schlick(comps)
  Then reflectance = 1.0
*/

TEST(IntersectionsTest, FindingSchlickApproximationUnderTotalInternalReflection) {
    Sphere shape = GlassySphere();
    Ray r { Point { 0, 0, kHalfSqrt2 }, Vector { 0, 1, 0 } };
    Intersection x1 { -kHalfSqrt2, &shape },
                 x2 { kHalfSqrt2, &shape };
    IntersectionList intersections {};
    intersections << x1 << x2;
    IntersectionComputation comps { x2, r, &intersections };
    ASSERT_TRUE(simple_floating_point_compare(comps.Reflectance(), 1.0));
}

/*
Scenario: The Schlick approximation with a perpendicular viewing angle
  Given shape ← glass_sphere()
    And r ← ray(point(0, 0, 0), vector(0, 1, 0))
    And xs ← intersections(-1:shape, 1:shape)
  When comps ← prepare_computations(xs[1], r, xs)
    And reflectance ← schlick(comps)
  Then reflectance = 0.04
*/

TEST(IntersectionsTest, FindingSchlickApproximationWithAPerpendicularViewingAngle) {
    Sphere shape = GlassySphere();
    Ray r { Point { 0, 0, 0 }, Vector { 0, 1, 0 } };
    Intersection x1 { -1, &shape },
                 x2 { 1, &shape };
    IntersectionList intersections {};
    intersections << x1 << x2;
    IntersectionComputation comps { x2, r, &intersections };
    ASSERT_TRUE(simple_floating_point_compare(comps.Reflectance(), 0.04));
}

/*
Scenario: The Schlick approximation with small angle and n2 > n1
  Given shape ← glass_sphere()
    And r ← ray(point(0, 0.99, -2), vector(0, 0, 1))
    And xs ← intersections(1.8589:shape)
  When comps ← prepare_computations(xs[0], r, xs)
    And reflectance ← schlick(comps)
  Then reflectance = 0.48873
*/

TEST(IntersectionsTest, FindingSchlickApproximationWithASmallAngle) {
    Sphere shape = GlassySphere();
    Ray r { Point { 0, 0.99, -2 }, Vector { 0, 0, 1 } };
    Intersection x1 { 1.8589, &shape };
    IntersectionList intersections {};
    intersections << x1;
    IntersectionComputation comps { x1, r, &intersections };
    ASSERT_TRUE(simple_floating_point_compare(comps.Reflectance(), 0.48873));
}

/*
Scenario: An intersection can encapsulate `u` and `v`
  Given s ← triangle(point(0, 1, 0), point(-1, 0, 0), point(1, 0, 0))
  When i ← intersection_with_uv(3.5, s, 0.2, 0.4)
  Then i.u = 0.2
    And i.v = 0.4
*/