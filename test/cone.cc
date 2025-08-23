#include <gtest/gtest.h>

#include <limits>
#include <array>
// #include <iostream>

#include "cone.h"

#include "space.h"
#include "ray.h"
#include "shape.h"

/*
Scenario Outline: Intersecting a cone with a ray
  Given shape ← cone()
    And direction ← normalize(<direction>)
    And r ← ray(<origin>, direction)
  When xs ← local_intersect(shape, r)
  Then xs.count = 2
    And xs[0].t = <t0>
    And xs[1].t = <t1>

  Examples:
    | origin          | direction           | t0      | t1       |
    | point(0, 0, -5) | vector(0, 0, 1)     | 5       |  5       |
    | point(0, 0, -5) | vector(1, 1, 1)     | 8.66025 |  8.66025 |
    | point(1, 1, -5) | vector(-0.5, -1, 1) | 4.55006 | 49.44994 |
*/

TEST(ConeTest, IntersectingAConeWithARay) {
    Cone c {};
    using Record = std::tuple<Point, Vector, double, double>;
    std::array<Record, 3> records {
        Record { Point { 0, 0, -5 }, Vector { 0, 0, 1 },     5,       5 },
        Record { Point { 0, 0, -5 }, Vector { 1, 1, 1 },     8.66025, 8.66025 },
        Record { Point { 1, 1, -5 }, Vector { -0.5, -1, 1 }, 4.55006, 49.44994 }
    };
    for (auto record: records) {
        Vector direction = std::get<1>(record);
        Ray r { std::get<0>(record), direction.Normalize() };
        IntersectionList xs {};
        c.Intersect(xs, r);
        ASSERT_EQ(xs.Size(), 2);
        ASSERT_NEAR(xs[0]->Distance(), std::get<2>(record), 1e-5);
        ASSERT_NEAR(xs[1]->Distance(), std::get<3>(record), 1e-5);
    }
}

/*
Scenario: Intersecting a cone with a ray parallel to one of its halves
  Given shape ← cone()
    And direction ← normalize(vector(0, 1, 1))
    And r ← ray(point(0, 0, -1), direction)
  When xs ← local_intersect(shape, r)
  Then xs.count = 1
    And xs[0].t = 0.35355
*/

TEST(ConeTest, IntersectingAConeWithARayParallelToOneOfItsHalves) {
    Cone c {};
    Vector direction { 0, 1, 1 };
    Ray r { Point { 0, 0, -1 }, direction.Normalize() };
    IntersectionList xs {};
    c.Intersect(xs, r);
    ASSERT_EQ(xs.Size(), 1);
    ASSERT_NEAR(xs[0]->Distance(), 0.35355, 1e-5);
}

/*
Scenario Outline: Intersecting a cone's end caps
  Given shape ← cone()
    And shape.minimum ← -0.5
    And shape.maximum ← 0.5
    And shape.closed ← true
    And direction ← normalize(<direction>)
    And r ← ray(<origin>, direction)
  When xs ← local_intersect(shape, r)
  Then xs.count = <count>

  Examples:
    | origin             | direction       | count |
    | point(0, 0, -5)    | vector(0, 1, 0) | 0     |
    | point(0, 0, -0.25) | vector(0, 1, 1) | 2     |
    | point(0, 0, -0.25) | vector(0, 1, 0) | 4     |
*/

TEST(ConeTest, IntersectingAConesEndCaps) {
    Cone c { -0.5, 0.5, true };
    using Record = std::tuple<Point, Vector, int>;
    std::array<Record, 3> records {
        Record { Point { 0, 0, -5 },    Vector { 0, 1, 0 }, 0 },
        Record { Point { 0, 0, -0.25 }, Vector { 0, 1, 1 }, 2 },
        Record { Point { 0, 0, -0.25 }, Vector { 0, 1, 0 }, 4 },
    };
    for (auto record: records) {
        Vector direction { std::get<1>(record) };
        Ray r { std::get<0>(record), direction.Normalize() };
        IntersectionList xs {};
        c.Intersect(xs, r);
        ASSERT_EQ(xs.Size(), std::get<2>(record));
    }
}

/*
Scenario Outline: Computing the normal vector on a cone
  Given shape ← cone()
  When n ← local_normal_at(shape, <point>)
  Then n = <normal>

  Examples:
    | point             | normal                 |
    | point(0, 0, 0)    | vector(0, 0, 0)        |
    | point(1, 1, 1)    | vector(1, -√2, 1)      |
    | point(-1, -1, 0)  | vector(-1, 1, 0)       |
*/

TEST(ConeTest, ComputingTheNormalVectorOnACone) {
    Cone c {};
    using Record = std::pair<Point, Vector>;
    std::array<Record, 3> records {
        Record { Point { 0, 0, 0 },   Vector { 0, 0, 0 } },
        Record { Point { 1, 1, 1 },   Vector { 1, -sqrt(2), 1 } },
        Record { Point { -1, -1, 0 }, Vector { -1, 1, 0 } },
    };
    for (auto record: records) {
        Vector n = c.LocalNormalAt(std::get<0>(record));
        ASSERT_EQ(n, std::get<1>(record));
    }
}

/*
Scenario: An unbounded cone has a bounding box
  Given shape ← cone()
  When box ← bounds_of(shape)
  Then box.min = point(-infinity, -infinity, -infinity)
    And box.max = point(infinity, infinity, infinity)
*/

TEST(ConeTest, AnUnboundedConeHasABoundingBox) {
    Cone c {};
    BoundingBox box = c.BoundsOf();
    Point min { -kInfinity, -kInfinity, -kInfinity },
          max { kInfinity, kInfinity, kInfinity };
    ASSERT_EQ(min, box.Min());
    ASSERT_EQ(max, box.Max());
}

/*
Scenario: A bounded cone has a bounding box
  Given shape ← cone()
    And shape.minimum ← -5
    And shape.maximum ← 3
  When box ← bounds_of(shape)
  Then box.min = point(-5, -5, -5)
    And box.max = point(5, 3, 5)
*/

TEST(ConeTest, AnBoundedConeHasABoundingBox) {
    Cone c { -5, 3, true };
    BoundingBox box = c.BoundsOf();
    Point min { -5, -5, -5 },
          max { 5, 3, 5 };
    ASSERT_EQ(min, box.Min());
    ASSERT_EQ(max, box.Max());
}