#include <cmath>
#include "transformations.h"

Translation::Translation(int size, double *src): Matrix(size + 1, size + 1) {
    for (int i = 0; i < nrows_; i++) {
        m_[i][i] = 1;
        if (i < size) {
            m_[i][size] = src[i];
        }
    }
}

Scaling::Scaling(int size, double *src): Matrix(size + 1, size + 1) {
    for (int i = 0; i < nrows_; i++) {
        m_[i][i] = (i < size) ? src[i] : 1.0;
    }
}

XAxisRotation::XAxisRotation(double radians): Matrix { 4, 4 } {
    double cos_r { std::cos(radians) }, sin_r { std::sin(radians) };
    m_[0][0] = 1.0;
    m_[1][1] = cos_r;
    m_[1][2] = -sin_r;
    m_[2][1] = sin_r;
    m_[2][2] = cos_r;
    m_[3][3] = 1.0;
}

YAxisRotation::YAxisRotation(double radians): Matrix { 4, 4 } {
    double cos_r { std::cos(radians) }, sin_r { std::sin(radians) };
    m_[0][0] = cos_r;
    m_[0][2] = sin_r;
    m_[1][1] = 1.0;
    m_[2][0] = -sin_r;
    m_[2][2] = cos_r;
    m_[3][3] = 1.0;
}

ZAxisRotation::ZAxisRotation(double radians): Matrix { 4, 4 } {
    double cos_r { std::cos(radians) }, sin_r { std::sin(radians) };
    m_[0][0] = cos_r;
    m_[0][1] = -sin_r;
    m_[1][0] = sin_r;
    m_[1][1] = cos_r;
    m_[2][2] = 1.0;
    m_[3][3] = 1.0;
}

Shearing::Shearing(double xy, double xz, double yx, double yz, double zx, double zy): Matrix { 4, 4 } {
    m_[0][0] = 1.0;
    m_[0][1] = xy;
    m_[0][2] = xz;
    m_[1][0] = yx;
    m_[1][1] = 1.0;
    m_[1][2] = yz;
    m_[2][0] = zx;
    m_[2][1] = zy;
    m_[2][2] = 1.0;
    m_[3][3] = 1.0;
}

Transformation::Transformation(): Matrix { 4, 4 } {
    // start with the identity matrix
    for (int i = 0; i < nrows_; i++) {
        m_[i][i] = 1.0;
    }
}

// For fluent API, this operator actually performs (mx × *this) so operations
// can be applied in order.
Transformation& Transformation::operator*=(const Matrix& mx) {
    if (!mx.IsSquare() || mx.Ncolumns() != nrows_) {
        throw std::invalid_argument("Operand dimensions invalid for Matrix multiplication");
    }
    Matrix copy { *this };
    Multiply(*this, mx, copy);
    return *this;
}

Transformation& Transformation::RotateX(double radians) {
    XAxisRotation transform { radians };
    return *this *= transform;
}

Transformation& Transformation::RotateY(double radians) {
    YAxisRotation transform { radians };
    return *this *= transform;
}

Transformation& Transformation::RotateZ(double radians) {
    ZAxisRotation transform { radians };
    return *this *= transform;
}

Transformation& Transformation::Scale(double x, double y, double z) {
    double src[] { x, y, z };
    Scaling transform { 3, src };
    return *this *= transform;
}

Transformation& Transformation::Scale(double scale) {
    double src[] { scale, scale, scale };
    Scaling transform { 3, src };
    return *this *= transform;
}

Transformation& Transformation::Translate(double x, double y, double z) {
    double src[] { x, y, z };
    Translation transform { 3, src };
    return *this *= transform;
}

Transformation& Transformation::Shear(double xy, double xz, double yx, double yz, double zx, double zy) {
    Shearing transform { xy, xz, yx, yz, zx, zy };
    return *this *= transform;
}

Transformation& Transformation::Apply(const Transformation& t) {
    return *this *= t;
}

ViewTransform::ViewTransform(const Point& from, const Point& to, const Vector& up): Matrix { 4, 4 } {
    Vector to_from { to - from },
           forward = to_from.Normalize(),
           normal_up = up.Normalize(),
           left = Vector::CrossProduct(forward, normal_up),
           true_up = Vector::CrossProduct(left, forward);

    Vector data[] { left, true_up, -forward };
    std::size_t size = sizeof(data) / sizeof(Vector);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            m_[i][j] = data[i][j];
        }
    }
    m_[nrows_ - 1][ncolumns_ - 1] = 1;
    Matrix translation = Transformation().Translate(-from.X(), -from.Y(), -from.Z());
    *this *= translation;
}