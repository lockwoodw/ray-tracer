#include <gtest/gtest.h>

#include <array>

#include "group.h"

#include "space.h"
#include "ray.h"
#include "shape.h"
#include "sphere.h"
#include "transformations.h"
#include "cylinder.h"

/*
Scenario: Creating a new group
  Given g ← group()
  Then g.transform = identity_matrix
    And g is empty
*/

TEST(GroupTest, CreatingANewGroup) {
    ShapeGroup g {};
    ASSERT_EQ(g.Transform(), Matrix::Identity(4));
    ASSERT_TRUE(g.IsEmpty());
}

/*
Scenario: Adding a child to a group
  Given g ← group()
    And s ← test_shape()
  When add_child(g, s)
  Then g is not empty
    And g includes s
    And s.parent = g
*/

TEST(GroupTest, AddingAChildToAGroup) {
    ShapeGroup g {};
    TestShape s {};
    g.Add(&s);
    ASSERT_FALSE(g.IsEmpty());
    ASSERT_TRUE(g.Contains(&s));
    ASSERT_EQ(s.Parent(), &g);
}

/*
Scenario: Intersecting a ray with an empty group
  Given g ← group()
    And r ← ray(point(0, 0, 0), vector(0, 0, 1))
  When xs ← local_intersect(g, r)
  Then xs is empty
*/

TEST(GroupTest, IntersectingARayWithAnEmptyGroup) {
    ShapeGroup g {};
    Ray r { Point { 0, 0, 0 }, Vector { 0, 0, 1 } };
    IntersectionList xs {};
    g.Intersect(xs, r);
    ASSERT_EQ(xs.Size(), 0);
}

/*
Scenario: Intersecting a ray with a nonempty group
  Given g ← group()
    And s1 ← sphere()
    And s2 ← sphere()
    And set_transform(s2, translation(0, 0, -3))
    And s3 ← sphere()
    And set_transform(s3, translation(5, 0, 0))
    And add_child(g, s1)
    And add_child(g, s2)
    And add_child(g, s3)
  When r ← ray(point(0, 0, -5), vector(0, 0, 1))
    And xs ← local_intersect(g, r)
  Then xs.count = 4
    And xs[0].object = s2
    And xs[1].object = s2
    And xs[2].object = s1
    And xs[3].object = s1
*/

TEST(GroupTest, IntersectingARayWithAnNonemptyGroup) {
    ShapeGroup g {};
    Sphere s1 {}, s2 {}, s3 {};
    s2.SetTransform(Transformation().Translate(0, 0, -3));
    s3.SetTransform(Transformation().Translate(5, 0, 0));
    g << &s1 << &s2 << &s3;
    Ray r { Point { 0, 0, -5 }, Vector { 0, 0, 1 } };
    IntersectionList xs {};
    g.Intersect(xs, r);
    ASSERT_EQ(xs.Size(), 4);
    std::array<Sphere*, 4> expected { &s2, &s2, &s1, &s1 };
    int index {};
    for (auto i: xs) {
        ASSERT_EQ(i->Object(), expected[index++]);
    }
}

/*
Scenario: Intersecting a transformed group
  Given g ← group()
    And set_transform(g, scaling(2, 2, 2))
    And s ← sphere()
    And set_transform(s, translation(5, 0, 0))
    And add_child(g, s)
  When r ← ray(point(10, 0, -10), vector(0, 0, 1))
    And xs ← intersect(g, r)
  Then xs.count = 2
*/

TEST(GroupTest, IntersectingATransformedGroup) {
    ShapeGroup g {};
    g.SetTransform(Transformation().Scale(2));
    Sphere s {};
    s.SetTransform(Transformation().Translate(5, 0, 0));
    g << &s;
    Ray r { Point { 10, 0, -10 }, Vector { 0, 0, 1 } };
    IntersectionList xs {};
    g.Intersect(xs, r);
    ASSERT_EQ(xs.Size(), 2);
}

/*
Scenario: A group has a bounding box that contains its children
  Given s ← sphere()
    And set_transform(s, translation(2, 5, -3) * scaling(2, 2, 2))
    And c ← cylinder()
    And c.minimum ← -2
    And c.maximum ← 2
    And set_transform(c, translation(-4, -1, 4) * scaling(0.5, 1, 0.5))
    And shape ← group()
    And add_child(shape, s)
    And add_child(shape, c)
  When box ← bounds_of(shape)
  Then box.min = point(-4.5, -3, -5)
    And box.max = point(4, 7, 4.5)
*/

TEST(GroupTest, ConfirmingAGroupsBoundingBoxContainsItsChildren) {
    Sphere s {};
    s.SetTransform(Transformation().Scale(2).Translate(2, 5, -3));
    Cylinder c { -2, 2, true };
    c.SetTransform(Transformation().Scale(0.5, 1, 0.5).Translate(-4, -1, 4));
    ShapeGroup group {};
    group << &s << &c;
    BoundingBox box = group.BoundsOf();
    ASSERT_EQ(box.Min(), Point(-4.5, -3, -5));
    ASSERT_EQ(box.Max(), Point(4, 7, 4.5));
}

/*
Scenario: Intersecting ray+group doesn't test children if box is missed
  Given child ← test_shape()
    And shape ← group()
    And add_child(shape, child)
    And r ← ray(point(0, 0, -5), vector(0, 1, 0))
  When xs ← intersect(shape, r)
  Then child.saved_ray is unset
*/

TEST(GroupTest, IntersectingGroupDoesNotTestChildrenIfBoxIsMissed) {
    TestShape child {};
    ShapeGroup group {};
    group << &child;
    Ray r { Point { 0, 0, -5 }, Vector { 0, 1, 0 } };
    IntersectionList xs {};
    ASSERT_FALSE(group.Intersect(xs, r));
}

/*
Scenario: Intersecting ray+group tests children if box is hit
Given child ← test_shape()
And shape ← group()
And add_child(shape, child)
And r ← ray(point(0, 0, -5), vector(0, 0, 1))
When xs ← intersect(shape, r)
Then child.saved_ray is set
*/

TEST(GroupTest, IntersectingGroupTestsChildrenIfBoxIsHit) {
    TestShape child {};
    ShapeGroup group {};
    group << &child;
    Ray r { Point { 0, 0, -5 }, Vector { 0, 0, 1 } };
    IntersectionList xs {};
    ASSERT_TRUE(group.Intersect(xs, r));
}
