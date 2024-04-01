#include <cmath>
#include <stdexcept>
#include "space.h"

SpatialTuple::SpatialTuple(const Tuple& t): Tuple { t } {
    if (t.Size() != 4) {
        throw std::invalid_argument("Incorrect tuple size for SpatialTuple");
    }
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

Vector Vector::Reflect(const Vector& in, const Vector& normal) {
    return in - normal * 2 * DotProduct(in, normal);
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