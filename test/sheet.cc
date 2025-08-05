#include <gtest/gtest.h>

#include "sheet.h"

#include "space.h"
#include "ray.h"
#include "shape.h"

TEST(SheetTest, ConfirmingTheNormalOfASheetIsConstantEverywhere) {
    Sheet s {};
    Vector expected { 0, 1, 0 },
           n1 = s.LocalNormalAt(Point { 0, 0, 0 }),
           n2 = s.LocalNormalAt(Point { 0.25, 0, -0.25 }),
           n3 = s.LocalNormalAt(Point { -0.5, 0, 0.5 });
    ASSERT_EQ(n1, expected);
    ASSERT_EQ(n2, expected);
    ASSERT_EQ(n3, expected);
}

TEST(SheetTest, IntersectingWithARayParallelToTheSheet) {
    Sheet s {};
    Ray r { Point { 0, 10, 0 }, Vector { 0, 0, 1 } };
    IntersectionList xs {};
    s.Intersect(xs, r);
    ASSERT_EQ(xs.Size(), 0);
}

TEST(SheetTest, IntersectingSheetWithACoplanarRay) {
    Sheet s {};
    Ray r { Point { 0, 0, 0 }, Vector { 0, 0, 1 } };
    IntersectionList xs {};
    s.Intersect(xs, r);
    ASSERT_EQ(xs.Size(), 0);
}

TEST(SheetTest, IntersectingWithARayAboveTheSheet) {
    Sheet p {};
    Ray r { Point { 0, 1, 0 }, Vector { 0, -1, 0 } };
    IntersectionList xs {};
    p.Intersect(xs, r);
    ASSERT_EQ(xs.Size(), 1);
    const Intersection* i = xs[0];
    ASSERT_DOUBLE_EQ(i->Distance(), 1);
    ASSERT_EQ(p, *i->Object());
}

TEST(SheetTest, IntersectingWithARayBelowTheSheet) {
    Sheet s {};
    Ray r { Point { 0, -1, 0 }, Vector { 0, 1, 0 } };
    IntersectionList xs {};
    s.Intersect(xs, r);
    ASSERT_EQ(xs.Size(), 1);
    const Intersection* i = xs[0];
    ASSERT_DOUBLE_EQ(i->Distance(), 1);
    ASSERT_EQ(s, *i->Object());
}

TEST(SheetTest, MissingASheet) {
    Sheet s {};
    Ray r { Point { 5, 5, 0 }, Vector { 0, 0, 1 } };
    IntersectionList xs {};
    s.Intersect(xs, r);
    ASSERT_EQ(xs.Size(), 0);
}

TEST(SheetTest, HittingASheetOnAnAngle) {
    Sheet s {};
    Point ray_origin { 5, 5, 0 },
          intersect { 0.2, 0, 0.3 };
    Vector direction = intersect - ray_origin;
    Ray r { ray_origin, direction };
    IntersectionList xs {};
    s.Intersect(xs, r);
    ASSERT_EQ(xs.Size(), 1);
    double distance = xs[0]->Distance();
    Point calculated_intersect = ray_origin + direction * distance;
    ASSERT_EQ(intersect, calculated_intersect);
}
