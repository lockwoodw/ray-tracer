#include <stdexcept>
#include "matrix.h"
#include "utils.h"

Matrix::Matrix(int nrows, int ncolumns) : nrows_ { nrows }, ncolumns_ { ncolumns } {
    m_ = new double*[nrows_];
    for (int i = 0; i < nrows_; i++) {
        m_[i] = new double[ncolumns_];
    }
}

Matrix::~Matrix() {
    for (int i = 0; i < nrows_; i++) {
        delete[] m_[i];
    }
    delete[] m_;
}

double& Matrix::At(int row, int column) {
    if (row < 0 || row >= nrows_) {
        throw std::runtime_error("Row index out of bounds");
    }
    if (column < 0 || column >= ncolumns_) {
        throw std::runtime_error("Column index out of bounds");
    }
    return m_[row][column];
}

bool Matrix::operator==(const Matrix &m) const {
    if (nrows_ != m.nrows_ || ncolumns_ != m.ncolumns_) {
        return false;
    }
    for (int i = 0; i < nrows_; i++) {
        for (int j = 0; j < ncolumns_; j++) {
            if (!floating_point_compare(m_[i][j], m.m_[i][j])) {
                return false;
            }
        }
    }
    return true;
}

bool Matrix::operator!=(const Matrix &m) const {
    return !(*this == m);
}

Matrix* Matrix::operator*(const Matrix &m) const {
    if (ncolumns_ != m.nrows_) {
        throw std::runtime_error("Operand dimensions invalid for Matrix multiplication");
    }
    int common = ncolumns_;
    Matrix *product = new Matrix(nrows_, m.ncolumns_);
    for (int i = 0; i < nrows_; i++) {
        for (int j = 0; j < m.ncolumns_; j++) {
            double sum = 0;
            for (int k = 0; k < common; k++) {
                    sum += m_[i][k] * m.m_[k][j];
            }
            product->m_[i][j] = sum;
        }
    }
    return product;
}

Matrix4x4::Matrix4x4(double m[4][4]): Matrix { 4, 4 } {
    for (int i = 0; i < nrows_; i++) {
        for (int j = 0; j < ncolumns_; j++) {
            m_[i][j] = m[i][j];
        }
    }
}

Matrix2x2::Matrix2x2(double m[2][2]): Matrix { 2, 2 } {
    for (int i = 0; i < nrows_; i++) {
        for (int j = 0; j < ncolumns_; j++) {
            m_[i][j] = m[i][j];
        }
    }
}

Matrix3x3::Matrix3x3(double m[3][3]): Matrix { 3, 3 } {
    for (int i = 0; i < nrows_; i++) {
        for (int j = 0; j < ncolumns_; j++) {
            m_[i][j] = m[i][j];
        }
    }
}