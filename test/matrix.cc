#include <gtest/gtest.h>
#include <array>
#include "matrix.h"

TEST(MatrixTest, ConstructingAndInspectingA4x4Matrix) {
    // note double braces around literals
    // see https://www.learncpp.com/cpp-tutorial/stdarray-of-class-types-and-brace-elision/
    std::array<std::array<double, 4>, 4> data {{
        { 1, 2, 3, 4 },
        { 5.5, 6.5, 7.5, 8.5 },
        { 9, 10, 11, 12 },
        { 13.5, 14.5, 15.5, 16.5 }
    }};
    Matrix m { data };
    for (int i = 0; i < m.Nrows(); i++) {
        for (int j = 0; j < m.Ncolumns(); j++) {
            ASSERT_DOUBLE_EQ(data[i][j], m[i][j]);
        }
    }
}

TEST(MatrixTest, SettingAnElementInA4x4Matrix) {
    Matrix m { 4, 4 };
    m[0][0] = 2.5;
    m[3][3] = 42;
    ASSERT_DOUBLE_EQ(2.5, m[0][0]);
    ASSERT_DOUBLE_EQ(42, m[3][3]);
    ASSERT_DOUBLE_EQ(0, m[1][1]);
}

TEST(MatrixTest, RequestingAnElementInA4x4MatrixOutOfBounds) {
    Matrix m { 4, 4 };
    ASSERT_ANY_THROW(m.At(0, 4));
    ASSERT_ANY_THROW(m.At(-1, 2));
    ASSERT_ANY_THROW(m.At(4, 0));
    ASSERT_ANY_THROW(m.At(0, -4));
}

TEST(MatrixTest, ConstructingAndInspectingA2x2Matrix) {
    std::array<std::array<double, 2>, 2> data {{
        { -3, 5 },
        { 1, -2 }
    }};
    Matrix m { data };
    ASSERT_DOUBLE_EQ(data[0][0], m.At(0, 0));
    ASSERT_DOUBLE_EQ(data[0][1], m.At(0, 1));
    ASSERT_DOUBLE_EQ(data[1][0], m.At(1, 0));
    ASSERT_DOUBLE_EQ(data[1][1], m.At(1, 1));
}

TEST(MatrixTest, ConstructingAndInspectingA3x3Matrix) {
    std::array<std::array<double, 3>, 3> data {{
        { -3, 5, 0 },
        { 1, -2, -7 },
        { 0, 1, 1}
    }};
    Matrix m { data };
    for (int i = 0; i < m.Nrows(); i++) {
        for (int j = 0; j < m.Ncolumns(); j++) {
            ASSERT_DOUBLE_EQ(data[i][j], m[i][j]);
        }
    }
}

TEST(MatrixTest, MatrixEqualityWithIdenticalMatrices) {
    std::array<std::array<double, 4>, 4>
        a {{
            { 1, 2, 3, 4 },
            { 5, 6, 7, 8 },
            { 9, 8, 7, 6 },
            { 5, 4, 3, 2 }
        }},
        b {{
            { 1, 2, 3, 4 },
            { 5, 6, 7, 8 },
            { 9, 8, 7, 6 },
            { 5, 4, 3, 2 }
        }};
    Matrix ma { a }, mb { b };
    ASSERT_EQ(ma, mb);
}

TEST(MatrixTest, MatrixEqualityWithDifferentMatrices) {
    std::array<std::array<double, 4>, 4>
        a {{
            { 1, 2, 3, 4 },
            { 5, 6, 7, 8 },
            { 9, 8, 7, 6 },
            { 5, 4, 3, 2 }
        }},
        b {{
            { 1, 2, 3, 4 },
            { 5, 6, 7, 8 },
            { 9, 8, 7, 6 },
            { 5, 4, 0, 2 }
        }};
    Matrix ma { a }, mb { b };
    ASSERT_NE(ma, mb);
}

TEST(MatrixTest, MultiplyingTwo4x4Matrices) {
    std::array<std::array<double, 4>, 4>
        a {{
            { 1, 2, 3, 4 },
            { 5, 6, 7, 8 },
            { 9, 8, 7, 6 },
            { 5, 4, 3, 2 }
        }},
        b {{
            { -2, 1, 2, 3 },
            { 3, 2, 1, -1 },
            { 4, 3, 6, 5 },
            { 1, 2, 7, 8 }
        }},
        axb {{
            { 20, 22, 50, 48 },
            { 44, 54, 114, 108 },
            { 40, 58, 110, 102 },
            { 16, 26, 46, 42}
        }};
    Matrix ma { a }, mb { b }, m_axb { axb }, product = ma * mb;
    ASSERT_EQ(m_axb, product);
}

TEST(MatrixTest, MultiplyingTwoMatricesWithACommonDimension) {
    std::array<std::array<double, 2>, 4> a4x2 {{
        { 10, 8 },
        { 11, 9 },
        { -4, 42 },
        { 12, 10 }
    }};
    std::array<std::array<double, 3>, 2> b2x3 {{
        { 1, 2, 3 },
        { 4, 5, 16 }
    }};
    std::array<std::array<double, 3>, 4> c4x3 {{
        { 42, 60, 158 },
        { 47, 67, 177 },
        { 164, 202, 660 },
        { 52, 74, 196 }
    }};
    Matrix ma { a4x2 }, mb { b2x3 }, product = ma * mb, expected { c4x3 };
    ASSERT_EQ(product, expected);
}

TEST(MatrixTest, MultiplyingTwoMatricesWithoutACommonDimension) {
    Matrix m2x2 { 2, 2 };
    Matrix m3x3 { 3, 3 };
    ASSERT_ANY_THROW(m2x2 * m3x3);
}

TEST(MatrixTest, MultiplyingAMatrixByATuple) {
    std::array<std::array<double, 4>, 4> a {{
        { 1, 2, 3, 4 },
        { 2, 4, 4, 2 },
        { 8, 6, 4, 1 },
        { 0, 0, 0, 1 }
    }};
    Matrix ma { a };
    Tuple b { 1, 2, 3, 1 }, expected { 18, 24, 33, 1 }, product = ma * b;
    ASSERT_EQ(product, expected);
}

TEST(MatrixTest, AccessingAMatrixValueByOperator) {
    std::array<std::array<double, 3>, 3> data {{
        { -3, 5, 0 },
        { 1, -2, -7 },
        { 0, 1, 1}
    }};
    Matrix m { data };
    for (int i = 0; i < m.Nrows(); i++) {
        for (int j = 0; j < m.Ncolumns(); j++) {
            ASSERT_DOUBLE_EQ(data[i][j], m[i][j]);
        }
    }
    // Try setting by indexing operator
    double new_value = 42.0;
    m[2][0] = new_value;
    ASSERT_NE(m[2][0], data[2][0]);
    ASSERT_DOUBLE_EQ(m[2][0], new_value);
}

TEST(MatrixTest, GeneratingIdentityMatrix) {
    std::array<std::array<double, 4>, 4> id {{
        { 1, 0, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 1 }
    }};
    Matrix expected { id };
    ASSERT_EQ(SquareMatrix::Identity(expected.Nrows()), expected);
}

TEST(MatrixTest, MultiplyingAMatrixByTheIdentityMatrix) {
    std::array<std::array<double, 4>, 4> a {{
        { 0, 1, 2, 4 },
        { 1, 2, 4, 8 },
        { 2, 4, 8, 16 },
        { 4, 8, 16, 32 }
    }};
    Matrix ma { a };
    SquareMatrix id = SquareMatrix::Identity(ma.Nrows());
    ASSERT_EQ(id * ma, ma);
}

TEST(MatrixTest, MultiplyingTheIdentityMatrixByTuple) {
    Tuple a { 1, 2, 3, 4 };
    SquareMatrix id = SquareMatrix::Identity(4);
    ASSERT_EQ(id * a, a);
}

TEST(MatrixTest, TransposingAMatrix) {
    std::array<std::array<double, 4>, 4>
        a {{
            { 0, 9, 3, 0 },
            { 9, 8, 0, 8 },
            { 1, 8, 5, 3 },
            { 0, 0, 5, 8 }
        }},
        b {{
            { 0, 9, 1, 0 },
            { 9, 8, 8, 0 },
            { 3, 0, 5, 5 },
            { 0, 8, 3, 8 }
        }};
    Matrix ma { a }, expected { b };
    ASSERT_EQ(ma.Transpose(), expected);
}

TEST(MatrixTest, TransposingTheIdentityMatrix) {
    SquareMatrix id = SquareMatrix::Identity(4);
    ASSERT_EQ(id.Transpose(), id);
}

TEST(MatrixTest, TransposingANonsquareMatrix) {
    std::array<std::array<double, 2>, 4> a {{
        { 10, 8 },
        { 11, 9 },
        { -4, 42 },
        { 12, 10 }
    }};
    std::array<std::array<double, 4>, 2> b {{
        { 10, 11, -4, 12 },
        { 8, 9, 42, 10 }
    }};
    Matrix ma { a }, mb { b };
    ASSERT_EQ(ma.Transpose(), mb);
}

TEST(MatrixTest, GeneratingDeterminantOfA2x2Matrix) {
    std::array<std::array<double, 2>, 2> a {{
        { 1, 5 },
        { -3, 2 }
    }};
    SquareMatrix ma { a };
    ASSERT_DOUBLE_EQ(ma.Determinant(), 17);
}

TEST(MatrixTest, GeneratingSubmatrixOfA3x3Matrix) {
    std::array<std::array<double, 3>, 3> a {{
        { 1, 5, 0 },
        { -3, 2, 7 },
        { 0, 6, -3 }
    }};
    std::array<std::array<double, 2>, 2> b {{
        { -3, 2 },
        { 0, 6 }
    }};
    Matrix ma { a }, mb { b };
    ASSERT_EQ(ma.Submatrix(0, 2), mb);
}

TEST(MatrixTest, GeneratingSubmatrixOfA4x4Matrix) {
    std::array<std::array<double, 4>, 4> a {{
        { -6, 1, 1, 6 },
        { -8, 5, 8, 6 },
        { -1, 0, 8, 2 },
        { -7, 1, -1, 1 }
    }};
    std::array<std::array<double, 3>, 3> b {{
        { -6, 1, 6 },
        { -8, 8, 6 },
        { -7, -1, 1 }
    }};
    Matrix ma { a }, mb { b };
    ASSERT_EQ(ma.Submatrix(2, 1), mb);
}

TEST(MatrixTest, GeneratingSubmatrixOfANonSquareMatrix) {
    std::array<std::array<double, 3>, 4> a {{
        { -6, 1, 6 },
        { -8, 5, 6 },
        { -1, 0, 42 },
        { -7, 1, 1 }
    }};
    std::array<std::array<double, 2>, 3> b {{
        { 1, 6 },
        { 5, 6 },
        { 1, 1 }
    }};
    Matrix ma { a }, mb { b };
    ASSERT_EQ(ma.Submatrix(2, 0), mb);
}

TEST(MatrixTest, CalculatingAMinorOfA3x3Matrix) {
    std::array<std::array<double, 3>, 3> a {{
        { 3, 5, 0 },
        { 2, -1, -7 },
        { 6, -1, 5 }
    }};
    SquareMatrix ma { a };
    ASSERT_DOUBLE_EQ(ma.Minor(1, 0), 25);
}

TEST(MatrixTest, CalculatingACofactorOfA3x3Matrix) {
    std::array<std::array<double, 3>, 3> a {{
        { 3, 5, 0 },
        { 2, -1, -7 },
        { 6, -1, 5 }
    }};
    SquareMatrix ma { a };
    ASSERT_DOUBLE_EQ(ma.Cofactor(0, 0), -12);
    ASSERT_DOUBLE_EQ(ma.Cofactor(1, 0), -25);
}

TEST(MatrixTest, CalculatingDeterminantOfA3x3Matrix) {
    std::array<std::array<double, 3>, 3> a {{
        { 1 , 2, 6 },
        { -5, 8, -4 },
        { 2, 6, 4 }
    }};
    SquareMatrix ma { a };
    ASSERT_DOUBLE_EQ(ma.Determinant(), -196);
}

TEST(MatrixTest, CalculatingDeterminantOfA4x4Matrix) {
    std::array<std::array<double, 4>, 4> a {{
        { -2, -8, 3, 5 },
        { -3, 1, 7, 3 },
        { 1, 2, -9, 6 },
        { -6, 7, 7, -9 }
    }};
    SquareMatrix ma { a };
    ASSERT_DOUBLE_EQ(ma.Determinant(), -4071);
}

TEST(MatrixTest, TestingAnInvertibleMatrixForInvertability) {
    std::array<std::array<double, 4>, 4> a {{
        { 6, 4, 4, 4 },
        { 5, 5, 7, 6 },
        { 4, -9, 3, -7 },
        { 9, 1, 7, -6 }
    }};
    SquareMatrix ma { a };
    ASSERT_DOUBLE_EQ(ma.Determinant(), -2120);
}

TEST(MatrixTest, TestingAnNoninvertibleMatrixForInvertability) {
    std::array<std::array<double, 4>, 4> a {{
        { -4, 2, -2, -3 },
        { 9, 6, 2, 6 },
        { 0, -5, 1, -5 },
        { 0, 0, 0, 0 }
    }};
    SquareMatrix ma { a };
    ASSERT_DOUBLE_EQ(ma.Determinant(), 0);
}

// Because the given reference values are not sufficiently precise for
// comparison using ASSERT_DOUBLE_EQ, compare them using a comparator
// with a smaller Epsilon
void compare_matrices(const Matrix& a, const Matrix& b) {
    for (int i = 0; i < a.Nrows(); i++) {
        for (int j = 0; j < a.Ncolumns(); j++) {
            ASSERT_TRUE(simple_floating_point_compare(a.At(i, j), b.At(i, j)));
        }
    }
}

TEST(MatrixTest, CalculatingTheInverseOfAMatrix) {
    std::array<std::array<double, 4>, 4>
        a {{
            { -5, 2, 6, -8 },
            { 1, -5, 1, 8 },
            { 7, 7, -6, -7 },
            { 1, -3, 7, 4 }
        }},
        b {{
            {  0.21805,  0.45113,  0.24060, -0.04511 },
            { -0.80827, -1.45677, -0.44361,  0.52068 },
            { -0.07895, -0.22368, -0.05263,  0.19737 },
            { -0.52256, -0.81391, -0.30075,  0.30639 }
        }};
    SquareMatrix ma { a }, mb { b }, inverse = ma.Inverse();
    compare_matrices(inverse, mb);
}

TEST(MatrixTest, CalculatingTheInverseOfAnotherMatrix) {
    std::array<std::array<double, 4>, 4>
        a {{
            {  8, -5,  9,  2 },
            {  7,  5,  6,  1 },
            { -6,  0,  9,  6 },
            { -3,  0, -9, -4 }
        }},
        b {{
            { -0.15385, -0.15385, -0.28205, -0.53846 },
            { -0.07692,  0.12308,  0.02564,  0.03077 },
            {  0.35897,  0.35897,  0.43590,  0.92308 },
            { -0.69231, -0.69231, -0.76923, -1.92308 }
        }};
    SquareMatrix ma { a }, mb { b }, inverse = ma.Inverse();
    compare_matrices(inverse, mb);
}

TEST(MatrixTest, CalculatingTheInverseOfAThirdMatrix) {
    std::array<std::array<double, 4>, 4>
        a {{
            {  9,  3,  0,  9 },
            { -5, -2, -6, -3 },
            { -4,  9,  6,  4 },
            { -7,  6,  6,  2 }
        }},
        b {{
            { -0.04074, -0.07778,  0.14444, -0.22222 },
            { -0.07778,  0.03333,  0.36667, -0.33333 },
            { -0.02901, -0.14630, -0.10926,  0.12963 },
            {  0.17778,  0.06667, -0.26667,  0.33333 }
        }};
    SquareMatrix ma { a }, mb { b }, inverse = ma.Inverse();
    compare_matrices(inverse, mb);
}

TEST(MatrixTest, MultiplyingTheProductOfTwoMatricesByTheInverseOfOneOfThem) {
    std::array<std::array<double, 4>, 4>
        a {{
            {  3, -9,  7,  3 },
            {  3, -8,  2, -9 },
            { -4,  4,  4,  1 },
            { -6,  5, -1,  1 }
        }},
        b {{
            {  8,  2,  2,  2 },
            {  3, -1,  7,  0 },
            {  7,  0,  5,  4 },
            {  6, -2,  0,  5 }
        }};
    SquareMatrix ma { a }, mb { b };
    Matrix mc = ma * mb;
    ASSERT_EQ(mc * mb.Inverse(), ma);
}

TEST(MatrixTest, InvertingTheIdentityMatrix) {
    // Inverting the Identity matrix yields the same
    SquareMatrix id = SquareMatrix::Identity(4);
    ASSERT_EQ(id.Inverse(), id);
}

TEST(MatrixTest, MultiplyingAMatrixByItsInverse) {
    // Multiplying a matrix by its inverse yeilds the Identity matrix
    std::array<std::array<double, 4>, 4> a {{
        {  3, -9,  7,  3 },
        {  3, -8,  2, -9 },
        { -4,  4,  4,  1 },
        { -6,  5, -1,  1 }
    }};
    SquareMatrix ma { a }, inverse = ma.Inverse();
    ASSERT_EQ(ma * inverse, SquareMatrix::Identity(4));
}

TEST(MatrixTest, TransposingAndInvertingAMatrix) {
    // The inverse of the transpose of a matrix is the same as
    // the transpose of the inverse
    std::array<std::array<double, 4>, 4> a {{
        {  3, -9,  7,  3 },
        {  3, -8,  2, -9 },
        { -4,  4,  4,  1 },
        { -6,  5, -1,  1 }
    }};
    SquareMatrix ma { a }, transpose = ma.Transpose<SquareMatrix>(), inverse = ma.Inverse();
    ASSERT_EQ(transpose.Inverse(), inverse.Transpose());
}

TEST(MatrixTest, MultiplyingATupleByAModifiedIdentityMatrix) {
    // Multiplying a tuple by a modified Identity matrix
    // scales the tuple by the modified amount
    Tuple t { 3, 4, 5, 6 }, expected { 3, 40, 5, -12 };
    SquareMatrix id = SquareMatrix::Identity(4);
    id[1][1] = 10;
    id[3][3] = -2;
    ASSERT_EQ(id * t, expected);
}