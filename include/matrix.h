#ifndef RAY_TRACER_MATRIX_H
#define RAY_TRACER_MATRIX_H

#include <cstddef>
#include "tuple.h"

class Matrix {
    protected:
        double** m_;
        int nrows_;
        int ncolumns_;

    public:
        void static SetProduct(Matrix &product, const Matrix &m1, const Matrix &m2);
        Matrix(int nrows, int ncolumns);
        ~Matrix();
        int Nrows() const { return nrows_; }
        int Ncolumns() const { return ncolumns_; }
        double& At(int row, int column);
        double* operator[](int row);
        bool operator==(const Matrix &m) const;
        bool operator!=(const Matrix &m) const;
        Matrix* operator*(const Matrix &m) const;
};

class Matrix4x4 : public Matrix {
    public:
        Matrix4x4(): Matrix { 4, 4 } {}
        Matrix4x4(double m[4][4]);
        Matrix4x4 operator*(const Matrix4x4 &m) const;
        Tuple operator*(const Tuple &t) const;
};

class Matrix4x1 : public Matrix {
    public:
        Matrix4x1(): Matrix { 4, 1 } {}
        Matrix4x1(const Tuple &t);
        friend Matrix4x4;
};

class Matrix2x2 : public Matrix {
    public:
        Matrix2x2(): Matrix { 2, 2 } {}
        Matrix2x2(double m[2][2]);
};

class Matrix3x3 : public Matrix {
    public:
        Matrix3x3(): Matrix { 3, 3 } {}
        Matrix3x3(double m[3][3]);
};

#endif