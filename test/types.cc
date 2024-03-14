#include <gtest/gtest.h>
#include "types.h"
#include <cmath>

TEST(TypesTest, EqualityOperator) {
    double x { 4.3 }, y { -4.2 }, z { 3.1 }, w { 1.0 };
    Tuple t1 { x, y, z, w }, t2 { x, y, z, w };
    ASSERT_EQ(t1, t2);
}

/*
Scenario: A tuple with w=1.0 is a point
Given   a <-- tuple(4.3, -4.2, 3.1, 1.0)
Then    a.x = 4.3
    And a.y = -4.2
    And a.z = 3.1
    And a.w = 1.0
    And a is a point
    And a is not a vector

Scenario: Point creates tuples with w = 1
Given   p <-- Point(4, -4, 3)
Then    p = Tuple(4, -4, 3, 1)
*/

TEST(TypesTest, IsAPoint) {
    double x { 4.3 }, y { -4.2 }, z { 3.1 }, w { 1.0 };
    Tuple t { x, y, z, w };
    ASSERT_DOUBLE_EQ(t.x_, x);
    ASSERT_DOUBLE_EQ(t.y_, y);
    ASSERT_DOUBLE_EQ(t.z_, z);
    ASSERT_DOUBLE_EQ(t.w_, w);
    Point p { x, y, z };
    ASSERT_EQ(t, p);
    Vector v { x, y, z };
    ASSERT_NE(t, v);
}

/*
Scenario: A tuple with w = 0 is a vector
Given   a <-- tuple(4.3, -4.2, 3.1, 1.0)
Then    a.x = 4.3
    And a.y = -4.2
    And a.z = 3.1
    And a.w = 0.0
    And a is not a point
    And a is a vector

Scenario: Vector creates tuples with w = 0
Given   v <-- Vector(4, -4, 3)
Then    v = Tuple(4, -4, 3, 0)
*/

TEST(TypesTest, IsAVector) {
    double x { 4.3 }, y { -4.2 }, z { 3.1 }, w { 0.0 };
    Tuple t { x, y, z, w };
    ASSERT_DOUBLE_EQ(t.x_, x);
    ASSERT_DOUBLE_EQ(t.y_, y);
    ASSERT_DOUBLE_EQ(t.z_, z);
    ASSERT_DOUBLE_EQ(t.w_, w);
    Vector v { x, y, z };
    ASSERT_EQ(t, v);
    Point p { x, y, z };
    ASSERT_NE(t, p);
}

/*
Scenario: Adding two tuples
Given   a1 <-- Tuple(3, -2, 5, 1)
    And a2 <-- Tuple(-2, 3, 1, 0)
Then    a1 + a2 = Tuple(1, 1, 6, 1)
*/

TEST(TypesTest, AddingTwoTuples) {
    Tuple a1 { 3, -2, 5, 1 }, a2 { -2, 3, 1, 0 }, expected { 1, 1, 6, 1 };
    Tuple sum = a1 + a2;
    ASSERT_EQ(sum, expected);
}

TEST(TypesTest, AddingTwoPoints) {
    Point p1 { 3, 2, 1 }, p2 { 5, 6, 7 };
    ASSERT_ANY_THROW(p1 + p2);
}

/*
Scenario: Subtracting two points
Given   p1 <-- Point(3, 2, 1)
    And p2 <-- Point(5, 6, 7)
Then    p1 - p2 = Vector(-2, -4, -6)
*/

TEST(TypesTest, SubtractingTwoPoints) {
    Point p1 { 3, 2, 1 }, p2 { 5, 6, 7 };
    Vector expected { -2, -4, -6 };
    Tuple difference = p1 - p2;
    ASSERT_EQ(difference, expected);
}

/*
Scenario: Subtracting a vector from a point
Given   p <-- Point(3, 2, 1)
    And v <-- Vector(5, 6, 7)
Then    p - v = Point(-2, -4, -6)
*/

TEST(TypesTest, SubtractingAVectorFromAPoint) {
    Point p { 3, 2, 1 }, expected { -2, -4, -6 } ;
    Vector v { 5, 6, 7 };
    Tuple difference = p - v;
    ASSERT_EQ(difference, expected);
}

/*
Scenario: Subtracting two vectors
Given   v1 <-- Vector(3, 2, 1)
    And v2 <-- Vector(5, 6, 7)
Then    v1 - v2 = Vector(-2, -4, -6)
*/

TEST(TypesTest, SubtractingTwoVectors) {
    Vector v1 { 3, 2, 1 }, v2 { 5, 6, 7 }, expected { -2, -4, -6 } ;
    Tuple difference = v1 - v2;
    ASSERT_EQ(difference, expected);
}

TEST(TypesTest, SubtractingAPointFromAVector) {
    Vector v { 3, 2, 1 };
    Point p { 5, 6, 7 };
    ASSERT_ANY_THROW(v - p);
}

/*
Scenario: Subtracting a vector from the zero vector
  Given zero ← vector(0, 0, 0)
    And v ← vector(1, -2, 3)
  Then zero - v = vector(-1, 2, -3)
*/

TEST(TypesTest, SubtractingAVectorFromTheZeroVector) {
    Vector zero { 0, 0, 0 }, v { 1, -2, 3 }, expected { -1, 2, -3 };
    ASSERT_EQ(zero - v, expected);
}

/*
Scenario: Negating a tuple
  Given a ← tuple(1, -2, 3, -4)
  Then -a = tuple(-1, 2, -3, 4)
*/

TEST(TypesTest, NegatingATuple) {
    Tuple a { 1, -2, 3, -4 }, expected { -1, 2, -3, 4 };
    ASSERT_EQ(-a, expected);
}

/*
Scenario: Multiplying a tuple by a scalar
  Given a ← tuple(1, -2, 3, -4)
  Then a * 3.5 = tuple(3.5, -7, 10.5, -14)
*/

TEST(TypesTest, MultiplyingATupleByAScalar) {
    Tuple a { 1, -2, 3, -4 }, expected { 3.5, -7, 10.5, -14 };
    double scale { 3.5 };
    ASSERT_EQ(a * scale, expected);
}

/*
Scenario: Multiplying a tuple by a fraction
  Given a ← tuple(1, -2, 3, -4)
  Then a * 0.5 = tuple(0.5, -1, 1.5, -2)
*/

TEST(TypesTest, MultiplyingATupleByAFraction) {
    Tuple a { 1, -2, 3, -4 }, expected { 0.5, -1, 1.5, -2 };
    double scale { 0.5 };
    ASSERT_EQ(a * scale, expected);
}

/*Scenario: Dividing a tuple by a scalar
  Given a ← tuple(1, -2, 3, -4)
  Then a / 2 = tuple(0.5, -1, 1.5, -2)
*/

TEST(TypesTest, DividingATupleByAScalar) {
    Tuple a { 1, -2, 3, -4 }, expected { 0.5, -1, 1.5, -2 };
    double divisor { 2.0 };
    ASSERT_EQ(a / divisor, expected);
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

TEST(TypesTest, ComputingMagnitudeOfVectors) {
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

TEST(TypesTest, NormalizingVectors) {
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

TEST(TypesTest, ComputingMagnitudeOfNormalizedVector) {
    Vector v { 1, 2, 3 };
    Tuple t = v.Normalize();
    ASSERT_EQ(t.Magnitude(), 1.0);
}

/*
Scenario: The dot product of two tuples
  Given a ← vector(1, 2, 3)
    And b ← vector(2, 3, 4)
  Then dot(a, b) = 20
*/

TEST(TypesTest, ComputingDotProductOfVectors) {
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

TEST(TypesTest, ComputingCrossProductOfVectors) {
    Vector a { 1, 2, 3 }, b { 2, 3, 4 }, ab_product { -1, 2, -1 }, ba_product { 1, -2, 1 };
    ASSERT_EQ(Vector::CrossProduct(a, b), ab_product);
    ASSERT_EQ(Vector::CrossProduct(b, a), ba_product);
}
