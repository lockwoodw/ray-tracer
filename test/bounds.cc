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