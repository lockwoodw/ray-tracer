#include <gtest/gtest.h>
#include "tuple.h"

TEST(TupleTest, EqualityOperator) {
    std::array<double, 4> src { 4.3, -4.2, 3.1, 1.0 };
    Tuple t1 { src },
          t2 { src };
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
*/

bool is_a_point(const Tuple& t) {
    return (t.Size() == 4 && t.At(3) == 1.0);
}

bool is_a_vector(const Tuple& t) {
    return (t.Size() == 4 && t.At(3) == 0.0);
}

TEST(TupleTest, IsAPoint) {
    double src[4] { 4.3, -4.2, 3.1, 1.0 };
    std::size_t n = sizeof(src) /  sizeof(double);
    Tuple t { n, src };
    for (int i = 0; i < n; i++) {
        ASSERT_DOUBLE_EQ(t[i], src[i]);
    }
    ASSERT_TRUE(is_a_point(t));
    ASSERT_FALSE(is_a_vector(t));
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
*/

TEST(TupleTest, IsAVector) {
    std::array<double, 4> src { 4.3, -4.2, 3.1, 0.0 };
    Tuple t { src };
    for (int i = 0; i < src.size(); i++) {
        ASSERT_DOUBLE_EQ(t[i], src[i]);
    }
    ASSERT_FALSE(is_a_point(t));
    ASSERT_TRUE(is_a_vector(t));
}

/*
Scenario: Adding two tuples
Given   a1 <-- Tuple(3, -2, 5, 1)
    And a2 <-- Tuple(-2, 3, 1, 0)
Then    a1 + a2 = Tuple(1, 1, 6, 1)
*/

TEST(TupleTest, AddingTwoTuples) {
    std::array<double, 4> a1 { 3, -2, 5, 1 },
                          a2 { -2, 3, 1, 0 },
                          expected { 1, 1, 6, 1 };
    Tuple ta1 { a1 },
          ta2 { a2 },
          sum { expected };
    ASSERT_EQ(ta1 + ta2, sum);
}

/*
Scenario: Subtracting two points
Given   p1 <-- Point(3, 2, 1)
    And p2 <-- Point(5, 6, 7)
Then    p1 - p2 = Vector(-2, -4, -6)
*/

// factory methods to simplify things
Tuple get_vector(double x, double y, double z) {
    return get_4_tuple(x, y, z, 0);
}

Tuple get_point(double x, double y, double z) {
    return get_4_tuple(x, y, z, 1.0);
}

TEST(TupleTest, SubtractingTwoPoints) {
    Tuple p1 = get_point(3, 2, 1),
          p2 = get_point(5, 6, 7),
          expected = get_vector(-2, -4, -6),
          difference = p1 - p2;
    ASSERT_EQ(difference, expected);
    ASSERT_TRUE(is_a_vector(difference));
}

/*
Scenario: Subtracting a vector from a point
Given   p <-- Point(3, 2, 1)
    And v <-- Vector(5, 6, 7)
Then    p - v = Point(-2, -4, -6)
*/

TEST(TupleTest, SubtractingAVectorFromAPoint) {
    Tuple p = get_point(3, 2, 1),
          expected = get_point(-2, -4, -6),
          v = get_vector(5, 6, 7),
          difference = p - v;
    ASSERT_EQ(difference, expected);
    ASSERT_TRUE(is_a_point(difference));
}

/*
Scenario: Subtracting two vectors
Given   v1 <-- Vector(3, 2, 1)
    And v2 <-- Vector(5, 6, 7)
Then    v1 - v2 = Vector(-2, -4, -6)
*/

TEST(TupleTest, SubtractingTwoVectors) {
    Tuple v1 = get_vector(3, 2, 1),
          v2 = get_vector(5, 6, 7),
          expected = get_vector(-2, -4, -6),
          difference = v1 - v2;
    ASSERT_EQ(difference, expected);
    ASSERT_TRUE(is_a_vector(difference));
}

/*
Scenario: Subtracting a vector from the zero vector
  Given zero ← vector(0, 0, 0)
    And v ← vector(1, -2, 3)
  Then zero - v = vector(-1, 2, -3)
*/

TEST(TupleTest, SubtractingAVectorFromTheZeroVector) {
    Tuple zero = get_vector(0, 0, 0),
          v = get_vector(1, -2, 3),
          expected = get_vector(-1, 2, -3);
    ASSERT_EQ(zero - v, expected);
}

/*
Scenario: Negating a tuple
  Given a ← tuple(1, -2, 3, -4)
  Then -a = tuple(-1, 2, -3, 4)
*/

TEST(TupleTest, NegatingATuple) {
    Tuple a = get_4_tuple(1, -2, 3, -4),
          expected = get_4_tuple(-1, 2, -3, 4);
    ASSERT_EQ(-a, expected);
}

/*
Scenario: Multiplying a tuple by a scalar
  Given a ← tuple(1, -2, 3, -4)
  Then a * 3.5 = tuple(3.5, -7, 10.5, -14)
*/

TEST(TupleTest, MultiplyingATupleByAScalar) {
    Tuple a = get_4_tuple(1, -2, 3, -4),
          expected = get_4_tuple(3.5, -7, 10.5, -14);
    double scale { 3.5 };
    ASSERT_EQ(a * scale, expected);
}

/*
Scenario: Multiplying a tuple by a fraction
  Given a ← tuple(1, -2, 3, -4)
  Then a * 0.5 = tuple(0.5, -1, 1.5, -2)
*/

TEST(TupleTest, MultiplyingATupleByAFraction) {
    Tuple a = get_4_tuple(1, -2, 3, -4),
          expected = get_4_tuple(0.5, -1, 1.5, -2);
    double scale { 0.5 };
    ASSERT_EQ(a * scale, expected);
}

/*Scenario: Dividing a tuple by a scalar
  Given a ← tuple(1, -2, 3, -4)
  Then a / 2 = tuple(0.5, -1, 1.5, -2)
*/

TEST(TupleTest, DividingATupleByAScalar) {
    Tuple a = get_4_tuple(1, -2, 3, -4),
          expected = get_4_tuple(0.5, -1, 1.5, -2);
    double divisor { 2.0 };
    ASSERT_EQ(a / divisor, expected);
}