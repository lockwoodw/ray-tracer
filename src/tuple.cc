#include <cmath>
#include <stdexcept>
#include <cstring> // memset
#include "tuple.h"
#include "utils.h"

// Tuple::Tuple(double x, double y, double z, double w) : x_ { x }, y_ { y }, z_ { z }, w_ { w } {}

Tuple::Tuple(std::size_t n, const double* src) : size_ { n } {
    if (n < 0) {
        throw std::invalid_argument("Tuple size must be > 0");
    }
    elements_ = new double[n];
    for (int i = 0; i < n; i++) {
        elements_[i] = src[i];
    }
}

Tuple::Tuple(std::size_t n): size_ { n } {
    if (n < 0) {
        throw std::invalid_argument("Tuple size must be > 0");
    }
    elements_ = new double[n];
    // initialize all values to zero
    memset(elements_, 0, n * sizeof(double));
}

Tuple::Tuple(const Tuple& t): size_ { t.size_ } {
    elements_ = new double[size_];
    for (int i = 0; i < size_; i++) {
        elements_[i] = t.At(i);
    }
}

bool Tuple::operator==(const Tuple& t) const {
    if (size_ != t.size_) {
        return false;
    }
    for (int i = 0; i < size_; i++) {
        if (!floating_point_compare(elements_[i], t.elements_[i])) {
            return false;
        }
    }
    return true;
}

bool Tuple::operator!=(const Tuple& t) const {
    return ! operator==(t);
}

Tuple Tuple::operator+(const Tuple& t) const {
    if (size_ != t.size_) {
        throw std::invalid_argument("Addition not possible: tuples are not same size");
    }
    Tuple sum { size_ };
    for (int i = 0; i < size_; i++) {
        sum.elements_[i] = elements_[i] + t.elements_[i];
    }
    return sum;
}

Tuple Tuple::operator-(const Tuple& t) const {
    if (size_ != t.size_) {
        throw std::invalid_argument("Subtraction not possible: tuples are not same size");
    }
    Tuple difference { size_ };
    for (int i = 0; i < size_; i++) {
        difference.elements_[i] = elements_[i] - t.elements_[i];
    }
    return difference;
}

Tuple Tuple::operator-() const {
    Tuple negated { size_ };
    for (int i = 0; i < size_; i++) {
        negated.elements_[i] = -elements_[i];
    }
    return negated;
}

Tuple Tuple::operator*(double d) const {
    Tuple scaled { size_ };
    for (int i = 0; i < size_; i++) {
        scaled.elements_[i] = elements_[i] * d;
    }
    return scaled;
}

// Tuple& Tuple::operator*=(double d) {
//     for (int i = 0; i < size_; i++) {
//         elements_[i] *= d;
//     }
//     return *this;
// }

Tuple Tuple::operator/(double d) const {
    if (d == 0.0) {
        throw std::invalid_argument("Divide by zero attempted");
    }
    Tuple scaled { size_ };
    for (int i = 0; i < size_; i++) {
        scaled.elements_[i] = elements_[i] / d;
    }
    return scaled;
}

double& Tuple::operator[](std::size_t index) {
    if (index < 0 || index >= size_) {
        throw std::out_of_range("Requested index is out of range");
    }
    return elements_[index];
}

double Tuple::At(std::size_t index) const {
    if (index < 0 || index >= size_) {
        throw std::out_of_range("Requested index is out of range");
    }
    return elements_[index];
}

std::ostream& operator<<(std::ostream& os, const Tuple& t) {
    os << "[ ";
    for (int i = 0; i < t.size_; i++) {
        os << (i > 0 ? ", " : "") << t.elements_[i];
    }
    os << " ]" << std::endl;
    return os;
}

Tuple get_4_tuple(double x, double y, double z, double w) {
    Tuple p { 4 };
    p[0] = x;
    p[1] = y;
    p[2] = z;
    p[3] = w;
    return p;
}
/*




double Tuple::Magnitude() const {
    return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_ + w_ * w_);
}

Tuple Tuple::Normalize() const {
    Tuple normalized = *this / this->Magnitude();
    return normalized;
}

std::string Tuple::ToString() const {
    return "[" + std::to_string(x_) + ", "  +
        std::to_string(y_) + ", " + std::to_string(z_) + ", " +
        std::to_string(w_) + "]";
}

Point::Point(double x, double y, double z) : Tuple(x, y, z, 1.0) {}

Tuple Point::operator+(const Tuple& t) const {
    if (t.w_ == 1.0) {
        throw std::runtime_error("Can't add Point to a Point");
    }
    return static_cast<Tuple>(*this) + t;
}

double Point::Magnitude() const {
    throw std::runtime_error("Points have no magnitude");
}

Point Point::operator+(const Vector& v) const {
    Tuple t = static_cast<Tuple>(*this) + v;
    return Point { t.x_, t.y_, t.z_ };
}

Vector::Vector(double x, double y, double z) : Tuple(x, y, z, 0.0) {}

Tuple Vector::operator-(const Tuple& t) const {
    if (t.w_ == 1.0) {
        throw std::runtime_error("Can't subtract a Point from a Vector");
    }
    return static_cast<Tuple>(*this) - t;
}

double Vector::DotProduct(const Vector& v1, const Vector& v2) {
    double product = v1.x_ * v2.x_ + v1.y_ * v2.y_ + v1.z_ * v2.z_;
    return product;
}

Vector Vector::CrossProduct(const Vector& v1, const Vector& v2) {
    Vector product {
        v1.y_ * v2.z_ - v1.z_ * v2.y_,
        v1.z_ * v2.x_ - v1.x_ * v2.z_,
        v1.x_ * v2.y_ - v1.y_ * v2.x_
    };
    return product;
}

Vector Vector::Normalize() const {
    Tuple t = static_cast<Tuple>(*this).Normalize();
    return Vector { t.x_, t.y_, t.z_ };
}

Vector Vector::operator*(const double& d) const {
    Tuple t = static_cast<Tuple>(*this) * d;
    return Vector { t.x_, t.y_, t.z_ };
}

Vector Vector::operator+(const Vector& v) const {
    Tuple t = static_cast<Tuple>(*this) + v;
    return Vector { t.x_, t.y_, t.z_ };
}

*/