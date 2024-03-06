#include "types.h"
#include <algorithm>
#include <cmath>
#include <stdexcept>

Tuple::Tuple(double x, double y, double z, double w) : x_ { x }, y_ { y }, z_ { z }, w_ { w } {}

// Use combination of regular-epsilon and Donald Knuth's scaled-epsilon method to
// test equality of doubles;
// see https://www.learncpp.com/cpp-tutorial/relational-operators-and-floating-point-comparisons/

const double Tuple::Tuple::kAbsEpsilon = 1e-12;
const double Tuple::Tuple::kRelEpsilon = 1e-8;

bool Tuple::RelEqual(double a, double b, double rel_epsilon) {
    return (std::abs(a - b) <= (std::max(std::abs(a), std::abs(b)) * rel_epsilon));
}

bool Tuple::AbsEqual(double a, double b, double abs_epsilon, double rel_epsilon) {
    // if the numbers are close, use the regular epsilon method
    if (std::abs(a - b) <= abs_epsilon)
        return true;
    // otherwise use Knuth's scaled approach
    return Tuple::RelEqual(a, b, rel_epsilon);
}

bool Tuple::operator==(const Tuple &t) const {
    return Tuple::AbsEqual(x_, t.x_, Tuple::kAbsEpsilon, Tuple::kRelEpsilon)
        && Tuple::AbsEqual(y_, t.y_, Tuple::kAbsEpsilon, Tuple::kRelEpsilon)
        && Tuple::AbsEqual(z_, t.z_, Tuple::kAbsEpsilon, Tuple::kRelEpsilon)
        && Tuple::AbsEqual(w_, t.w_, Tuple::kAbsEpsilon, Tuple::kRelEpsilon);
}

bool Tuple::operator!=(const Tuple &t) const {
    return ! operator==(t);
}

Tuple Tuple::operator+(const Tuple &t) const {
    Tuple sum { x_ + t.x_, y_ + t.y_, z_ + t.z_, w_ + t.w_ };
    return sum;
}

Tuple Tuple::operator-(const Tuple &t) const {
    Tuple difference { x_ - t.x_, y_ - t.y_, z_ - t.z_, w_ - t.w_ };
    return difference;
}

Tuple Tuple::operator-() const {
    Tuple negated { -x_, -y_, -z_, -w_ };
    return negated;
}

Tuple Tuple::operator*(const double &d) const {
    Tuple scaled { x_ * d, y_ * d, z_ * d, w_ * d };
    return scaled;
}

Tuple Tuple::operator/(const double &d) const {
    if (d == 0.0) {
        throw std::runtime_error("Divide by zero attempted");
    }
    Tuple scaled { x_ / d, y_ / d, z_ / d, w_ / d };
    return scaled;
}

double Tuple::magnitude() const {
    return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_ + w_ * w_);
}

Tuple Tuple::normalize() const {
    Tuple normalized = *this / this->magnitude();
    return normalized;
}

Point::Point(double x, double y, double z) : Tuple(x, y, z, 1.0) {}

Tuple Point::operator+(const Tuple &t) const {
    if (t.w_ == 1.0) {
        throw std::runtime_error("Can't add Point to a Point");
    }
    return static_cast<Tuple>(*this) + t;
}

double Point::magnitude() const {
    throw std::runtime_error("Points have no magnitude");
}

Vector::Vector(double x, double y, double z) : Tuple(x, y, z, 0.0) {}

Tuple Vector::operator-(const Tuple &t) const {
    if (t.w_ == 1.0) {
        throw std::runtime_error("Can't subtract a Point from a Vector");
    }
    return static_cast<Tuple>(*this) - t;
}