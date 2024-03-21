#include <cmath>
#include <stdexcept>
#include "tuple.h"
#include "utils.h"

Tuple::Tuple(double x, double y, double z, double w) : x_ { x }, y_ { y }, z_ { z }, w_ { w } {}

bool Tuple::operator==(const Tuple& t) const {
    return floating_point_compare(x_, t.x_)
        && floating_point_compare(y_, t.y_)
        && floating_point_compare(z_, t.z_)
        && floating_point_compare(w_, t.w_);
}

bool Tuple::operator!=(const Tuple& t) const {
    return ! operator==(t);
}

Tuple Tuple::operator+(const Tuple& t) const {
    Tuple sum { x_ + t.x_, y_ + t.y_, z_ + t.z_, w_ + t.w_ };
    return sum;
}

Tuple Tuple::operator-(const Tuple& t) const {
    Tuple difference { x_ - t.x_, y_ - t.y_, z_ - t.z_, w_ - t.w_ };
    return difference;
}

Tuple Tuple::operator-() const {
    Tuple negated { -x_, -y_, -z_, -w_ };
    return negated;
}

Tuple Tuple::operator*(const double& d) const {
    Tuple scaled { x_ * d, y_ * d, z_ * d, w_ * d };
    return scaled;
}

Tuple Tuple::operator/(const double& d) const {
    if (d == 0.0) {
        throw std::runtime_error("Divide by zero attempted");
    }
    Tuple scaled { x_ / d, y_ / d, z_ / d, w_ / d };
    return scaled;
}

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
