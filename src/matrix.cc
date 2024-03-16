#include <stdexcept>
#include <cstring> // memset
#include "matrix.h"
#include "utils.h"

Matrix::Matrix(int nrows, int ncolumns) : nrows_ { nrows }, ncolumns_ { ncolumns } {
    m_ = new double*[nrows_];
    for (int i = 0; i < nrows_; i++) {
        m_[i] = new double[ncolumns_];
        // initialize all values to zero
        memset(m_[i], 0, ncolumns_ * sizeof(double));
    }
}

Matrix::~Matrix() {
    for (int i = 0; i < nrows_; i++) {
        delete[] m_[i];
    }
    delete[] m_;
}

void Matrix::SetProduct(Matrix &product, const Matrix &m1, const Matrix &m2) {
    for (int i = 0; i < m1.nrows_; i++) {
        for (int j = 0; j < m2.ncolumns_; j++) {
            double sum = 0;
            for (int k = 0; k < m1.ncolumns_; k++) {
                    sum += m1.m_[i][k] * m2.m_[k][j];
            }
            product.m_[i][j] = sum;
        }
    }
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

double* Matrix::operator[](int row) {
    if (row < 0 || row >= nrows_) {
        throw std::runtime_error("Row index out of bounds");
    }
    return m_[row];
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
    return ! operator==(m);
}

Matrix Matrix::operator*(const Matrix &m) const {
    if (ncolumns_ != m.nrows_) {
        throw std::runtime_error("Operand dimensions invalid for Matrix multiplication");
    }
    Matrix product { nrows_, m.ncolumns_ };
    Matrix::SetProduct(product, *this, m);
    return product;
}

const Matrix Matrix::Identity() const {
    if (nrows_ != ncolumns_) {
        throw std::runtime_error("Cannot generate Identity matrix");
    }
    Matrix identity { nrows_, ncolumns_ };
    for (int i = 0; i < nrows_; i++) {
        identity.m_[i][i] = 1;
    }
    return identity;
}

std::ostream& operator<<(std::ostream& os, const Matrix& m) {
    for (int i = 0; i < m.nrows_; i++) {
        for (int j = 0; j < m.ncolumns_; j++) {
            os << (j > 0 ? "\t" : "") << m.m_[i][j];
        }
        os << std::endl;
    }
    return os;
}

Matrix4x1::Matrix4x1(const Tuple &t): Matrix { 4, 1 } {
    m_[0][0] = t.x_;
    m_[1][0] = t.y_;
    m_[2][0] = t.z_;
    m_[3][0] = t.w_;
}

Matrix4x4::Matrix4x4(double m[4][4]): Matrix { 4, 4 } {
    for (int i = 0; i < nrows_; i++) {
        for (int j = 0; j < ncolumns_; j++) {
            m_[i][j] = m[i][j];
        }
    }
}

// Tuple Matrix4x4::operator*(const Tuple &t) const {
//     Matrix4x1 tuple { t }, product;
//     Matrix::SetProduct(product, *this, tuple);
//     return Tuple { product.m_[0][0], product.m_[1][0], product.m_[2][0], product.m_[3][0] };
// }

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