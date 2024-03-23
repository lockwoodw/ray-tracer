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

Matrix::Matrix(const Tuple& t): Matrix { t.Size(), 1 } {
    for (int i = 0; i < nrows_; i++) {
        m_[i][0] = t.At(i);
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

bool Matrix::operator==(const Matrix& m) const {
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

bool Matrix::operator!=(const Matrix& m) const {
    return ! operator==(m);
}

Matrix const Matrix::operator*(const Matrix& m) const {
    if (ncolumns_ != m.nrows_) {
        throw std::runtime_error("Operand dimensions invalid for Matrix multiplication");
    }
    Matrix product { nrows_, m.ncolumns_ };
    for (int i = 0; i < nrows_; i++) {
        for (int j = 0; j < m.ncolumns_; j++) {
            double sum = 0;
            for (int k = 0; k < ncolumns_; k++) {
                    sum += m_[i][k] * m.m_[k][j];
            }
            product.m_[i][j] = sum;
        }
    }
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

const Tuple operator*(const Matrix& m, const Tuple& t) {
    if (m.ncolumns_ != t.Size()) {
        throw std::runtime_error("Operand dimensions invalid for multiplication");
    }
    Tuple product { static_cast<std::size_t>(m.nrows_) };
    for (int i = 0; i < m.nrows_; i++) {
        double sum = 0;
        for (int j = 0; j < m.ncolumns_; j++) {
            sum += m.m_[i][j] * t.At(j);
        }
        product[i] = sum;
    }
    return product;
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

// The minor of an element at (i, j) is the determinant of the
// submatrix(i, j)
double Matrix::Minor(int row, int column) const {
    Matrix submatrix = Submatrix(row, column);
    return submatrix.Determinant();
}

double Matrix::Cofactor(int row, int column) const {
    int sign = (row + column) % 2 == 0 ? 1 : -1;
    return Minor(row, column) * sign;
}

double Matrix::Determinant() const {
    if (nrows_ != ncolumns_) {
        throw std::domain_error("Matrix is not square; determinant cannot be calculated");
    }
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

Matrix Matrix::Inverse() const {
    double determinant = Determinant();

    if (floating_point_compare(determinant, 0)) {
        throw std::runtime_error("Matrix not invertible");
    }

    Matrix inverse { nrows_, nrows_ };

    for (int i = 0; i < nrows_; i++) {
        for (int j = 0; j < nrows_; j++) {
            double cofactor = Cofactor(i, j);
            inverse.m_[j][i] = cofactor / determinant;
        }
    }

    return inverse;
}

const Matrix Matrix::Identity(int size) {
    Matrix identity { size, size };
    for (int i = 0; i < size; i++) {
        identity.m_[i][i] = 1;
    }
    return identity;
}
