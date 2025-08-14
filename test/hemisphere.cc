#define _USE_MATH_DEFINES // for M_PI
#include <gtest/gtest.h>
#include <cmath>
#include "ray.h"
#include "hemisphere.h"
#include "transformations.h"

TEST(HemisphereTest, ConfirmingAMissThroughTheRightHemisphere) {
    Hemisphere h {};
    Ray r { Point { 0.5, 1, 0 }, Vector { 0, -1, 0 } };
    IntersectionList xs {};
    h.Intersect(xs, r);
    ASSERT_EQ(xs.Size(), 0);
}

TEST(HemisphereTest, ConfirmingHitsThroughTheLeftHemisphere) {
    Hemisphere h {};
    Ray r { Point { -0.5, 1, 0 }, Vector { 0, -1, 0 } };
    IntersectionList xs {};
    h.Intersect(xs, r);
    ASSERT_EQ(xs.Size(), 2);
    Point i0 { -0.5, std::sin(M_PI * 120/180), 0 }, // Θ = 120°
          i1 { -0.5, std::sin(M_PI * 240/180), 0 }, // Θ = 240°
          actual_0 = r.Origin() + r.Direction() * xs[0]->Distance(),
          actual_1 = r.Origin() + r.Direction() * xs[1]->Distance();
    ASSERT_EQ(i0, actual_0);
    ASSERT_EQ(i1, actual_1);
}

TEST(HemisphereTest, ConfirmingHitThroughTheFlatPlane) {
    Hemisphere h {};
    Ray r { Point { -5, 0, 0 }, Vector { 1, 0, 0 } };
    IntersectionList xs {};
    h.Intersect(xs, r);
    ASSERT_EQ(xs.Size(), 2);
    ASSERT_DOUBLE_EQ(xs[0]->Distance(), 4);
    ASSERT_DOUBLE_EQ(xs[1]->Distance(), 5);
}

TEST(HemisphereTest, IntersectingAHemisphereFromInside) {
    Point origin { -0.5, 0, 0 };
    Vector direction { 1, 0, 0 };
    Ray r { origin, direction };
    Hemisphere h { };
    IntersectionList xs {};
    h.Intersect(xs, r);
    ASSERT_EQ(xs.Size(), 2);
    ASSERT_DOUBLE_EQ(xs[0]->Distance(), -0.5);
    ASSERT_DOUBLE_EQ(xs[1]->Distance(), 0.5);
}

TEST(HemisphereTest, FindingTheNormalOfAHemisphereAtANonaxialPoint) {
    Hemisphere h {};
    double d { std::sqrt(3) /  3 };
    Point p { -d, d, d };
    Vector expected { -d, d, d },
           n = h.NormalAt(p);
    ASSERT_EQ(n, expected);
}

TEST(HemisphereTest, FindingTheNormalOfAHemisphereOnTheFlatPlane) {
    Hemisphere h {};
    Point p { 0, 0.2, 0.3 };
    Vector expected { 1, 0, 0 },
           n = h.NormalAt(p);
    ASSERT_EQ(n, expected);
}

TEST(HemisphereTest, ConfirmingTheBoundingBoxForAHemisphere) {
    Hemisphere h {};
    BoundingBox box = h.BoundsOf();
    Point min { -1, -1, -1 },
          max { 0, 1, 1 };
    ASSERT_EQ(min, box.Min());
    ASSERT_EQ(max, box.Max());
}

TEST(HemisphereTest, ConfirmingTheBoundingBoxForATransformedHemisphere) {
    Hemisphere h {};
    h.SetTransform(Transformation().Scale(2).RotateZ(M_PI/2));
    BoundingBox box = h.BoundsOfInParentSpace();
    Point min { -2, -2, -2 },
          max { 2, 0, 2 };
    ASSERT_EQ(min, box.Min());
    ASSERT_EQ(max, box.Max());
}
