#include <gtest/gtest.h>
#include "utils.h"

TEST(UtilsTest, ComparingDoublesForEquality) {
    // Samples from https://www.learncpp.com/cpp-tutorial/relational-operators-and-floating-point-comparisons/
    double a { 10000 }, b { 10000.0001 }, epsilon { 1e-5 };
    ASSERT_TRUE(floating_point_compare(a, b, epsilon, epsilon));
    double c { 1e-7 }, d { 1e-5 };
    ASSERT_TRUE(floating_point_compare(c, d, epsilon, epsilon));
    double e { 1.0 }, f { 1.0001 }, g { 1.00001 };
    ASSERT_FALSE(floating_point_compare(e, f, epsilon, epsilon));
    ASSERT_TRUE(floating_point_compare(e, g, epsilon, epsilon));
    double close_to_one { 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 + 0.1 };
    ASSERT_TRUE(floating_point_compare(close_to_one - 1.0, 0.0, kAbsEpsilon, kRelEpsilon));
}