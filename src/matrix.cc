#include <stdexcept>
#include <cstring> // memset
#include "matrix.h"

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

double Matrix::At(int row, int column) const {
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

Matrix const Matrix::operator*(const Matrix &m) const {
    if (ncolumns_ != m.nrows_) {
        throw std::runtime_error("Operand dimensions invalid for Matrix multiplication");
    }
    Matrix product { nrows_, m.ncolumns_ };
    Matrix::SetProduct(product, *this, m);
    return product;
}

Matrix Matrix::Transpose() const {
    Matrix transposed { ncolumns_, nrows_ };
    for (int i = 0; i < nrows_; i++) {
        for (int j = 0; j < ncolumns_; j++) {
            transposed.m_[j][i] = m_[i][j];
        }
    }
    return transposed;
}

// Returns a copy of the matrix with the given row and column removed
Matrix Matrix::Submatrix(int row, int column) const {
    if (nrows_ < 2 || ncolumns_ < 2) {
        throw std::runtime_error("Submatrix does not exist");
    }
    if (row < 0 || row >= nrows_) {
        throw std::runtime_error("Row index out of bounds");
    }
    if (column < 0 || column >= ncolumns_) {
        throw std::runtime_error("Column index out of bounds");
    }
    Matrix submatrix { nrows_ - 1, ncolumns_ - 1 };
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

const Tuple operator*(const Matrix& m, const Tuple &t) {
    Matrix4x1 tuple { t }, product;
    Matrix::SetProduct(product, m, tuple);
    return Tuple { product[0][0], product[1][0], product[2][0], product[3][0] };
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

// Conversion constructor required for Submatrix()
SquareMatrix::SquareMatrix(const Matrix& m): Matrix { m.Nrows(), m.Nrows() } {
    if (m.Ncolumns() < nrows_) {
        throw std::runtime_error("Cannot convert to square matrix");
    }
    for (int i = 0; i < nrows_; i++) {
        for (int j = 0; j < ncolumns_; j++) {
            m_[i][j] = m.At(i, j);
        }
    }
}

// The minor of an element at (i, j) is the determinant of the
// submatrix(i, j)
double SquareMatrix::Minor(int row, int column) const {
    SquareMatrix submatrix = Submatrix(row, column);
    return submatrix.Determinant();
}

double SquareMatrix::Cofactor(int row, int column) const {
    int sign = (row + column) % 2 == 0 ? 1 : -1;
    return Minor(row, column) * sign;
}

double SquareMatrix::Determinant() const {
    if (nrows_ == 2) {
        // By definition:
        return m_[0][0] * m_[1][1] - m_[1][0] * m_[0][1];
    }
    // Choose first row: sum the products of each element and its cofactor
    double determinant = 0;
    for (int j = 0; j < ncolumns_; j++) {
        determinant += m_[0][j] * Cofactor(0, j);
    }
    return determinant;
}

SquareMatrix SquareMatrix::Inverse() const {
    double determinant = Determinant();

    if (floating_point_compare(determinant, 0)) {
        throw std::runtime_error("Matrix not invertible");
    }

    SquareMatrix inverse { nrows_ };

    for (int i = 0; i < nrows_; i++) {
        for (int j = 0; j < nrows_; j++) {
            double cofactor = Cofactor(i, j);
            inverse.m_[j][i] = cofactor / determinant;
        }
    }

    return inverse;
}

const SquareMatrix SquareMatrix::Identity(int size) {
    SquareMatrix identity { size };
    for (int i = 0; i < size; i++) {
        identity.m_[i][i] = 1;
    }
    return identity;
}

Matrix4x1::Matrix4x1(const Tuple &t): Matrix { 4, 1 } {
    m_[0][0] = t.x_;
    m_[1][0] = t.y_;
    m_[2][0] = t.z_;
    m_[3][0] = t.w_;
}

Matrix4x4::Matrix4x4(double m[4][4]): SquareMatrix { 4 } {
    for (int i = 0; i < nrows_; i++) {
        for (int j = 0; j < ncolumns_; j++) {
            m_[i][j] = m[i][j];
        }
    }
}

Matrix2x2::Matrix2x2(double m[2][2]): SquareMatrix { 2 } {
    for (int i = 0; i < nrows_; i++) {
        for (int j = 0; j < ncolumns_; j++) {
            m_[i][j] = m[i][j];
        }
    }
}

Matrix3x3::Matrix3x3(double m[3][3]): SquareMatrix { 3 } {
    for (int i = 0; i < nrows_; i++) {
        for (int j = 0; j < ncolumns_; j++) {
            m_[i][j] = m[i][j];
        }
    }
}
