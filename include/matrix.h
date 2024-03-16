#ifndef RAY_TRACER_MATRIX_H
#define RAY_TRACER_MATRIX_H

#include <cstddef>
#include <iostream>
#include "tuple.h"

class Matrix {
    protected:
        double** m_;
        int nrows_;
        int ncolumns_;

    public:
        void static SetProduct(Matrix &product, const Matrix &m1, const Matrix &m2);
        const static Matrix Identity(int size);

        Matrix(int nrows, int ncolumns);
        ~Matrix();

        int Nrows() const { return nrows_; }
        int Ncolumns() const { return ncolumns_; }
        double At(int row, int column) const;
        double* operator[](int row);
        bool operator==(const Matrix &m) const;
        bool operator!=(const Matrix &m) const;
        const Matrix operator*(const Matrix &m) const;
        Matrix Transpose() const;
        Matrix Submatrix(int row, int column) const;

        friend const Tuple operator*(const Matrix& m, const Tuple &t);
        friend std::ostream& operator<<(std::ostream& os, const Matrix& m);
};

class SquareMatrix : public Matrix {
    public:
        SquareMatrix(int size): Matrix { size, size } {}
        SquareMatrix(const Matrix& m);

        double Minor(int row, int column) const;
        double Cofactor(int row, int column) const;
        double Determinant() const;
};

class Matrix4x4 : public SquareMatrix {
    public:
        Matrix4x4(): SquareMatrix { 4 } {}
        Matrix4x4(double m[4][4]);
};

class Matrix4x1 : public Matrix {
    public:
        Matrix4x1(): Matrix { 4, 1 } {}
        Matrix4x1(const Tuple &t);
        friend Matrix4x4;
};

class Matrix2x2 : public SquareMatrix {
    public:
        Matrix2x2(): SquareMatrix { 2 } {}
        Matrix2x2(double m[2][2]);
};

class Matrix3x3 : public SquareMatrix {
    public:
        Matrix3x3(): SquareMatrix { 3 } {}
        Matrix3x3(double m[3][3]);
};

#endif