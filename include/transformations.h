#ifndef RAY_TRACER_TRANSFORMATIONS_H
#define RAY_TRACER_TRANSFORMATIONS_H

#include "matrix.h"
#include "space.h"

class Translation: public Matrix {
    public:
        template <std::size_t S>
        Translation(const std::array<double, S>& src): Matrix(S + 1, S + 1) {
            for (int i = 0; i < nrows_; i++) {
                // provide the identity matrix
                m_[i][i] = 1;
                // fill in last column with translation values; exclude last row
                if (i < S) {
                    m_[i][S] = src[i];
                }
            }
        }

        Translation(int size, double *src);
};

class Scaling: public Matrix {
    public:
        template <std::size_t S>
        Scaling(const std::array<double, S>& src): Matrix(S + 1, S + 1) {
            for (int i = 0; i < nrows_; i++) {
                // fill in diagonal with scaling values; exclude last row
                m_[i][i] = (i < S) ? src[i] : 1.0;
            }
        }

        Scaling(int size, double *src);
};

class XAxisRotation: public Matrix {
    public:
        XAxisRotation(double radians);
};

class YAxisRotation: public Matrix {
    public:
        YAxisRotation(double radians);
};

class ZAxisRotation: public Matrix {
    public:
        ZAxisRotation(double radians);
};

class Shearing: public Matrix {
    public:
        Shearing(double xy, double xz, double yx, double yz, double zx, double zy);
};

// Fluent API functions for 4x4 matrix
class Transformation: public Matrix {
    Transformation& operator*=(const Matrix& t);

    public:
        Transformation();
        Transformation(const Transformation& t): Matrix { t } {}
        Transformation& RotateX(double radians);
        Transformation& RotateY(double radians);
        Transformation& RotateZ(double radians);
        Transformation& Scale(double x, double y, double z);
        Transformation& Scale(double scale);
        Transformation& Translate(double x, double y, double z);
        Transformation& Shear(double xy, double xz, double yx, double yz, double zx, double zy);
};

class ViewTransform: public Matrix {
    public:
        ViewTransform(const Point& from, const Point& to, const Vector& up);
};

#endif