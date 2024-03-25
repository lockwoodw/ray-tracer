#include <cmath>
#include <stdexcept>
#include "space.h"

Vector::Vector(const Tuple& t): Tuple { t } {
    if (t.Size() != 4) {
        throw std::invalid_argument("Incorrect tuple size for Vector");
    }
    elements_[kW] = 0.0;
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

double Vector::Magnitude() const {
    return std::sqrt(elements_[kX] * elements_[kX]
        + elements_[kY] * elements_[kY]
        + elements_[kZ] * elements_[kZ]
    );
}

Vector Vector::Normalize() const {
    return *this / this->Magnitude();
}

Point::Point(const Tuple& t): Tuple { t } {
    if (t.Size() != 4) {
        throw std::invalid_argument("Incorrect tuple size for Point");
    }
    elements_[kW] = 1.0;
}