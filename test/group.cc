#include <gtest/gtest.h>

#include <array>

#include "group.h"

#include "space.h"
#include "ray.h"
#include "shape.h"
#include "sphere.h"
#include "transformations.h"

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
    g.AddShape(&s);
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
