#define _USE_MATH_DEFINES // for M_PI
#include <gtest/gtest.h>
#include <cmath>
#include "ray.h"
#include "sphere.h"
#include "transformations.h"

TEST(SphereTest, ComparingSpheresThroughBaseClass) {
    Point origin1 { 0, 0, -5 },
          origin2 { 0, 0, 0 };
    Sphere sphere1 { origin1, 1.0 },
           sphere2 { origin2, 1.0 },
           sphere3 { origin1, 2.0 },
           sphere4 { origin1, 1.0 };
    Shape* shape1 = &sphere1;
    Shape* shape2 = &sphere2;
    Shape* shape3 = &sphere3;
    Shape* shape4 = &sphere4;
    ASSERT_NE(*shape1, *shape2);
    ASSERT_NE(*shape1, *shape3);
    ASSERT_EQ(*shape1, *shape4);
}

TEST(SphereTest, ConstructingASphereWithTheDefaultConstructor) {
    Point origin { 0, 0, 0 };
    double radius { 1.0 };
    Sphere s { };
    ASSERT_EQ(s.Origin(), origin);
    ASSERT_DOUBLE_EQ(s.Radius(), radius);
}

TEST(SphereTest, ConstructingASphereWithAlternativeConstructor) {
    Point origin { 1, 1, 1 };
    double radius { 10.0 };
    Sphere s { origin, radius };
    ASSERT_EQ(s.Origin(), origin);
    ASSERT_DOUBLE_EQ(s.Radius(), radius);
}

/*
Scenario: A ray intersects a sphere at two points
  Given r ← ray(point(0, 0, -5), vector(0, 0, 1))
    And s ← sphere()
  When xs ← intersect(s, r)
  Then xs.count = 2
    And xs[0] = 4.0
    And xs[1] = 6.0
*/

TEST(SphereTest, IntersectingAUnitSphereAtTwoPoints) {
    Point ray_origin { 0, 0, -5 };
    Vector direction { 0, 0, 1 };
    Ray r { ray_origin, direction };
    Sphere s { };
    IntersectionList xs {};
    s.AddIntersections(xs, r);
    ASSERT_EQ(xs.Size(), 2);
    ASSERT_DOUBLE_EQ(xs[0]->Distance(), 4.0);
    ASSERT_DOUBLE_EQ(xs[1]->Distance(), 6.0);
}

TEST(SphereTest, IntersectingANonUnitSphereAtTwoPoints) {
    Point ray_origin { 0, 0, -5 },
          sphere_origin { 0, 0, 0 };
    Vector direction { 0, 0, 1 };
    Ray r { ray_origin, direction };
    double radius { 2.5 };
    Sphere s { sphere_origin, radius };
    IntersectionList xs {};
    s.AddIntersections(xs, r);
    ASSERT_EQ(xs.Size(), 2);
    ASSERT_DOUBLE_EQ(xs[0]->Distance(), 2.5);
    ASSERT_DOUBLE_EQ(xs[1]->Distance(), 7.5);
}

/*
Scenario: A ray intersects a sphere at a tangent
  Given r ← ray(point(0, 1, -5), vector(0, 0, 1))
    And s ← sphere()
  When xs ← intersect(s, r)
  Then xs.count = 2
    And xs[0] = 5.0
    And xs[1] = 5.0
*/

TEST(SphereTest, IntersectingASphereAtATangent) {
    Point ray_origin { 0, 1, -5 };
    Vector direction { 0, 0, 1 };
    Ray r { ray_origin, direction };
    Sphere s { };
    IntersectionList xs {};
    s.AddIntersections(xs, r);
    ASSERT_EQ(xs.Size(), 2);
    ASSERT_DOUBLE_EQ(xs[0]->Distance(), 5.0);
    ASSERT_DOUBLE_EQ(xs[1]->Distance(), 5.0);
}

/*
Scenario: A ray misses a sphere
  Given r ← ray(point(0, 2, -5), vector(0, 0, 1))
    And s ← sphere()
  When xs ← intersect(s, r)
  Then xs.count = 0
*/

TEST(SphereTest, NotIntersectingASphere) {
    Point ray_origin { 0, 2, -5 };
    Vector direction { 0, 0, 1 };
    Ray r { ray_origin, direction };
    Sphere s { };
    IntersectionList xs {};
    s.AddIntersections(xs, r);
    ASSERT_EQ(xs.Size(), 0);
}

/*
Scenario: A ray originates inside a sphere
  Given r ← ray(point(0, 0, 0), vector(0, 0, 1))
    And s ← sphere()
  When xs ← intersect(s, r)
  Then xs.count = 2
    And xs[0] = -1.0
    And xs[1] = 1.0
*/

TEST(SphereTest, IntersectingASphereFromInside) {
    Point origin { 0, 0, 0 };
    Vector direction { 0, 0, 1 };
    Ray r { origin, direction };
    Sphere s { };
    IntersectionList xs {};
    s.AddIntersections(xs, r);
    ASSERT_EQ(xs.Size(), 2);
    ASSERT_DOUBLE_EQ(xs[0]->Distance(), -1.0);
    ASSERT_DOUBLE_EQ(xs[1]->Distance(), 1.0);
}

/*
Scenario: A sphere is behind a ray
  Given r ← ray(point(0, 0, 5), vector(0, 0, 1))
    And s ← sphere()
  When xs ← intersect(s, r)
  Then xs.count = 2
    And xs[0] = -6.0
    And xs[1] = -4.0
*/

TEST(SphereTest, IntersectingASphereBehindARay) {
    Point ray_origin { 0, 0, 5 };
    Vector direction { 0, 0, 1 };
    Ray r { ray_origin, direction };
    Sphere s { };
    IntersectionList xs {};
    s.AddIntersections(xs, r);
    ASSERT_EQ(xs.Size(), 2);
    ASSERT_DOUBLE_EQ(xs[0]->Distance(), -6.0);
    ASSERT_DOUBLE_EQ(xs[1]->Distance(), -4.0);
}

/*
Scenario: Intersect sets the object on the intersection
  Given r ← ray(point(0, 0, -5), vector(0, 0, 1))
    And s ← sphere()
  When xs ← intersect(s, r)
  Then xs.count = 2
    And xs[0].object = s
    And xs[1].object = s
*/

TEST(SphereTest, IncludingTheObjectInTheIntersection) {
    Point ray_origin { 0, 0, -5 };
    Vector direction { 0, 0, 1 };
    Ray r { ray_origin, direction };
    Sphere s { };
    IntersectionList xs {};
    s.AddIntersections(xs, r);
    ASSERT_EQ(xs.Size(), 2);
    ASSERT_EQ(*xs[0]->Object(), s);
    ASSERT_EQ(*xs[1]->Object(), s);
}

/*
Scenario: A sphere's default transformation
  Given s ← sphere()
  Then s.transform = identity_matrix
*/

TEST(SphereTest, ConfirmingDefaultTransformation) {
    Sphere s { };
    ASSERT_EQ(s.Transform(), Matrix::Identity(4));
}

/*
Scenario: Changing a sphere's transformation
  Given s ← sphere()
    And t ← translation(2, 3, 4)
  When set_transform(s, t)
  Then s.transform = t
*/

TEST(SphereTest, ChangingTheTransformation) {
    Sphere s { };
    Matrix t = Transformation().Translate(2, 3, 4);
    s.SetTransform(t);
    ASSERT_EQ(s.Transform(), t);
}

/*
Scenario: Intersecting a scaled sphere with a ray
  Given r ← ray(point(0, 0, -5), vector(0, 0, 1))
    And s ← sphere()
  When set_transform(s, scaling(2, 2, 2))
    And xs ← intersect(s, r)
  Then xs.count = 2
    And xs[0].t = 3
    And xs[1].t = 7
*/

TEST(SphereTest, IntersectingASphereWithAScaledRay) {
    Point ray_origin { 0, 0, -5 };
    Vector direction { 0, 0, 1 };
    Ray r { ray_origin, direction };
    Sphere s { };
    Matrix t = Transformation().Scale(2, 2, 2);
    s.SetTransform(t);
    IntersectionList xs {};
    s.AddIntersections(xs, r);
    ASSERT_EQ(xs.Size(), 2);
    ASSERT_EQ(xs[0]->Distance(), 3);
    ASSERT_EQ(xs[1]->Distance(), 7);
}

/*
Scenario: Intersecting a translated sphere with a ray
  Given r ← ray(point(0, 0, -5), vector(0, 0, 1))
    And s ← sphere()
  When set_transform(s, translation(5, 0, 0))
    And xs ← intersect(s, r)
  Then xs.count = 0
*/

TEST(SphereTest, IntersectingATranslatedSphereWithARay) {
    Point ray_origin { 0, 0, -5 };
    Vector direction { 0, 0, 1 };
    Ray r { ray_origin, direction };
    Sphere s { };
    Matrix t = Transformation().Translate(5, 0, 0);
    s.SetTransform(t);
    IntersectionList xs {};
    s.AddIntersections(xs, r);
    ASSERT_EQ(xs.Size(), 0);
}

/*
Scenario: The normal on a sphere at a point on the x axis
  Given s ← sphere()
  When n ← normal_at(s, point(1, 0, 0))
  Then n = vector(1, 0, 0)
*/

TEST(SphereTest, FindingTheNormalOfASphereAtAPointOnTheXAxis) {
    Sphere s {};
    Point px { 1, 0, 0 };
    Vector expected { 1, 0, 0 },
           n = s.NormalAt(px);
    ASSERT_EQ(n, expected);
}

/*
Scenario: The normal on a sphere at a point on the y axis
  Given s ← sphere()
  When n ← normal_at(s, point(0, 1, 0))
  Then n = vector(0, 1, 0)
*/

TEST(SphereTest, FindingTheNormalOfASphereAtAPointOnTheYAxis) {
    Sphere s {};
    Point py { 0, 1, 0 };
    Vector expected { 0, 1, 0 },
           n = s.NormalAt(py);
    ASSERT_EQ(n, expected);
}

/*
Scenario: The normal on a sphere at a point on the z axis
  Given s ← sphere()
  When n ← normal_at(s, point(0, 0, 1))
  Then n = vector(0, 0, 1)
*/

TEST(SphereTest, FindingTheNormalOfASphereAtAPointOnTheZAxis) {
    Sphere s {};
    Point pz { 0, 0, 1 };
    Vector expected { 0, 0, 1 },
           n = s.NormalAt(pz);
    ASSERT_EQ(n, expected);
}

/*
Scenario: The normal on a sphere at a nonaxial point
  Given s ← sphere()
  When n ← normal_at(s, point(√3/3, √3/3, √3/3))
  Then n = vector(√3/3, √3/3, √3/3)
*/

TEST(SphereTest, FindingTheNormalOfASphereAtANonaxialPoint) {
    Sphere s {};
    double d { std::sqrt(3) /  3 };
    Point p { d, d, d };
    Vector expected { d, d, d },
           n = s.NormalAt(p);
    ASSERT_EQ(n, expected);
}

/*
Scenario: The normal is a normalized vector
  Given s ← sphere()
  When n ← normal_at(s, point(√3/3, √3/3, √3/3))
  Then n = normalize(n)
*/

TEST(SphereTest, ConfirmingTheNormalIsANormalizedVector) {
    Sphere s {};
    double d { std::sqrt(3) /  3 };
    Point p { d, d, d };
    Vector n = s.NormalAt(p);
    ASSERT_EQ(n, n.Normalize());
}

/*
Scenario: Computing the normal on a translated sphere
  Given s ← sphere()
    And set_transform(s, translation(0, 1, 0))
  When n ← normal_at(s, point(0, 1.70711, -0.70711))
  Then n = vector(0, 0.70711, -0.70711)
*/

TEST(SphereTest, ConfirmingTheNormalOnATranslatedSphere) {
    Sphere s {};
    Matrix transform = Transformation().Translate(0, 1, 0);
    s.SetTransform(transform);
    Point p { 0, 1.70711, -0.70711 };
    Vector expected { 0, 0.70711, -0.70711 },
           n = s.NormalAt(p);
    // Compare elements individually using a comparator with smaller Epsilon
    ASSERT_EQ(expected.Size(), n.Size());
    for (int i = 0; i < expected.Size(); i++) {
        ASSERT_TRUE(simple_floating_point_compare(expected[i], n[i]));
    }
}

/*
Scenario: Computing the normal on a transformed sphere
  Given s ← sphere()
    And m ← scaling(1, 0.5, 1) * rotation_z(π/5)
    And set_transform(s, m)
  When n ← normal_at(s, point(0, √2/2, -√2/2))
  Then n = vector(0, 0.97014, -0.24254)
*/

TEST(SphereTest, ConfirmingTheNormalOnATransformedSphere) {
    Sphere s {};
    Matrix transform = Transformation().RotateZ(M_PI / 5).Scale(1, 0.5, 1);
    s.SetTransform(transform);
    double d = sqrt(2) / 2;
    Point p { 0, d, -d };
    Vector expected { 0, 0.97014, -0.24254 },
           n = s.NormalAt(p);
    // Compare elements individually using a comparator with smaller Epsilon
    ASSERT_EQ(expected.Size(), n.Size());
    for (int i = 0; i < expected.Size(); i++) {
        ASSERT_TRUE(simple_floating_point_compare(expected[i], n[i]));
    }
}

/*
Scenario: A sphere has a default material
  Given s ← sphere()
  When m ← s.material
  Then m = material()
*/

TEST(SphereTest, ConfirmingTheDefaultMaterialOfASphere) {
    Sphere s {};
    Material m = s.ShapeMaterial();
    ASSERT_EQ(m, Material());
}

/*
Scenario: A sphere may be assigned a material
  Given s ← sphere()
    And m ← material()
    And m.ambient ← 1
  When s.material ← m
  Then s.material = m
*/

TEST(SphereTest, AssigningAMaterialToASphere) {
    Sphere s {};
    Material m {};
    m.Ambient(1);
    s.SetMaterial(m);
    ASSERT_DOUBLE_EQ(s.ShapeMaterial().Ambient(), 1);
    ASSERT_EQ(m, s.ShapeMaterial());
}

/*
Scenario: A helper for producing a sphere with a glassy material
  Given s ← glass_sphere()
  Then s.transform = identity_matrix
    And s.material.transparency = 1.0
    And s.material.refractive_index = 1.5
*/