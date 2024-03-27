#define _USE_MATH_DEFINES // for M_PI
#include <gtest/gtest.h>
#include <array>
#include <cmath>
#include "transformations.h"
#include "space.h"

/*
Scenario: Multiplying by a translation matrix
  Given transform ← translation(5, -3, 2)
    And p ← point(-3, 4, 5)
   Then transform * p = point(2, 1, 7)
*/

TEST(TransformationsTest, MultiplyingByATranslationMatrix) {
    std::array<double, 3> data { 5, -3, 2 };
    Translation transform { data };
    Point p { -3, 4, 5 }, expected { 2, 1, 7 };
    ASSERT_EQ(transform * p, expected);
}

TEST(TransformationsTest, MultiplyingByA3DTranslationMatrix) {
    std::array<double, 2> data { 10, -10 };
    Translation transform { data };
    double src[] { 2, 4, 1 }, translated[] { 12, -6, 1 };
    Tuple t { 3, src }, expected { 3, translated };
    ASSERT_EQ(transform * t, expected);
}

/*
Scenario: Multiplying by the inverse of a translation matrix
  Given transform ← translation(5, -3, 2)
    And inv ← inverse(transform)
    And p ← point(-3, 4, 5)
   Then inv * p = point(-8, 7, 3)
*/

TEST(TransformationsTest, MultiplyingByTheInverseOfATranslationMatrix) {
    // use alternate constructor
    double data[] { 5, -3, 2 };
    Translation transform { sizeof(data) / sizeof(double), data };
    Matrix inverse = transform.Inverse();
    Point p { -3, 4, 5 }, expected { -8, 7, 3 };
    ASSERT_EQ(inverse * p, expected);
}

/*
Scenario: Translation does not affect vectors
  Given transform ← translation(5, -3, 2)
    And v ← vector(-3, 4, 5)
   Then transform * v = v
*/

TEST(TransformationsTest, MultiplyingAVectorByATranslationMatrix) {
    std::array<double, 3> data { 5, -3, 2 };
    Translation transform { data };
    Vector v { -3, 4, 5 };
    ASSERT_EQ(transform * v, v);
}

/*
Scenario: A scaling matrix applied to a point
  Given transform ← scaling(2, 3, 4)
    And p ← point(-4, 6, 8)
   Then transform * p = point(-8, 18, 32)
*/

TEST(TransformationsTest, ApplyingAScalingMatrixToAPoint) {
    std::array<double, 3> data { 2, 3, 4 };
    Scaling transform { data };
    Point p { -4, 6, 8 }, expected { -8, 18, 32 };
    ASSERT_EQ(transform * p, expected);
}

TEST(TransformationsTest, ApplyingA3DScalingMatrix) {
    std::array<double, 2> data { 10, -10 };
    Scaling transform { data };
    double src[] { -4, 6, 1 }, scaled[] { -40, -60, 1 };
    Tuple t { 3, src }, expected { 3, scaled };
    ASSERT_EQ(transform * t, expected);
}

/*
Scenario: A scaling matrix applied to a vector
  Given transform ← scaling(2, 3, 4)
    And v ← vector(-4, 6, 8)
   Then transform * v = vector(-8, 18, 32)
*/

TEST(TransformationsTest, ApplyingAScalingMatrixToAVector) {
    // use alternate constructor
    double data[] { 2, 3, 4 };
    Scaling transform { sizeof(data) / sizeof(double), data };
    Vector v { -4, 6, 8 }, expected { -8, 18, 32 };
    ASSERT_EQ(transform * v, expected);
}

/*
Scenario: Multiplying by the inverse of a scaling matrix
  Given transform ← scaling(2, 3, 4)
    And inv ← inverse(transform)
    And v ← vector(-4, 6, 8)
   Then inv * v = vector(-2, 2, 2)
*/

TEST(TransformationsTest, MultiplyingByTheInverseOfAScalingMatrix) {
    std::array<double, 3> data { 2, 3, 4 };
    Scaling transform { data };
    Matrix inverse = transform.Inverse();
    Vector v { -4, 6, 8 }, expected { -2, 2, 2 };
    ASSERT_EQ(inverse * v, expected);
}

/*
Scenario: Reflection is scaling by a negative value
  Given transform ← scaling(-1, 1, 1)
    And p ← point(2, 3, 4)
   Then transform * p = point(-2, 3, 4)
*/

TEST(TransformationsTest, ReflectingAPointInTheXAxis) {
    std::array<double, 3> data { -1, 1, 1 };
    Scaling transform { data };
    Point p { 2, 3, 4 }, expected { -2, 3, 4 };
    ASSERT_EQ(transform * p, expected);
}

/*
Scenario: Rotating a point around the x axis
  Given p ← point(0, 1, 0)
    And half_quarter ← rotation_x(π / 4)
    And full_quarter ← rotation_x(π / 2)
  Then half_quarter * p = point(0, √2/2, √2/2)
    And full_quarter * p = point(0, 0, 1)
*/

TEST(TransformationsTest, RotatingAPointAroundTheXAxis) {
    double sqrt2_div_2 = std::sqrt(2) / 2;
    Point p { 0, 1, 0 },
          p_half_quarter { 0, sqrt2_div_2, sqrt2_div_2 },
          p_full_quarter { 0, 0, 1 };
    XAxisRotation half_quarter { M_PI / 4 },
                  full_quarter { M_PI / 2 };
    ASSERT_EQ(half_quarter * p, p_half_quarter);
    ASSERT_EQ(full_quarter * p, p_full_quarter);
}

/*
Scenario: The inverse of an x-rotation rotates in the opposite direction
  Given p ← point(0, 1, 0)
    And half_quarter ← rotation_x(π / 4)
    And inv ← inverse(half_quarter)
  Then inv * p = point(0, √2/2, -√2/2)
*/

TEST(TransformationsTest, RotatingAPointAroundTheXAxisCounterClockwise) {
    double sqrt2_div_2 = std::sqrt(2) / 2;
    Point p { 0, 1, 0 },
          expected { 0, sqrt2_div_2, -sqrt2_div_2 };
    XAxisRotation half_quarter { M_PI / 4 };
    Matrix inverse = half_quarter.Inverse();
    ASSERT_EQ(inverse * p, expected);
}

/*
Scenario: Rotating a point around the y axis
  Given p ← point(0, 0, 1)
    And half_quarter ← rotation_y(π / 4)
    And full_quarter ← rotation_y(π / 2)
  Then half_quarter * p = point(√2/2, 0, √2/2)
    And full_quarter * p = point(1, 0, 0)
*/

TEST(TransformationsTest, RotatingAPointAroundTheYAxis) {
    double sqrt2_div_2 = std::sqrt(2) / 2;
    Point p { 0, 0, 1 },
          p_half_quarter { sqrt2_div_2, 0, sqrt2_div_2 },
          p_full_quarter { 1, 0, 0 };
    YAxisRotation half_quarter { M_PI / 4 },
                  full_quarter { M_PI / 2 };
    ASSERT_EQ(half_quarter * p, p_half_quarter);
    ASSERT_EQ(full_quarter * p, p_full_quarter);
}

/*
Scenario: Rotating a point around the z axis
  Given p ← point(0, 1, 0)
    And half_quarter ← rotation_z(π / 4)
    And full_quarter ← rotation_z(π / 2)
  Then half_quarter * p = point(-√2/2, √2/2, 0)
    And full_quarter * p = point(-1, 0, 0)
*/

TEST(TransformationsTest, RotatingAPointAroundTheZAxis) {
    double sqrt2_div_2 = std::sqrt(2) / 2;
    Point p { 0, 1, 0 },
          p_half_quarter { -sqrt2_div_2, sqrt2_div_2, 0 },
          p_full_quarter { -1, 0, 0 };
    ZAxisRotation half_quarter { M_PI / 4 },
                  full_quarter { M_PI / 2 };
    ASSERT_EQ(half_quarter * p, p_half_quarter);
    ASSERT_EQ(full_quarter * p, p_full_quarter);
}

/*
Scenario: A shearing transformation moves x in proportion to y
  Given transform ← shearing(1, 0, 0, 0, 0, 0)
    And p ← point(2, 3, 4)
  Then transform * p = point(5, 3, 4)
*/

TEST(TransformationsTest, ShearingXInProportionToY) {
    Shearing transform { 1, 0, 0, 0, 0, 0 };
    Point p { 2, 3, 4 }, expected { 5, 3, 4 };
    ASSERT_EQ(transform * p, expected);
}

/*
Scenario: A shearing transformation moves x in proportion to z
  Given transform ← shearing(0, 1, 0, 0, 0, 0)
    And p ← point(2, 3, 4)
  Then transform * p = point(6, 3, 4)
*/

TEST(TransformationsTest, ShearingXInProportionToZ) {
    Shearing transform { 0, 1, 0, 0, 0, 0 };
    Point p { 2, 3, 4 }, expected { 6, 3, 4 };
    ASSERT_EQ(transform * p, expected);
}

/*
Scenario: A shearing transformation moves y in proportion to x
  Given transform ← shearing(0, 0, 1, 0, 0, 0)
    And p ← point(2, 3, 4)
  Then transform * p = point(2, 5, 4)
*/

TEST(TransformationsTest, ShearingYInProportionToX) {
    Shearing transform { 0, 0, 1, 0, 0, 0 };
    Point p { 2, 3, 4 }, expected { 2, 5, 4 };
    ASSERT_EQ(transform * p, expected);
}

/*
Scenario: A shearing transformation moves y in proportion to z
  Given transform ← shearing(0, 0, 0, 1, 0, 0)
    And p ← point(2, 3, 4)
  Then transform * p = point(2, 7, 4)
*/

TEST(TransformationsTest, ShearingYInProportionToZ) {
    Shearing transform { 0, 0, 0, 1, 0, 0 };
    Point p { 2, 3, 4 }, expected { 2, 7, 4 };
    ASSERT_EQ(transform * p, expected);
}

/*
Scenario: A shearing transformation moves z in proportion to x
  Given transform ← shearing(0, 0, 0, 0, 1, 0)
    And p ← point(2, 3, 4)
  Then transform * p = point(2, 3, 6)
*/

TEST(TransformationsTest, ShearingZInProportionToX) {
    Shearing transform { 0, 0, 0, 0, 1, 0 };
    Point p { 2, 3, 4 }, expected { 2, 3, 6 };
    ASSERT_EQ(transform * p, expected);
}

/*
Scenario: A shearing transformation moves z in proportion to y
  Given transform ← shearing(0, 0, 0, 0, 0, 1)
    And p ← point(2, 3, 4)
  Then transform * p = point(2, 3, 7)
*/

TEST(TransformationsTest, ShearingZInProportionToY) {
    Shearing transform { 0, 0, 0, 0, 0, 1 };
    Point p { 2, 3, 4 }, expected { 2, 3, 7 };
    ASSERT_EQ(transform * p, expected);
}

/*
Scenario: Individual transformations are applied in sequence
  Given p ← point(1, 0, 1)
    And A ← rotation_x(π / 2)
    And B ← scaling(5, 5, 5)
    And C ← translation(10, 5, 7)
  # apply rotation first
  When p2 ← A * p
  Then p2 = point(1, -1, 0)
  # then apply scaling
  When p3 ← B * p2
  Then p3 = point(5, -5, 0)
  # then apply translation
  When p4 ← C * p3
  Then p4 = point(15, 0, 7)
*/

TEST(TransformationsTest, ApplyingTransformationsInSequence) {
    Point p { 1, 0, 1 },
          p2 { 1, -1, 0 },
          p3 { 5, -5, 0 },
          p4 { 15, 0 , 7 };
    XAxisRotation a { M_PI / 2 };
    Scaling b { std::array<double, 3> { 5, 5, 5 } };
    Translation c { std::array<double, 3> { 10, 5, 7 } };
    ASSERT_EQ(a * p, p2);
    ASSERT_EQ(b * p2, p3);
    ASSERT_EQ(c * p3, p4);
}

/*
Scenario: Chained transformations must be applied in reverse order
  Given p ← point(1, 0, 1)
    And A ← rotation_x(π / 2)
    And B ← scaling(5, 5, 5)
    And C ← translation(10, 5, 7)
  When T ← C * B * A
  Then T * p = point(15, 0, 7)
*/

TEST(TransformationsTest, ApplyingChainedTransformationsInReverseOrder) {
    Point p { 1, 0, 1 },
          expected { 15, 0, 7 };
    XAxisRotation a { M_PI / 2 };
    Scaling b { std::array<double, 3> { 5, 5, 5 } };
    Translation c { std::array<double, 3> { 10, 5, 7 } };
    Matrix t = c * b * a;
    ASSERT_EQ(t * p, expected);
}

TEST(TransformationsTest, ApplyingTransformationsUsingFluentAPI) {
    Point p { 1, 0, 1 },
          expected { 15, 0, 7 };
    Matrix transform = Transformation().RotateX(M_PI / 2).Scale(5, 5, 5).Translate(10, 5, 7);
    ASSERT_EQ(transform * p, expected);
}

/*
Scenario: The transformation matrix for the default orientation
  Given from ← point(0, 0, 0)
    And to ← point(0, 0, -1)
    And up ← vector(0, 1, 0)
  When t ← view_transform(from, to, up)
  Then t = identity_matrix
*/

/*
Scenario: A view transformation matrix looking in positive z direction
  Given from ← point(0, 0, 0)
    And to ← point(0, 0, 1)
    And up ← vector(0, 1, 0)
  When t ← view_transform(from, to, up)
  Then t = scaling(-1, 1, -1)
*/

/*
Scenario: The view transformation moves the world
  Given from ← point(0, 0, 8)
    And to ← point(0, 0, 0)
    And up ← vector(0, 1, 0)
  When t ← view_transform(from, to, up)
  Then t = translation(0, 0, -8)
*/

/*
Scenario: An arbitrary view transformation
  Given from ← point(1, 3, 2)
    And to ← point(4, -2, 8)
    And up ← vector(1, 1, 0)
  When t ← view_transform(from, to, up)
  Then t is the following 4x4 matrix:
      | -0.50709 | 0.50709 |  0.67612 | -2.36643 |
      |  0.76772 | 0.60609 |  0.12122 | -2.82843 |
      | -0.35857 | 0.59761 | -0.71714 |  0.00000 |
      |  0.00000 | 0.00000 |  0.00000 |  1.00000 |
*/