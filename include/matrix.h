#ifndef RAY_TRACER_MATRIX_H
#define RAY_TRACER_MATRIX_H

#include <cstddef>
#include <iostream>
#include <array>
#include "tuple.h"
#include "utils.h"

class Matrix {
    protected:
        double** m_;
        int nrows_;
        int ncolumns_;

    public:
        void static SetProduct(Matrix& product, const Matrix& m1, const Matrix& m2);
        const static Matrix Identity(int size);

        Matrix(int nrows, int ncolumns);

        // templated methods must be defined in the header file
        template <std::size_t R, std::size_t C>
        Matrix(const std::array<std::array<double, C>, R>& src): nrows_ { R }, ncolumns_ { C } {
            m_ = new double*[nrows_];
            for (int i = 0; i < nrows_; i++) {
                m_[i] = new double[ncolumns_];
                for (int j = 0; j < ncolumns_; j++) {
                    m_[i][j] = src[i][j];
                }
            }
        }

        ~Matrix();

        // conversion constructor
        Matrix(const Tuple& t): Matrix { 4, 1 } {
            m_[0][0] = t.x_;
            m_[1][0] = t.y_;
            m_[2][0] = t.z_;
            m_[3][0] = t.w_;
        }

        int Nrows() const { return nrows_; }
        int Ncolumns() const { return ncolumns_; }
        double At(int row, int column) const;
        double* operator[](int row);
        bool operator==(const Matrix& m) const;
        bool operator!=(const Matrix& m) const;
        const Matrix operator*(const Matrix& m) const;

        Matrix Transpose() const;
        Matrix Submatrix(int row, int column) const;
        double Minor(int row, int column) const;
        double Cofactor(int row, int column) const;
        double Determinant() const;
        Matrix Inverse() const;

        friend const Tuple operator*(const Matrix& m, const Tuple& t);
        friend std::ostream& operator<<(std::ostream& os, const Matrix& m);
};

#endif