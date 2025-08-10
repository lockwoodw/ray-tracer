#include <gtest/gtest.h>

#define _USE_MATH_DEFINES // for M_PI
#include <cmath>

#include "disc.h"

#include "space.h"
#include "ray.h"
#include "shape.h"

TEST(DiscTest, IntersectingWithARayAboveTheDisc) {
    Disc d {};
    Ray r { Point { 0, 1, 0 }, Vector { 0, -1, 0 } };
    IntersectionList xs {};
    d.Intersect(xs, r);
    ASSERT_EQ(xs.Size(), 1);
    const Intersection* i = xs[0];
    ASSERT_DOUBLE_EQ(i->Distance(), 1);
    ASSERT_EQ(d, *i->Object());
}

TEST(DiscTest, IntersectingWithARayBelowTheDisc) {
    Disc d {};
    Ray r { Point { 0, -1, 0 }, Vector { 0, 1, 0 } };
    IntersectionList xs {};
    d.Intersect(xs, r);
    ASSERT_EQ(xs.Size(), 1);
    const Intersection* i = xs[0];
    ASSERT_DOUBLE_EQ(i->Distance(), 1);
    ASSERT_EQ(d, *i->Object());
}

TEST(DiscTest, MissingADisc) {
    Disc d {};
    // Pick a point just outside the circumference at Θ = 45°
    double angle = M_PI / 4,
           offset = 1e-4,
           x = std::cos(angle) + offset,
           z = std::sin(angle) + offset;
    Ray r { Point { x, 5, z }, Vector { 0, -1, 0 } };
    IntersectionList xs {};
    d.Intersect(xs, r);
    ASSERT_EQ(xs.Size(), 0);
}

TEST(DiscTest, HittingADisc) {
    Disc d {};
    // Pick a point on the circumference at Θ = 45°
    double angle = M_PI / 4,
           x = std::cos(angle),
           z = std::sin(angle);
    Ray r { Point { x, 5, z }, Vector { 0, -1, 0 } };
    IntersectionList xs {};
    d.Intersect(xs, r);
    ASSERT_EQ(xs.Size(), 1);
    const Intersection* i = xs[0];
    ASSERT_DOUBLE_EQ(i->Distance(), 5);
}

TEST(DiscTest, ADiscHasABoundingBox) {
    Disc d {};
    BoundingBox box = d.BoundsOf();
    Point min { -1, 0, -1 },
          max { 1, 0, 1 };
    ASSERT_EQ(min, box.Min());
    ASSERT_EQ(max, box.Max());
}