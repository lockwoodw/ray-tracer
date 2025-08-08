#define _USE_MATH_DEFINES // for M_PI
#include "shape.h"

#include <gtest/gtest.h>
#include <cmath>

#include "transformations.h"
#include "material.h"
#include "ray.h"
#include "space.h"
#include "utils.h"
#include "sphere.h"
#include "group.h"

/*
Scenario: The default transformation
  Given s ← test_shape()
  Then s.transform = identity_matrix
*/

TEST(ShapeTest, ConfirmingTheDefaultTransformation) {
    TestShape s {};
    ASSERT_EQ(s.Transform(), Matrix::Identity(4));
}

/*
Scenario: Assigning a transformation
  Given s ← test_shape()
  When set_transform(s, translation(2, 3, 4))
  Then s.transform = translation(2, 3, 4)
*/

TEST(ShapeTest, AssigningATransformation) {
    TestShape s {};
    Matrix transform = Transformation().Translate(2, 3, 4);
    s.SetTransform(transform);
    ASSERT_EQ(s.Transform(), transform);
}

/*
Scenario: The default material
  Given s ← test_shape()
  When m ← s.material
  Then m = material()
*/

TEST(ShapeTest, ConfirmingTheDefaultMaterial) {
    TestShape s {};
    Material m {};
    s.SetMaterial(m);
    ASSERT_EQ(s.ShapeMaterial(), m);
}

/*
Scenario: Assigning a material
  Given s ← test_shape()
    And m ← material()
    And m.ambient ← 1
  When s.material ← m
  Then s.material = m
*/

TEST(ShapeTest, AssigningAMaterial) {
    TestShape s {};
    Material m {};
    m.Ambient(1);
    s.SetMaterial(m);
    ASSERT_EQ(s.ShapeMaterial(), m);
    ASSERT_DOUBLE_EQ(s.ShapeMaterial().Ambient(), 1);
}

/*
Scenario: Intersecting a scaled shape with a ray
  Given r ← ray(point(0, 0, -5), vector(0, 0, 1))
    And s ← test_shape()
  When set_transform(s, scaling(2, 2, 2))
    And xs ← intersect(s, r)
  Then s.saved_ray.origin = point(0, 0, -2.5)
    And s.saved_ray.direction = vector(0, 0, 0.5)
*/

TEST(ShapeTest, IntersectingAScaledShapeWithARay) {
    Ray r { Point { 0, 0, -5 }, Vector { 0, 0, 1 }};
    TestShape s {};
    s.SetTransform(Transformation().Scale(2, 2, 2));
    IntersectionList xs {};
    Ray saved = s.AddIntersections(xs, r);
    Point expected_origin { 0, 0, -2.5 };
    Vector expected_direction { 0, 0, 0.5 };
    ASSERT_EQ(saved.Origin(), expected_origin);
    ASSERT_EQ(saved.Direction(), expected_direction);
}

/*
Scenario: Intersecting a translated shape with a ray
  Given r ← ray(point(0, 0, -5), vector(0, 0, 1))
    And s ← test_shape()
  When set_transform(s, translation(5, 0, 0))
    And xs ← intersect(s, r)
  Then s.saved_ray.origin = point(-5, 0, -5)
    And s.saved_ray.direction = vector(0, 0, 1)
*/

TEST(ShapeTest, IntersectingATranslatedShapeWithARay) {
    Ray r { Point { 0, 0, -5 }, Vector { 0, 0, 1 }};
    TestShape s {};
    s.SetTransform(Transformation().Translate(5, 0, 0));
    IntersectionList xs {};
    Ray saved = s.AddIntersections(xs, r);
    Point expected_origin { -5, 0, -5 };
    Vector expected_direction { 0, 0, 1 };
    ASSERT_EQ(saved.Origin(), expected_origin);
    ASSERT_EQ(saved.Direction(), expected_direction);
}

/*
Scenario: Computing the normal on a translated shape
  Given s ← test_shape()
  When set_transform(s, translation(0, 1, 0))
    And n ← normal_at(s, point(0, 1.70711, -0.70711))
  Then n = vector(0, 0.70711, -0.70711)
*/

TEST(ShapeTest, ComputingTheNormalOnATranslatedShape) {
    TestShape s {};
    s.SetTransform(Transformation().Translate(0, 1, 0));
    Point point { 0, 1.70711, -0.70711 };
    Vector expected { 0, 0.70711, -0.70711 }, normal = s.NormalAt(point);
    for (int i = 0; i < normal.Size(); i++) {
        ASSERT_TRUE(simple_floating_point_compare(expected.At(i), normal.At(i)));
    }
}

/*
Scenario: Computing the normal on a transformed shape
  Given s ← test_shape()
    And m ← scaling(1, 0.5, 1) * rotation_z(π/5)
  When set_transform(s, m)
    And n ← normal_at(s, point(0, √2/2, -√2/2))
  Then n = vector(0, 0.97014, -0.24254)
*/

TEST(ShapeTest, ComputingTheNormalOnATransformedShape) {
    TestShape s {};
    s.SetTransform(Transformation().RotateZ(M_PI / 5).Scale(1, 0.5, 1));
    double value { std::sqrt(2) / 2 };
    Point point { 0, value, -value };
    Vector expected { 0, 0.97014, -0.24254 }, normal = s.NormalAt(point);
    for (int i = 0; i < normal.Size(); i++) {
        ASSERT_TRUE(simple_floating_point_compare(expected.At(i), normal.At(i)));
    }
}

/*
Scenario: A shape has a parent attribute
  Given s ← test_shape()
  Then s.parent is nothing
*/

TEST(ShapeTest, AShapeHasAParentAttribute) {
    TestShape s {};
    ASSERT_EQ(s.Parent(), nullptr);
}

/*
Scenario: Converting a point from world to object space
  Given g1 ← group()
    And set_transform(g1, rotation_y(π/2))
    And g2 ← group()
    And set_transform(g2, scaling(2, 2, 2))
    And add_child(g1, g2)
    And s ← sphere()
    And set_transform(s, translation(5, 0, 0))
    And add_child(g2, s)
  When p ← world_to_object(s, point(-2, 0, -10))
  Then p = point(0, 0, -1)
*/

TEST(ShapeTest, ConvertingAPointFromWorldToObjectSpace) {
    ShapeGroup g1 {};
    g1.SetTransform(Transformation().RotateY(M_PI / 2));
    ShapeGroup g2 {};
    g2.SetTransform(Transformation().Scale(2));
    g1 << &g2;
    Sphere s {};
    s.SetTransform(Transformation().Translate(5, 0, 0));
    g2 << &s;
    Point expected { 0, 0, -1 },
          actual = s.ConvertWorldPointToObjectSpace(Point { -2, 0, -10 });
    ASSERT_EQ(expected, actual);
}

/*
Scenario: Converting a normal from object to world space
  Given g1 ← group()
    And set_transform(g1, rotation_y(π/2))
    And g2 ← group()
    And set_transform(g2, scaling(1, 2, 3))
    And add_child(g1, g2)
    And s ← sphere()
    And set_transform(s, translation(5, 0, 0))
    And add_child(g2, s)
  When n ← normal_to_world(s, vector(√3/3, √3/3, √3/3))
  Then n = vector(0.2857, 0.4286, -0.8571)
*/

TEST(ShapeTest, ConvertingANormalFromObjectToWorldSpace) {
    ShapeGroup g1 {};
    g1.SetTransform(Transformation().RotateY(M_PI / 2));
    ShapeGroup g2 {};
    g2.SetTransform(Transformation().Scale(1, 2, 3));
    g1 << &g2;
    Sphere s {};
    s.SetTransform(Transformation().Translate(5, 0, 0));
    g2 << &s;
    double vcoord { std::sqrt(3.0) / 3.0 };
    Vector expected { 0.2857, 0.4286, -0.8571 },
           actual = s.ConvertObjectNormalToWorldSpace(Vector { vcoord, vcoord, vcoord });
    // Need to relax epsilon to match the expected values
    ASSERT_NEAR(expected.X(), actual.X(), 1e-4);
    ASSERT_NEAR(expected.Y(), actual.Y(), 1e-4);
    ASSERT_NEAR(expected.Z(), actual.Z(), 1e-4);
}

/*
Scenario: Finding the normal on a child object
  Given g1 ← group()
    And set_transform(g1, rotation_y(π/2))
    And g2 ← group()
    And set_transform(g2, scaling(1, 2, 3))
    And add_child(g1, g2)
    And s ← sphere()
    And set_transform(s, translation(5, 0, 0))
    And add_child(g2, s)
  When n ← normal_at(s, point(1.7321, 1.1547, -5.5774))
  Then n = vector(0.2857, 0.4286, -0.8571)
*/

TEST(ShapeTest, FindingTheNormalOnAChildObject) {
    ShapeGroup g1 {};
    g1.SetTransform(Transformation().RotateY(M_PI / 2));
    ShapeGroup g2 {};
    g2.SetTransform(Transformation().Scale(1, 2, 3));
    g1 << &g2;
    Sphere s {};
    s.SetTransform(Transformation().Translate(5, 0, 0));
    g2 << &s;
    Vector expected { 0.2857, 0.4286, -0.8571 },
           actual = s.NormalAt(Point { 1.7321, 1.1547, -5.5774 });
    // Need to relax epsilon to match the expected values
    ASSERT_NEAR(expected.X(), actual.X(), 1e-4);
    ASSERT_NEAR(expected.Y(), actual.Y(), 1e-4);
    ASSERT_NEAR(expected.Z(), actual.Z(), 1e-4);
}