#include <gtest/gtest.h>
#include <cmath>
#include "space.h"

/*
Scenario: Point creates tuples with w = 1
Given   p <-- Point(4, -4, 3)
Then    p = Tuple(4, -4, 3, 1)
*/

TEST(SpaceTest, IsAPoint) {
    double x { 4.3 }, y { -4.2 }, z { 3.1 }, w { 1.0 };
    Point p { x, y, z };
    Tuple t = get_4_tuple(x, y, z, w);
    ASSERT_EQ(p, t);
    Vector v { x, y, z };
    ASSERT_NE(v, t);
}

/*
Scenario: Vector creates tuples with w = 0
Given   v <-- Vector(4, -4, 3)
Then    v = Tuple(4, -4, 3, 0)
*/

TEST(SpaceTest, IsAVector) {
    double x { 4.3 }, y { -4.2 }, z { 3.1 }, w { 0.0 };
    Vector v { x, y, z };
    Tuple t = get_4_tuple(x, y, z, w);
    ASSERT_EQ(v, t);
    Point p { x, y, z };
    ASSERT_NE(p, t);
}

/*
Scenario: Subtracting two points
Given   p1 <-- Point(3, 2, 1)
    And p2 <-- Point(5, 6, 7)
Then    p1 - p2 = Vector(-2, -4, -6)
*/

TEST(SpaceTest, SubtractingTwoPoints) {
    Point p1 { 3, 2, 1 }, p2 { 5, 6, 7 };
    Vector expected { -2, -4, -6 }, difference = p1 - p2;
    ASSERT_EQ(difference, expected);
}

/*
Scenario: Subtracting a vector from a point
Given   p <-- Point(3, 2, 1)
    And v <-- Vector(5, 6, 7)
Then    p - v = Point(-2, -4, -6)
*/

TEST(SpaceTest, SubtractingAVectorFromAPoint) {
    Vector v { 5, 6, 7 };
    Point p { 3, 2, 1 }, expected { -2, -4, -6 }, difference = p - v;
    ASSERT_EQ(difference, expected);
}

/*
Scenario: Subtracting two vectors
Given   v1 <-- Vector(3, 2, 1)
    And v2 <-- Vector(5, 6, 7)
Then    v1 - v2 = Vector(-2, -4, -6)
*/

TEST(SpaceTest, SubtractingTwoVectors) {
    Vector v1 { 3, 2, 1 }, v2 { 5, 6, 7 }, expected { -2, -4, -6 }, difference = v1 - v2;
    ASSERT_EQ(difference, expected);
}

/*
Scenario: Subtracting a vector from the zero vector
  Given zero ← vector(0, 0, 0)
    And v ← vector(1, -2, 3)
  Then zero - v = vector(-1, 2, -3)
*/

TEST(SpaceTest, SubtractingAVectorFromTheZeroVector) {
    Vector zero { 0, 0, 0 }, v { 1, -2, 3 }, expected { -1, 2, -3 };
    ASSERT_EQ(zero - v, expected);
}

/*
Scenario: Computing the magnitude of vector(1, 0, 0)
  Given v ← vector(1, 0, 0)
  Then magnitude(v) = 1

Scenario: Computing the magnitude of vector(0, 1, 0)
  Given v ← vector(0, 1, 0)
  Then magnitude(v) = 1

Scenario: Computing the magnitude of vector(0, 0, 1)
  Given v ← vector(0, 0, 1)
  Then magnitude(v) = 1

Scenario: Computing the magnitude of vector(1, 2, 3)
  Given v ← vector(1, 2, 3)
  Then magnitude(v) = √14

Scenario: Computing the magnitude of vector(-1, -2, -3)
  Given v ← vector(-1, -2, -3)
  Then magnitude(v) = √14
*/

TEST(TupleTest, ComputingMagnitudeOfVectors) {
    Vector v1 { 1, 0, 0 };
    double expected { 1.0 };
    ASSERT_EQ(v1.Magnitude(), expected);
    Vector v2 { 0, 1, 0 };
    ASSERT_EQ(v2.Magnitude(), expected);
    Vector v3 { 0, 0, 1 };
    ASSERT_EQ(v3.Magnitude(), expected);
    Vector v4 { 1, 2, 3 };
    expected = std::sqrt(14);
    ASSERT_EQ(v4.Magnitude(), expected);
    Vector v5 { -1, -2, -3 };
    ASSERT_EQ(v5.Magnitude(), expected);
}

/*
Scenario: Normalizing vector(4, 0, 0) gives (1, 0, 0)
  Given v ← vector(4, 0, 0)
  Then normalize(v) = vector(1, 0, 0)

Scenario: Normalizing vector(1, 2, 3)
  Given v ← vector(1, 2, 3)
                                  # vector(1/√14,   2/√14,   3/√14)
  Then normalize(v) = approximately vector(0.26726, 0.53452, 0.80178)
*/

TEST(TupleTest, NormalizingVectors) {
    Vector v1 { 4, 0, 0 }, n1 { 1, 0, 0 };
    ASSERT_EQ(v1.Normalize(), n1);
    Vector v2 { 1, 2, 3 };
    double v2_magnitude = v2.Magnitude();
    Vector n2 { 1 / v2_magnitude, 2 / v2_magnitude, 3 / v2_magnitude };
    ASSERT_EQ(v2.Normalize(), n2);
}

/*
Scenario: The magnitude of a normalized vector
  Given v ← vector(1, 2, 3)
  When norm ← normalize(v)
  Then magnitude(norm) = 1
*/

TEST(TupleTest, ComputingMagnitudeOfNormalizedVector) {
    Vector v { 1, 2, 3 }, normalized = v.Normalize();
    ASSERT_EQ(normalized.Magnitude(), 1.0);
}

/*
Scenario: The dot product of two tuples
  Given a ← vector(1, 2, 3)
    And b ← vector(2, 3, 4)
  Then dot(a, b) = 20
*/

TEST(TupleTest, ComputingDotProductOfVectors) {
    Vector a { 1, 2, 3 }, b { 2, 3, 4 };
    ASSERT_EQ(Vector::DotProduct(a, b), 20.0);
}

/*
Scenario: The cross product of two vectors
  Given a ← vector(1, 2, 3)
    And b ← vector(2, 3, 4)
  Then cross(a, b) = vector(-1, 2, -1)
    And cross(b, a) = vector(1, -2, 1)
*/

TEST(TupleTest, ComputingCrossProductOfVectors) {
    Vector a { 1, 2, 3 }, b { 2, 3, 4 }, ab_product { -1, 2, -1 }, ba_product { 1, -2, 1 };
    ASSERT_EQ(Vector::CrossProduct(a, b), ab_product);
    ASSERT_EQ(Vector::CrossProduct(b, a), ba_product);
}