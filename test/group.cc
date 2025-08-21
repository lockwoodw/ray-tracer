#include <gtest/gtest.h>

#include <array>
#include <vector>

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
    g << &s;
    s.SetTransform(Transformation().Translate(5, 0, 0));
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

TEST(GroupTest, IntersectingGroupOfGroups) {
    Sphere s {};
    s.SetTransform(Transformation().Scale(2));
    ShapeGroup inner {};
    inner.SetTransform(Transformation().Translate(0, 1, 0));
    inner << &s;
    ShapeGroup outer {};
    outer << &inner;
    Ray r { Point { 0, 3, -5 }, Vector { 0, 0, 1 } };
    IntersectionList xs {};
    ASSERT_TRUE(outer.Intersect(xs, r));
    ASSERT_DOUBLE_EQ(xs[0]->Distance(), 5);
}

/*
Scenario: Partitioning a group's children
  Given s1 ← sphere() with:
      | transform | translation(-2, 0, 0) |
    And s2 ← sphere() with:
      | transform | translation(2, 0, 0) |
    And s3 ← sphere()
    And g ← group() of [s1, s2, s3]
  When (left, right) ← partition_children(g)
  Then g is a group of [s3]
    And left = [s1]
    And right = [s2]
*/

TEST(GroupTest, PartitioningAGroupsChildren) {
    Sphere s1 {};
    s1.SetTransform(Transformation().Translate(-2, 0, 0));
    Sphere s2 {};
    s2.SetTransform(Transformation().Translate(2, 0, 0));
    Sphere s3 {};
    ShapeGroup g {};
    g << &s1 << &s2 << &s3;
    auto partitions = g.Partition();
    ASSERT_TRUE(g.Contains(&s3));
    ASSERT_FALSE(g.Contains(&s1));
    ASSERT_FALSE(g.Contains(&s2));
    ASSERT_EQ(partitions[0].back(), &s1);
    ASSERT_EQ(partitions[1].back(), &s2);
}

/*
Scenario: Creating a sub-group from a list of children
  Given s1 ← sphere()
    And s2 ← sphere()
    And g ← group()
  When make_subgroup(g, [s1, s2])
  Then g.count = 1
    And g[0] is a group of [s1, s2]
*/

TEST(GroupTest, CreatingASubgroupFromAListOfChildren) {
    Sphere s1 {},
           s2 {};
    std::vector<Shape*> list { &s1, &s2 };
    ShapeGroup g {};
    g.AddSubgroup(list);
    ASSERT_EQ(g.Size(), 1);
    ShapeGroup* subgroup = static_cast<ShapeGroup*>(g[0]);
    ASSERT_TRUE(subgroup->Contains(&s1));
    ASSERT_TRUE(subgroup->Contains(&s2));
}

/*
Scenario: Subdividing a group partitions its children
  Given s1 ← sphere() with:
      | transform | translation(-2, -2, 0) |
    And s2 ← sphere() with:
      | transform | translation(-2, 2, 0) |
    And s3 ← sphere() with:
      | transform | scaling(4, 4, 4) |
    And g ← group() of [s1, s2, s3]
  When divide(g, 1)
  Then g[0] = s3
    And subgroup ← g[1]
    And subgroup is a group
    And subgroup.count = 2
    And subgroup[0] is a group of [s1]
    And subgroup[1] is a group of [s2]
*/

TEST(GroupTest, ConfirmingSubdividingAGroupPartitionsItsChildren) {
    Sphere s1 {};
    s1.SetTransform(Transformation().Translate(-2, -2, 0));
    Sphere s2;
    s2.SetTransform(Transformation().Translate(-2, 2, 0));
    Sphere s3;
    s3.SetTransform(Transformation().Scale(4));
    ShapeGroup g {};
    g << &s1 << &s2 << &s3;
    g.Divide(1);
    ASSERT_EQ(g[0], &s3);
    ShapeGroup* subgroup_0 = static_cast<ShapeGroup*>(g[1]);
    ASSERT_EQ(subgroup_0->Size(), 2);
    ShapeGroup* subgroup_1 = static_cast<ShapeGroup*>((*subgroup_0)[0]);
    ASSERT_TRUE(subgroup_1->Contains(&s1));
    ShapeGroup* subgroup_2 = static_cast<ShapeGroup*>((*subgroup_0)[1]);
    ASSERT_TRUE(subgroup_2->Contains(&s2));
}

/*
Scenario: Subdividing a group with too few children
  Given s1 ← sphere() with:
      | transform | translation(-2, 0, 0) |
    And s2 ← sphere() with:
      | transform | translation(2, 1, 0) |
    And s3 ← sphere() with:
      | transform | translation(2, -1, 0) |
    And subgroup ← group() of [s1, s2, s3]
    And s4 ← sphere()
    And g ← group() of [subgroup, s4]
  When divide(g, 3)
  Then g[0] = subgroup
    And g[1] = s4
    And subgroup.count = 2
    And subgroup[0] is a group of [s1]
    And subgroup[1] is a group of [s2, s3]
*/

TEST(GroupTest, SubdividingAGroupWithTooFewChildren) {
    Sphere s1 {};
    s1.SetTransform(Transformation().Translate(-2, 0, 0));
    Sphere s2;
    s2.SetTransform(Transformation().Translate(2, 1, 0));
    Sphere s3;
    s3.SetTransform(Transformation().Translate(2, -1, 0));
    ShapeGroup subgroup {};
    subgroup << &s1 << &s2 << &s3;
    Sphere s4 {};
    ShapeGroup g {};
    g << &subgroup << &s4;
    g.Divide(3);
    ASSERT_EQ(g[0], &subgroup);
    ASSERT_EQ(g[1], &s4);
    ASSERT_EQ(subgroup.Size(), 2);
    ShapeGroup* subgroup_0 = static_cast<ShapeGroup*>(subgroup[0]);
    ASSERT_TRUE(subgroup_0->Contains(&s1));
    ShapeGroup* subgroup_1 = static_cast<ShapeGroup*>(subgroup[1]);
    ASSERT_TRUE(subgroup_1->Contains(&s2));
    ASSERT_TRUE(subgroup_1->Contains(&s3));
}

// For Issue ShapeGroup::Divide() drops shapes under certain conditions #1
TEST(GroupTest, ConfirmingTheBoundingBoxOfAGroupAfterAddingAShape) {
    ShapeGroup g {};
    Sphere s {};
    g.Add(&s);
    BoundingBox g_bbox = g.BoundsOfInParentSpace(),
                s_bbox = s.BoundsOfInParentSpace();
    ASSERT_EQ(s_bbox.Min(), g_bbox.Min());
    ASSERT_EQ(s_bbox.Max(), g_bbox.Max());
}

// For Issue ShapeGroup::Divide() drops shapes under certain conditions #1
TEST(GroupTest, DividingACubeOfSpheresDoesNotDropObjects) {
    std::vector<Shape *> objects;

    auto clean_up = [&objects] () {
        for (auto o: objects) {
            delete o;
        }
    };

    try
    {
        // Set up the BVH test, a 6*6*6 cube of spheres
        ShapeGroup shapes {};
        int dim { 6 };
        for (int y = 0; y < dim; y++) {
            for (int z = 0; z < dim; z++) {
                for (int x = 0; x < dim; x++) {
                    Sphere* s = new Sphere();
                    objects.push_back(s);
                    shapes << s;
                    s->SetTransform(
                        Transformation()
                        .Translate(2*x, 2*y, 2*z)
                    );
                }
            }
        }
        // Divide the group with a threshold <= (dim/2)^3
        int half_dim { dim / 2};
        shapes.Divide(half_dim*half_dim*half_dim);

        // Fire a ray at the sphere centered at the origin
        Ray r { Point { 0, 0, -5 }, Vector { 0, 0, 1 } };
        IntersectionList xs {};
        bool result = shapes.Intersect(xs, r);
        ASSERT_TRUE(result);
    }
    catch(const std::exception& e)
    {
        clean_up();
        throw e;
    }
    clean_up();
}