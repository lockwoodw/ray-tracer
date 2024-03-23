#include <cmath>
#include <stdexcept>
#include "space.h"

Vector::Vector(const Tuple& t): tuple_ { t } {
    if (t.Size() != 4) {
        throw std::invalid_argument("Incorrect tuple size for Vector");
    }
    tuple_[kW] = 0.0;
}

double Vector::DotProduct(const Vector& v1, const Vector& v2) {
    return v1.At(kX) * v2.At(kX) + v1.At(kY) * v2.At(kY) + v1.At(kZ) * v2.At(kZ);
}

Vector Vector::CrossProduct(const Vector& v1, const Vector& v2) {
    Vector product {
        v1.At(kY) * v2.At(kZ) - v1.At(kZ) * v2.At(kY),
        v1.At(kZ) * v2.At(kX) - v1.At(kX) * v2.At(kZ),
        v1.At(kX) * v2.At(kY) - v1.At(kY) * v2.At(kX)
    };
    return product;
}

Vector Vector::operator+(const Vector& v) const {
    Vector sum { tuple_ + v.tuple_ };
    return sum;
}

Vector Vector::operator-(const Vector& v) const {
    Vector difference { tuple_ - v.tuple_ };
    return difference;
}

Vector Vector::operator*(double d) const {
    Vector v { tuple_ * d };
    return v;
}

Vector Vector::operator/(double d) const {
    Vector v { tuple_ / d };
    return v;
}

double& Vector::operator[](int index) {
    return tuple_[index];
}

double Vector::At(int index) const {
    return tuple_.At(index);
}

bool Vector::operator==(const Tuple& t) const {
    return tuple_ == t;
}

bool Vector::operator!=(const Tuple& t) const {
    return tuple_ != t;
}

bool Vector::operator==(const Vector& v) const {
    return tuple_ == v.tuple_;
}

bool Vector::operator!=(const Vector& v) const {
    return tuple_ != v.tuple_;
}

double Vector::Magnitude() const {
    return std::sqrt(tuple_.At(kX) * tuple_.At(kX)
        + tuple_.At(kY) * tuple_.At(kY)
        + tuple_.At(kZ) * tuple_.At(kZ)
    );
}

Vector Vector::Normalize() const {
    return *this / this->Magnitude();
}

Point::Point(const Tuple& t): tuple_ { t } {
    if (t.Size() != 4) {
        throw std::invalid_argument("Incorrect tuple size for Point");
    }
    tuple_[kW] = 1.0;
}

Point Point::operator+(const Vector& v) const {
    Point p { tuple_ + v.tuple_ };
    return p;
}

Point Point::operator-(const Vector& v) const {
    Point p { tuple_ - v.tuple_ };
    return p;
}

Vector Point::operator-(const Point& p) const {
    Vector v { tuple_ - p.tuple_ };
    return v;
}

Point Point::operator*(double d) const {
    Point p { tuple_ * d };
    return p;
}

Point Point::operator/(double d) const {
    Point p { tuple_ / d };
    return p;
}

double& Point::operator[](int index) {
    return tuple_[index];
}

double Point::At(int index) const {
    return tuple_.At(index);
}

bool Point::operator==(const Tuple& t) const {
    return tuple_ == t;
}

bool Point::operator!=(const Tuple& t) const {
    return tuple_ != t;
}

bool Point::operator==(const Point& p) const {
    return tuple_ == p.tuple_;
}

bool Point::operator!=(const Point& p) const {
    return tuple_ != p.tuple_;
}