#include <cmath>
#include <algorithm>
#include "utils.h"

// Use combination of regular-epsilon and Donald Knuth's scaled-epsilon method to
// test equality of doubles;
// see https://www.learncpp.com/cpp-tutorial/relational-operators-and-floating-point-comparisons/

bool floating_point_relative_compare(double a, double b, double rel_epsilon) {
    return (std::abs(a - b) <= (std::max(std::abs(a), std::abs(b)) * rel_epsilon));
}

bool floating_point_compare(double a, double b, double abs_epsilon, double rel_epsilon) {
    // if the numbers are close, use the regular epsilon method
    if (std::abs(a - b) <= abs_epsilon)
        return true;
    // otherwise use Knuth's scaled approach
    return floating_point_relative_compare(a, b, rel_epsilon);
}

bool simple_floating_point_compare(double a, double b, double epsilon) {
    return std::abs(a - b) < epsilon;
}

bool FloatingPointComparator::compare(double a, double b) {
    return floating_point_compare(a, b, abs_epsilon_, rel_epsilon_);
}

bool SimpleFloatingPointComparator::compare(double a, double b) {
    return simple_floating_point_compare(a, b, epsilon_);
}