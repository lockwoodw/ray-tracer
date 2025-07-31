#include <gtest/gtest.h>

#include <vector>
#include <utility>

#include "cube.h"

#include "space.h"
#include "ray.h"
#include "shape.h"

/*
Scenario Outline: A ray intersects a cube
  Given c ← cube()
    And r ← ray(<origin>, <direction>)
  When xs ← local_intersect(c, r)
  Then xs.count = 2
    And xs[0].t = <t1>
    And xs[1].t = <t2>

  Examples:
    |        | origin            | direction        | t1 | t2 |
    | +x     | point(5, 0.5, 0)  | vector(-1, 0, 0) |  4 |  6 |
    | -x     | point(-5, 0.5, 0) | vector(1, 0, 0)  |  4 |  6 |
    | +y     | point(0.5, 5, 0)  | vector(0, -1, 0) |  4 |  6 |
    | -y     | point(0.5, -5, 0) | vector(0, 1, 0)  |  4 |  6 |
    | +z     | point(0.5, 0, 5)  | vector(0, 0, -1) |  4 |  6 |
    | -z     | point(0.5, 0, -5) | vector(0, 0, 1)  |  4 |  6 |
    | inside | point(0, 0.5, 0)  | vector(0, 0, 1)  | -1 |  1 |

*/

TEST(CubeTest, ConfirmingARayIntersectsACube) {
    std::vector<std::tuple<Point, Vector, double, double>> test_cases
    {
        { Point { 5, 0.5, 0 },  Vector { -1, 0, 0 },  4, 6 },
        { Point { -5, 0.5, 0 }, Vector { 1, 0, 0 },   4, 6 },
        { Point { 0.5, 5, 0 },  Vector { 0, -1, 0 },  4, 6 },
        { Point { 0.5, -5, 0 }, Vector { 0, 1, 0 },   4, 6 },
        { Point { 0.5, 0, 5 },  Vector { 0, 0, -1 },  4, 6 },
        { Point { 0.5, 0, -5 }, Vector { 0, 0, 1 },   4, 6 },
        { Point { 0, 0.5, 0 },  Vector { 0, 0, 1 },  -1, 1 }
    };

    for (auto test_case: test_cases) {
        Cube c {};
        Point point = std::get<0>(test_case);
        Vector direction = std::get<1>(test_case);
        double t1 = std::get<2>(test_case),
               t2 = std::get<3>(test_case);
        Ray r { point, direction };
        IntersectionList xs {};
        c.AddIntersections(xs, r);
        ASSERT_EQ(xs.Size(), 2);
        ASSERT_DOUBLE_EQ(xs[0]->Distance(), t1);
        ASSERT_DOUBLE_EQ(xs[1]->Distance(), t2);
    }
}

/*
Scenario Outline: A ray misses a cube
  Given c ← cube()
    And r ← ray(<origin>, <direction>)
  When xs ← local_intersect(c, r)
  Then xs.count = 0

  Examples:
    | origin           | direction                      |
    | point(-2, 0, 0)  | vector(0.2673, 0.5345, 0.8018) |
    | point(0, -2, 0)  | vector(0.8018, 0.2673, 0.5345) |
    | point(0, 0, -2)  | vector(0.5345, 0.8018, 0.2673) |
    | point(2, 0, 2)   | vector(0, 0, -1)               |
    | point(0, 2, 2)   | vector(0, -1, 0)               |
    | point(2, 2, 0)   | vector(-1, 0, 0)               |
*/

TEST(CubeTest, ConfirmingARayMissesACube) {
    std::vector<std::pair<Point, Vector>> test_cases
    {
        { Point { -2, 0, 0},   Vector { 0.2673, 0.5345, 0.8018 } },
        { Point { 0, -2, 0 },  Vector { 0.8018, 0.2673, 0.5345 } },
        { Point { 0, 0, -2 },  Vector { 0.5345, 0.8018, 0.2673 } },
        { Point { 2, 0, 2 },   Vector { 0, 0, -1 } },
        { Point { 0, 2, 2 },   Vector { 0, -1, 0 } },
        { Point { 2, 2, 0 },   Vector { -1, 0, 0 } }
    };

    for (auto test_case: test_cases) {
        Cube c {};
        Point point = std::get<0>(test_case);
        Vector direction = std::get<1>(test_case);
        Ray r { point, direction };
        IntersectionList xs {};
        c.AddIntersections(xs, r);
        ASSERT_EQ(xs.Size(), 0);
    }
}

/*
Scenario Outline: The normal on the surface of a cube
  Given c ← cube()
    And p ← <point>
  When normal ← local_normal_at(c, p)
  Then normal = <normal>

  Examples:
    | point                | normal           |
    | point(1, 0.5, -0.8)  | vector(1, 0, 0)  |
    | point(-1, -0.2, 0.9) | vector(-1, 0, 0) |
    | point(-0.4, 1, -0.1) | vector(0, 1, 0)  |
    | point(0.3, -1, -0.7) | vector(0, -1, 0) |
    | point(-0.6, 0.3, 1)  | vector(0, 0, 1)  |
    | point(0.4, 0.4, -1)  | vector(0, 0, -1) |
    | point(1, 1, 1)       | vector(1, 0, 0)  |
    | point(-1, -1, -1)    | vector(-1, 0, 0) |
*/

TEST(CubeTest, ConfirmingTheNormalOnTheSurfaceOfACube) {
    Cube c {};

    std::vector<std::pair<Point, Vector>> test_cases
    {
        { Point { 1, 0.5, -0.8 },   Vector { 1, 0, 0 } },
        { Point { -1, -0.2, 0.9 },  Vector { -1, 0, 0 } },
        { Point { -0.4, 1, -0.1 },  Vector { 0, 1, 0 } },
        { Point { 0.3, -1, -0.7 },  Vector { 0, -1, 0 } },
        { Point { -0.6, 0.3, 1 },   Vector { 0, 0, 1 } },
        { Point { 0.4, 0.4, -1 },   Vector { 0, 0, -1 } },
        { Point { 1, 1, 1 },        Vector { 1, 0, 0 } },
        { Point { -1, -1, -1 },     Vector { -1, 0, 0 } },
    };

    for (auto test_case: test_cases) {
        Point point = std::get<0>(test_case);
        Vector expected = std::get<1>(test_case),
               normal = c.LocalNormalAt(point);
        ASSERT_EQ(normal, expected);
    }
}
