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
        void static SetProduct(Matrix &product, const Matrix &m1, const Matrix &m2);

        Matrix(int nrows, int ncolumns);

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

        int Nrows() const { return nrows_; }
        int Ncolumns() const { return ncolumns_; }
        double At(int row, int column) const;
        double* operator[](int row);
        bool operator==(const Matrix &m) const;
        bool operator!=(const Matrix &m) const;
        const Matrix operator*(const Matrix &m) const;

        // Templated methods let derived classes create instances of their
        // own type without a conversion constructor; must be defined in the
        // header file.
        template<typename T = Matrix> T Transpose() const {
            T transposed { ncolumns_, nrows_ };
            for (int i = 0; i < nrows_; i++) {
                for (int j = 0; j < ncolumns_; j++) {
                    transposed.m_[j][i] = m_[i][j];
                }
            }
            return transposed;
        }

        // Returns a copy of the matrix with the given row and column removed
        template<typename T = Matrix> T Submatrix(int row, int column) const {
            if (nrows_ < 2 || ncolumns_ < 2) {
                throw std::runtime_error("Submatrix does not exist");
            }
            if (row < 0 || row >= nrows_) {
                throw std::runtime_error("Row index out of bounds");
            }
            if (column < 0 || column >= ncolumns_) {
                throw std::runtime_error("Column index out of bounds");
            }
            T submatrix { nrows_ - 1, ncolumns_ - 1 };
            int r = 0, c;
            for (int i = 0; i < nrows_; i++) {
                if (i == row) {
                    continue;
                }
                c = 0;
                for (int j = 0; j < ncolumns_; j++) {
                    if (j == column) {
                        continue;
                    }
                    submatrix.m_[r][c] = m_[i][j];
                    c++;
                }
                r++;
            }
            return submatrix;
        }

        friend const Tuple operator*(const Matrix& m, const Tuple &t);
        friend std::ostream& operator<<(std::ostream& os, const Matrix& m);
};

class SquareMatrix : public Matrix {
    public:
        const static SquareMatrix Identity(int size);

        SquareMatrix(int size): Matrix { size, size } {}
        SquareMatrix(int size, int): Matrix { size, size } {} // for templated methods
        template <std::size_t R, std::size_t C>
        SquareMatrix(const std::array<std::array<double, C>, R>& src): Matrix { src } {}

        double Minor(int row, int column) const;
        double Cofactor(int row, int column) const;
        double Determinant() const;
        SquareMatrix Inverse() const;
};

class Matrix4x1 : public Matrix {
    public:
        Matrix4x1(): Matrix { 4, 1 } {}
        Matrix4x1(const Tuple &t);
        friend Matrix;
};

#endif