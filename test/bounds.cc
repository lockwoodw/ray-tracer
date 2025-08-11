#include <gtest/gtest.h>

#define _USE_MATH_DEFINES // for M_PI
#include <cmath>
#include <limits>
#include <utility>
#include <array>

#include "bounds.h"

#include "space.h"
#include "ray.h"
#include "shape.h"
#include "transformations.h"

/*
Scenario: Creating an empty bounding box
  Given box ← bounding_box(empty)
  Then box.min = point(infinity, infinity, infinity)
    And box.max = point(-infinity, -infinity, -infinity)
*/

TEST(BoundsTest, CreatingAnEmptyBoundingBox) {
    BoundingBox box {};
    double infinity = std::numeric_limits<double>::infinity();
    Point min { infinity, infinity, infinity },
          max { -infinity, -infinity, -infinity };

    ASSERT_EQ(min, box.Min());
    ASSERT_EQ(max, box.Max());
}

/*
Scenario: Creating a bounding box with volume
  Given box ← bounding_box(min=point(-1, -2, -3) max=point(3, 2, 1))
  Then box.min = point(-1, -2, -3)
    And box.max = point(3, 2, 1)
*/

TEST(BoundsTest, CreatingABoundingBoxWithAVolume) {
    Point min { -1, -2, -3 },
          max { 3, 2, 1 };
    BoundingBox box { min, max };
    ASSERT_EQ(min, box.Min());
    ASSERT_EQ(max, box.Max());
}

/*
Scenario: Adding points to an empty bounding box
  Given box ← bounding_box(empty)
    And p1 ← point(-5, 2, 0)
    And p2 ← point(7, 0, -3)
  When p1 is added to box
    And p2 is added to box
  Then box.min = point(-5, 0, -3)
    And box.max = point(7, 2, 0)
*/

TEST(BoundsTest, AddingPointsToAnEmptyBoundingBox) {
    Point p1 { -5, 2, 0 },
          p2 { 7, 0, -3 },
          min { -5, 0, -3 },
          max { 7, 2, 0 };
    BoundingBox box {};
    box.Add(p1);
    box.Add(p2);
    ASSERT_EQ(min, box.Min());
    ASSERT_EQ(max, box.Max());
}

/*
Scenario: Adding one bounding box to another
  Given box1 ← bounding_box(min=point(-5, -2, 0) max=point(7, 4, 4))
    And box2 ← bounding_box(min=point(8, -7, -2) max=point(14, 2, 8))
  When box2 is added to box1
  Then box1.min = point(-5, -7, -2)
    And box1.max = point(14, 4, 8)
*/

TEST(BoundsTest, AddingOneBoundingBoxToAnother) {
    BoundingBox box1 { Point { -5, -2, 0 }, Point { 7, 4, 4 } },
                box2 { Point { 8, -7, -2 }, Point { 14, 2, 8 } };
    box1.Add(box2);
    ASSERT_EQ(box1.Min(), Point(-5, -7, -2));
    ASSERT_EQ(box1.Max(), Point(14, 4, 8));
}

/*
Scenario Outline: Checking to see if a box contains a given point
  Given box ← bounding_box(min=point(5, -2, 0) max=point(11, 4, 7))
    And p ← <point>
  Then box_contains_point(box, p) is <result>

  Examples:
    | point           | result |
    | point(5, -2, 0) | true   |
    | point(11, 4, 7) | true   |
    | point(8, 1, 3)  | true   |
    | point(3, 0, 3)  | false  |
    | point(8, -4, 3) | false  |
    | point(8, 1, -1) | false  |
    | point(13, 1, 3) | false  |
    | point(8, 5, 3)  | false  |
    | point(8, 1, 8)  | false  |
*/

TEST(BoundsTest, ConfirmingIfABoxContainsAPoint) {
    BoundingBox box { Point { 5, -2, 0 }, Point { 11, 4, 7 } };
    using Record = std::pair<Point, bool>;
    std::array<Record, 9> records = {
        Record { Point { 5, -2, 0 }, true },
        Record { Point { 11, 4, 7 }, true },
        Record { Point { 8, 1, 3 },  true },
        Record { Point { 3, 0, 3 },  false },
        Record { Point { 8, -4, 3 }, false },
        Record { Point { 8, 1, -1 }, false },
        Record { Point { 13, 1, 3 }, false },
        Record { Point { 8, 5, 3 },  false },
        Record { Point { 8, 1, 8 },  false }
    };
    for (auto record: records) {
        ASSERT_EQ(box.Contains(std::get<0>(record)), std::get<1>(record));
    }
}

/*
Scenario Outline: Checking to see if a box contains a given box
  Given box ← bounding_box(min=point(5, -2, 0) max=point(11, 4, 7))
    And box2 ← bounding_box(min=<min> max=<max>)
  Then box_contains_box(box, box2) is <result>

  Examples:
    | min              | max             | result |
    | point(5, -2, 0)  | point(11, 4, 7) | true   |
    | point(6, -1, 1)  | point(10, 3, 6) | true   |
    | point(4, -3, -1) | point(10, 3, 6) | false  |
    | point(6, -1, 1)  | point(12, 5, 8) | false  |
*/

TEST(BoundsTest, ConfirmingIfABoxContainsAnotherBox) {
    BoundingBox box { Point { 5, -2, 0 }, Point { 11, 4, 7 } };
    using Record = std::tuple<Point, Point, bool>;
    std::array<Record, 4> records = {
        Record { Point { 5, -2, 0 },  Point { 11, 4, 7 }, true },
        Record { Point { 6, -1, 1 },  Point { 10, 3, 6 }, true },
        Record { Point { 4, -3, -1 }, Point { 10, 3, 6 }, false },
        Record { Point { 6, -1, 1 },  Point { 12, 5, 8 }, false }
    };
    for (auto record: records) {
        BoundingBox bb { std::get<0>(record), std::get<1>(record) };
        ASSERT_EQ(box.Contains(bb), std::get<2>(record));
    }
}

/*
Scenario: Transforming a bounding box
  Given box ← bounding_box(min=point(-1, -1, -1) max=point(1, 1, 1))
    And matrix ← rotation_x(π / 4) * rotation_y(π / 4)
  When box2 ← transform(box, matrix)
  Then box2.min = point(-1.4142, -1.7071, -1.7071)
    And box2.max = point(1.4142, 1.7071, 1.7071)
*/

TEST(BoundsTest, TransformingABoundingBox) {
    BoundingBox box { Point { -1, -1, -1 }, Point { 1, 1, 1 } };
    double degrees_45 = M_PI / 4;
    Transformation transform = Transformation().RotateY(degrees_45).RotateX(degrees_45);
    BoundingBox transformed = box.Transform(transform);
    Point min { -1.4142, -1.7071, -1.7071 },
          max { 1.4142, 1.7071, 1.7071 };
    for (auto index: BoundingBox::kIndices) {
        ASSERT_NEAR(transformed.Min().At(index), min.At(index), 1e-4);
        ASSERT_NEAR(transformed.Max().At(index), max.At(index), 1e-4);
    }
}

/*
Scenario Outline: Intersecting a ray with a bounding box at the origin
  Given box ← bounding_box(min=point(-1, -1, -1) max=point(1, 1, 1))
    And direction ← normalize(<direction>)
    And r ← ray(<origin>, direction)
  Then intersects(box, r) is <result>

  Examples:
    | origin            | direction        | result |
    | point(5, 0.5, 0)  | vector(-1, 0, 0) | true   |
    | point(-5, 0.5, 0) | vector(1, 0, 0)  | true   |
    | point(0.5, 5, 0)  | vector(0, -1, 0) | true   |
    | point(0.5, -5, 0) | vector(0, 1, 0)  | true   |
    | point(0.5, 0, 5)  | vector(0, 0, -1) | true   |
    | point(0.5, 0, -5) | vector(0, 0, 1)  | true   |
    | point(0, 0.5, 0)  | vector(0, 0, 1)  | true   |
    | point(-2, 0, 0)   | vector(2, 4, 6)  | false  |
    | point(0, -2, 0)   | vector(6, 2, 4)  | false  |
    | point(0, 0, -2)   | vector(4, 6, 2)  | false  |
    | point(2, 0, 2)    | vector(0, 0, -1) | false  |
    | point(0, 2, 2)    | vector(0, -1, 0) | false  |
    | point(2, 2, 0)    | vector(-1, 0, 0) | false  |
*/

TEST(BoundsTest, IntersectingARayWithABoundingBoxAtTheOrigin) {
    BoundingBox box { Point { -1, -1, -1 }, Point { 1, 1, 1 } };
    using Record = std::tuple<Point, Vector, bool>;
    std::array<Record, 13> records = {
        Record { Point { 5, 0.5, 0 },  Vector { -1, 0, 0 }, true },
        Record { Point { -5, 0.5, 0 }, Vector { 1, 0, 0 },  true },
        Record { Point { 0.5, 5, 0 },  Vector { 0, -1, 0 }, true },
        Record { Point { 0.5, -5, 0 }, Vector { 0, 1, 0 },  true },
        Record { Point { 0.5, 0, 5 },  Vector { 0, 0, -1 }, true },
        Record { Point { 0.5, 0, -5 }, Vector { 0, 0, 1 },  true },
        Record { Point { 0, 0.5, 0 },  Vector { 0, 0, 1 },  true },
        Record { Point { -2, 0, 0 },   Vector { 2, 4, 6 },  false },
        Record { Point { 0, -2, 0 },   Vector { 6, 2, 4 },  false },
        Record { Point { 0, 0, -2 },   Vector { 4, 6, 2 },  false },
        Record { Point { 2, 0, 2 },    Vector { 0, 0, -1 }, false },
        Record { Point { 0, 2, 2 },    Vector { 0, -1, 0 }, false },
        Record { Point { 2, 2, 0 },    Vector { -1, 0, 0 }, false },
    };
    for (auto record: records) {
        Ray r { std::get<0>(record), std::get<1>(record).Normalize() };
        ASSERT_EQ(box.Intersects(r), std::get<2>(record));
    }
}

/*
Scenario Outline: Intersecting a ray with a non-cubic bounding box
  Given box ← bounding_box(min=point(5, -2, 0) max=point(11, 4, 7))
    And direction ← normalize(<direction>)
    And r ← ray(<origin>, direction)
  Then intersects(box, r) is <result>

  Examples:
    | origin           | direction        | result |
    | point(15, 1, 2)  | vector(-1, 0, 0) | true   |
    | point(-5, -1, 4) | vector(1, 0, 0)  | true   |
    | point(7, 6, 5)   | vector(0, -1, 0) | true   |
    | point(9, -5, 6)  | vector(0, 1, 0)  | true   |
    | point(8, 2, 12)  | vector(0, 0, -1) | true   |
    | point(6, 0, -5)  | vector(0, 0, 1)  | true   |
    | point(8, 1, 3.5) | vector(0, 0, 1)  | true   |
    | point(9, -1, -8) | vector(2, 4, 6)  | false  |
    | point(8, 3, -4)  | vector(6, 2, 4)  | false  |
    | point(9, -1, -2) | vector(4, 6, 2)  | false  |
    | point(4, 0, 9)   | vector(0, 0, -1) | false  |
    | point(8, 6, -1)  | vector(0, -1, 0) | false  |
    | point(12, 5, 4)  | vector(-1, 0, 0) | false  |
*/

TEST(BoundsTest, IntersectingARayWithANonCubicBoundingBox) {
    BoundingBox box { Point { 5, -2, 0 }, Point { 11, 4, 7 } };
    using Record = std::tuple<Point, Vector, bool>;
    std::array<Record, 13> records = {
        Record { Point { 15, 1, 2 },  Vector { -1, 0, 0 }, true },
        Record { Point { -5, -1, 4 }, Vector { 1, 0, 0 },  true },
        Record { Point { 7, 6, 5 },   Vector { 0, -1, 0 }, true },
        Record { Point { 9, -5, 6 },  Vector { 0, 1, 0 },  true },
        Record { Point { 8, 2, 12 },  Vector { 0, 0, -1 }, true },
        Record { Point { 6, 0, -5 },  Vector { 0, 0, 1 },  true },
        Record { Point { 8, 1, 3.5 }, Vector { 0, 0, 1 },  true },
        Record { Point { 9, -1, -8 }, Vector { 2, 4, 6 },  false },
        Record { Point { 8, 3, -4 },  Vector { 6, 2, 4 },  false },
        Record { Point { 9, -1, -2 }, Vector { 4, 6, 2 },  false },
        Record { Point { 4, 0, 9 },   Vector { 0, 0, -1 }, false },
        Record { Point { 8, 6, -1 },  Vector { 0, -1, 0 }, false },
        Record { Point { 12, 5, 4 },  Vector { -1, 0, 0 }, false },
    };
    for (auto record: records) {
        Ray r { std::get<0>(record), std::get<1>(record).Normalize() };
        ASSERT_EQ(box.Intersects(r), std::get<2>(record));
    }
}

/*
Scenario: Splitting a perfect cube
  Given box ← bounding_box(min=point(-1, -4, -5) max=point(9, 6, 5))
  When (left, right) ← split_bounds(box)
  Then left.min = point(-1, -4, -5)
    And left.max = point(4, 6, 5)
    And right.min = point(4, -4, -5)
    And right.max = point(9, 6, 5)
*/

TEST(BoundsTest, SplittingAPerfectCube) {
    BoundingBox box { Point { -1, -4, -5 }, Point { 9, 6, 5 } };
    auto partition = box.Split();
    ASSERT_EQ(partition[0].Min(), Point(-1, -4, -5));
    ASSERT_EQ(partition[0].Max(), Point(4, 6, 5));
    ASSERT_EQ(partition[1].Min(), Point(4, -4, -5));
    ASSERT_EQ(partition[1].Max(), Point(9, 6, 5));
}

/*
Scenario: Splitting an x-wide box
  Given box ← bounding_box(min=point(-1, -2, -3) max=point(9, 5.5, 3))
  When (left, right) ← split_bounds(box)
  Then left.min = point(-1, -2, -3)
    And left.max = point(4, 5.5, 3)
    And right.min = point(4, -2, -3)
    And right.max = point(9, 5.5, 3)
*/

TEST(BoundsTest, SplittingAnXWideCube) {
    BoundingBox box { Point { -1, -2, -3 }, Point { 9, 5.5, 3 } };
    auto partition = box.Split();
    ASSERT_EQ(partition[0].Min(), Point(-1, -2, -3));
    ASSERT_EQ(partition[0].Max(), Point(4, 5.5, 3));
    ASSERT_EQ(partition[1].Min(), Point(4, -2, -3));
    ASSERT_EQ(partition[1].Max(), Point(9, 5.5, 3));
}

/*
Scenario: Splitting a y-wide box
  Given box ← bounding_box(min=point(-1, -2, -3) max=point(5, 8, 3))
  When (left, right) ← split_bounds(box)
  Then left.min = point(-1, -2, -3)
    And left.max = point(5, 3, 3)
    And right.min = point(-1, 3, -3)
    And right.max = point(5, 8, 3)
*/

TEST(BoundsTest, SplittingAYWideCube) {
    BoundingBox box { Point { -1, -2, -3 }, Point { 5, 8, 3 } };
    auto partition = box.Split();
    ASSERT_EQ(partition[0].Min(), Point(-1, -2, -3));
    ASSERT_EQ(partition[0].Max(), Point(5, 3, 3));
    ASSERT_EQ(partition[1].Min(), Point(-1, 3, -3));
    ASSERT_EQ(partition[1].Max(), Point(5, 8, 3));
}

/*
Scenario: Splitting a z-wide box
  Given box ← bounding_box(min=point(-1, -2, -3) max=point(5, 3, 7))
  When (left, right) ← split_bounds(box)
  Then left.min = point(-1, -2, -3)
    And left.max = point(5, 3, 2)
    And right.min = point(-1, -2, 2)
    And right.max = point(5, 3, 7)
*/

TEST(BoundsTest, SplittingAZWideCube) {
    BoundingBox box { Point { -1, -2, -3 }, Point { 5, 3, 7 } };
    auto partition = box.Split();
    ASSERT_EQ(partition[0].Min(), Point(-1, -2, -3));
    ASSERT_EQ(partition[0].Max(), Point(5, 3, 2));
    ASSERT_EQ(partition[1].Min(), Point(-1, -2, 2));
    ASSERT_EQ(partition[1].Max(), Point(5, 3, 7));
}