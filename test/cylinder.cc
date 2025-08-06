#include <gtest/gtest.h>

#include <limits>
#include <array>
// #include <iostream>

#include "cylinder.h"

#include "space.h"
#include "ray.h"
#include "shape.h"

/*
Scenario Outline: A ray misses a cylinder
  Given cyl ← cylinder()
    And direction ← normalize(<direction>)
    And r ← ray(<origin>, direction)
  When xs ← local_intersect(cyl, r)
  Then xs.count = 0

  Examples:
    | origin          | direction       |
    | point(1, 0, 0)  | vector(0, 1, 0) |
    | point(0, 0, 0)  | vector(0, 1, 0) |
    | point(0, 0, -5) | vector(1, 1, 1) |
*/

TEST(CylinderTest, ConfirmingARayMissesACylinder) {
    Cylinder c {};

    std::array<std::pair<Point, Vector>, 3> tests {
        std::pair<Point, Vector> { Point {1, 0, 0}, Vector { 0, 1, 0 } },
        std::pair<Point, Vector> { Point {0, 0, 0}, Vector { 0, 1, 0 } },
        std::pair<Point, Vector> { Point {0, 0, -5}, Vector { 1, 1, 1 } },
    };

    for (auto pair: tests) {
        Vector direction { std::get<1>(pair) };
        Ray r { std::get<0>(pair), direction.Normalize() };
        IntersectionList xs {};
        c.Intersect(xs, r);
        ASSERT_EQ(xs.Size(), 0);
    }
}

/*
Scenario Outline: A ray strikes a cylinder
  Given cyl ← cylinder()
    And direction ← normalize(<direction>)
    And r ← ray(<origin>, direction)
  When xs ← local_intersect(cyl, r)
  Then xs.count = 2
    And xs[0].t = <t0>
    And xs[1].t = <t1>

  Examples:
    | origin            | direction         | t0      | t1      |
    | point(1, 0, -5)   | vector(0, 0, 1)   | 5       | 5       |
    | point(0, 0, -5)   | vector(0, 0, 1)   | 4       | 6       |
    | point(0.5, 0, -5) | vector(0.1, 1, 1) | 6.80798 | 7.08872 |
*/

TEST(CylinderTest, ConfirmingARayStrikesACylinder) {
    Cylinder c {};
    using Record = std::tuple<Point, Vector, double, double>;
    std::array<Record, 3> tests {
        Record { Point {1, 0, -5}, Vector { 0, 0, 1 }, 5, 5 },
        Record { Point {0, 0, -5}, Vector { 0, 0, 1 }, 4, 6 },
        Record { Point {0.5, 0, -5}, Vector { 0.1, 1, 1 }, 6.80798, 7.08872 },
    };

    for (auto record: tests) {
        Vector direction { std::get<1>(record) };
        Ray r { std::get<0>(record), direction.Normalize() };
        IntersectionList xs {};
        c.Intersect(xs, r);
        ASSERT_EQ(xs.Size(), 2);
        ASSERT_NEAR(std::get<2>(record), xs[0]->Distance(), 1e-5);
        ASSERT_NEAR(std::get<3>(record), xs[1]->Distance(), 1e-5);
    }
}

/*
Scenario Outline: Normal vector on a cylinder
  Given cyl ← cylinder()
  When n ← local_normal_at(cyl, <point>)
  Then n = <normal>

  Examples:
    | point           | normal           |
    | point(1, 0, 0)  | vector(1, 0, 0)  |
    | point(0, 5, -1) | vector(0, 0, -1) |
    | point(0, -2, 1) | vector(0, 0, 1)  |
    | point(-1, 1, 0) | vector(-1, 0, 0) |
*/

TEST(CylinderTest, ConfirmingNormalVectorOnACylinder) {
    Cylinder c {};

    using Record = std::pair<Point, Vector>;
    std::array<Record, 4> records {
        Record { Point { 1, 0, 0 },  Vector { 1, 0, 0 } },
        Record { Point { 0, 5, -1 }, Vector { 0, 0, -1 } },
        Record { Point { 0, -2, 1 }, Vector { 0, 0, 1 } },
        Record { Point { -1, 1, 0 }, Vector { -1, 0, 0 } },
    };

    for (auto record: records) {
        ASSERT_EQ(std::get<1>(record), c.LocalNormalAt(std::get<0>(record)));
    }
}

/*
Scenario: The default minimum and maximum for a cylinder
  Given cyl ← cylinder()
  Then cyl.minimum = -infinity
    And cyl.maximum = infinity
*/

TEST(CylinderTest, ConfirmingTheDefaultMinAndMaxForACylinder) {
    Cylinder c {};
    double infinity = std::numeric_limits<double>::infinity();
    ASSERT_DOUBLE_EQ(-infinity, c.Minimum());
    ASSERT_DOUBLE_EQ(infinity, c.Maximum());
}

/*
Scenario Outline: Intersecting a constrained cylinder
  Given cyl ← cylinder()
    And cyl.minimum ← 1
    And cyl.maximum ← 2
    And direction ← normalize(<direction>)
    And r ← ray(<point>, direction)
  When xs ← local_intersect(cyl, r)
  Then xs.count = <count>

  Examples:
    |   | point             | direction         | count |
    | 1 | point(0, 1.5, 0)  | vector(0.1, 1, 0) | 0     |
    | 2 | point(0, 3, -5)   | vector(0, 0, 1)   | 0     |
    | 3 | point(0, 0, -5)   | vector(0, 0, 1)   | 0     |
    | 4 | point(0, 2, -5)   | vector(0, 0, 1)   | 0     |
    | 5 | point(0, 1, -5)   | vector(0, 0, 1)   | 0     |
    | 6 | point(0, 1.5, -2) | vector(0, 0, 1)   | 2     |
*/

TEST(CylinderTest, IntersectingAConstrainedCylinder) {
    Cylinder c { 1.0, 2.0, false };

    using Record = std::tuple<Point, Vector, int>;
    std::array<Record, 6> records {
        Record { Point { 0, 1.5, 0 },  Vector { 0.1, 1, 0 }, 0 },
        Record { Point { 0, 3, -5 },   Vector { 0, 0, 1 },   0 },
        Record { Point { 0, 0, -5 },   Vector { 0, 0, 1 },   0 },
        Record { Point { 0, 2, -5 },   Vector { 0, 0, 1 },   0 },
        Record { Point { 0, 1, -5 },   Vector { 0, 0, 1 },   0 },
        Record { Point { 0, 1.5, -2 }, Vector { 0, 0, 1 },   2 }
    };

    for (auto record: records) {
        Ray r { std::get<0>(record), std::get<1>(record).Normalize() };
        IntersectionList xs {};
        c.Intersect(xs, r);
        ASSERT_EQ(xs.Size(), std::get<2>(record));
    }
}

/*
Scenario: The default closed value for a cylinder
  Given cyl ← cylinder()
  Then cyl.closed = false
*/

TEST(CylinderTest, ConfirmingTheDefaultClosedValueForACylinder) {
    Cylinder c { };
    ASSERT_FALSE(c.Closed());
}

/*
Scenario Outline: Intersecting the caps of a closed cylinder
  Given cyl ← cylinder()
    And cyl.minimum ← 1
    And cyl.maximum ← 2
    And cyl.closed ← true
    And direction ← normalize(<direction>)
    And r ← ray(<point>, direction)
  When xs ← local_intersect(cyl, r)
  Then xs.count = <count>

  Examples:
    |   | point            | direction        | count |
    | 1 | point(0, 3, 0)   | vector(0, -1, 0) | 2     |
    | 2 | point(0, 3, -2)  | vector(0, -1, 2) | 2     |
    | 3 | point(0, 4, -2)  | vector(0, -1, 1) | 2     | # corner case
    | 4 | point(0, 0, -2)  | vector(0, 1, 2)  | 2     |
    | 5 | point(0, -1, -2) | vector(0, 1, 1)  | 2     | # corner case
*/

TEST(CylinderTest, IntersectingTheCapsOfAClosedCylinder) {
    Cylinder c { 1.0, 2.0, true };

    using Record = std::tuple<Point, Vector, int>;
    std::array<Record, 5> records {
        Record { Point { 0, 3, 0 },   Vector { 0, -1, 0 }, 2 },
        Record { Point { 0, 3, -2 },  Vector { 0, -1, 2 }, 2 },
        Record { Point { 0, 4, -2 },  Vector { 0, -1, 1 }, 2 },
        Record { Point { 0, 0, -2 },  Vector { 0, 1, 2 },  2 },
        Record { Point { 0, -1, -2 }, Vector { 0, 1, 1 },  2 }
    };

    for (auto record: records) {
        Ray r { std::get<0>(record), std::get<1>(record).Normalize() };
        IntersectionList xs {};
        c.Intersect(xs, r);
        ASSERT_EQ(xs.Size(), std::get<2>(record));
    }
}

/*
Scenario Outline: The normal vector on a cylinder's end caps
  Given cyl ← cylinder()
    And cyl.minimum ← 1
    And cyl.maximum ← 2
    And cyl.closed ← true
  When n ← local_normal_at(cyl, <point>)
  Then n = <normal>

  Examples:
    | point            | normal           |
    | point(0, 1, 0)   | vector(0, -1, 0) |
    | point(0.5, 1, 0) | vector(0, -1, 0) |
    | point(0, 1, 0.5) | vector(0, -1, 0) |
    | point(0, 2, 0)   | vector(0, 1, 0)  |
    | point(0.5, 2, 0) | vector(0, 1, 0)  |
    | point(0, 2, 0.5) | vector(0, 1, 0)  |
*/