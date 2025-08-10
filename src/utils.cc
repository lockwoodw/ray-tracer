#include <cmath>
#include <algorithm>
#include "utils.h"

// Use combination of regular-epsilon and Donald Knuth's scaled-epsilon method to
// test equality of doubles;
// see https://www.learncpp.com/cpp-tutorial/relational-operators-and-floating-point-comparisons/

bool floating_point_relative_compare(double a, double b, double rel_epsilon) {
    return (std::fabs(a - b) <= (std::max(std::fabs(a), std::fabs(b)) * rel_epsilon));
}

bool floating_point_compare(double a, double b, double abs_epsilon, double rel_epsilon) {
    // infinity check: normal floating-point comparisons don't seem to apply
    // in this case; regular equality check will catch if both values are infinite;
    // otherwise, they are always not equal
    if (std::isinf(a) || std::isinf(b)) {
        return a == b;
    }
    // if the numbers are close, use the regular epsilon method
    if (std::fabs(a - b) <= abs_epsilon)
        return true;
    // otherwise use Knuth's scaled approach
    return floating_point_relative_compare(a, b, rel_epsilon);
}

bool simple_floating_point_compare(double a, double b, double epsilon) {
    return std::fabs(a - b) < epsilon;
}

bool FloatingPointComparator::compare(double a, double b) {
    return floating_point_compare(a, b, abs_epsilon_, rel_epsilon_);
}

bool SimpleFloatingPointComparator::compare(double a, double b) {
    return simple_floating_point_compare(a, b, epsilon_);
}