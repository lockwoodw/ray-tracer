#include <gtest/gtest.h>
#include "colour.h"

/*
Scenario: Colors are (red, green, blue) tuples
  Given c ← color(-0.5, 0.4, 1.7)
  Then c.red = -0.5
    And c.green = 0.4
    And c.blue = 1.7
*/

TEST(ColourTest, ColoursAreTuples) {
    Colour c { -0.5, 0.4, 1.7 };
    ASSERT_DOUBLE_EQ(c.Red(), -0.5);
    ASSERT_DOUBLE_EQ(c.Green(), 0.4);
    ASSERT_DOUBLE_EQ(c.Blue(), 1.7);
}

/*
Scenario: Adding colors
  Given c1 ← color(0.9, 0.6, 0.75)
    And c2 ← color(0.7, 0.1, 0.25)
   Then c1 + c2 = color(1.6, 0.7, 1.0)
*/

TEST(ColourTest, AddingColours) {
    Colour c1 { 0.9, 0.6, 0.75 }, c2 { 0.7, 0.1, 0.25 }, expected { 1.6, 0.7, 1.0 };
    ASSERT_EQ(c1 + c2, expected);
}

/*
Scenario: Subtracting colors
  Given c1 ← color(0.9, 0.6, 0.75)
    And c2 ← color(0.7, 0.1, 0.25)
   Then c1 - c2 = color(0.2, 0.5, 0.5)
*/

TEST(ColourTest, SubtractingColours) {
    Colour c1 { 0.9, 0.6, 0.75 }, c2 { 0.7, 0.1, 0.25 }, expected { 0.2, 0.5, 0.5 };
    ASSERT_EQ(c1 - c2, expected);
}

/*
Scenario: Multiplying a color by a scalar
  Given c ← color(0.2, 0.3, 0.4)
  Then c * 2 = color(0.4, 0.6, 0.8)
*/

TEST(ColourTest, MultiplyingAColourByAScalar) {
    Colour c1 { 0.2, 0.3, 0.4 }, expected { 0.4, 0.6, 0.8 };
    ASSERT_EQ(c1 * 2, expected);
}

/*
Scenario: Multiplying colors
  Given c1 ← color(1, 0.2, 0.4)
    And c2 ← color(0.9, 1, 0.1)
   Then c1 * c2 = color(0.9, 0.2, 0.04)
*/

TEST(ColourTest, MultiplyingColours) {
    Colour c1 { 1, 0.2, 0.4 }, c2 { 0.9, 1, 0.1 }, expected { 0.9, 0.2, 0.04 };
    ASSERT_EQ(c1 * c2, expected);
}