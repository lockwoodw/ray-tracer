#include <gtest/gtest.h>
#include "matrix.h"

TEST(MatrixTest, ConstructingAndInspectingA4x4Matrix) {
    double data[][4] {
        { 1, 2, 3, 4 },
        { 5.5, 6.5, 7.5, 8.5 },
        { 9, 10, 11, 12 },
        { 13.5, 14.5, 15.5, 16.5 }
    };
    Matrix4x4 m { data };
    ASSERT_DOUBLE_EQ(data[0][0], m[0][0]);
    ASSERT_DOUBLE_EQ(data[0][3], m[0][3]);
    ASSERT_DOUBLE_EQ(data[1][2], m[1][2]);
    ASSERT_DOUBLE_EQ(data[3][2], m[3][2]);
}

TEST(MatrixTest, SettingAnElementInA4x4Matrix) {
    Matrix4x4 m;
    m[0][0] = 2.5;
    m[3][3] = 42;
    ASSERT_DOUBLE_EQ(2.5, m[0][0]);
    ASSERT_DOUBLE_EQ(42, m[3][3]);
    ASSERT_DOUBLE_EQ(0, m[1][1]);
}

// TEST(MatrixTest, SettingAnElementInA4x4MatrixOutOfBounds) {
//     Matrix4x4 m;
//     ASSERT_ANY_THROW(m[0][4] = 2.5);
//     ASSERT_ANY_THROW(m[-1][2] = 3.5);
//     ASSERT_ANY_THROW(m[4][0] = 4.5);
//     ASSERT_ANY_THROW(m[0][-4] = 42.5);
// }

TEST(MatrixTest, ConstructingAndInspectingA2x2Matrix) {
    double data[][2] {
        { -3, 5 },
        { 1, -2 }
    };
    Matrix2x2 m { data };
    ASSERT_DOUBLE_EQ(data[0][0], m.At(0, 0));
    ASSERT_DOUBLE_EQ(data[0][1], m.At(0, 1));
    ASSERT_DOUBLE_EQ(data[1][0], m.At(1, 0));
    ASSERT_DOUBLE_EQ(data[1][1], m.At(1, 1));
}

TEST(MatrixTest, ConstructingAndInspectingA3x3Matrix) {
    double data[][3] {
        { -3, 5, 0 },
        { 1, -2, -7 },
        { 0, 1, 1}
    };
    Matrix3x3 m { data };
    ASSERT_DOUBLE_EQ(data[0][0], m.At(0, 0));
    ASSERT_DOUBLE_EQ(data[1][1], m.At(1, 1));
    ASSERT_DOUBLE_EQ(data[2][2], m.At(2, 2));
}

TEST(MatrixTest, MatrixEqualityWithIdenticalMatrices) {
    double
        a[][4] {
            { 1, 2, 3, 4 },
            { 5, 6, 7, 8 },
            { 9, 8, 7, 6 },
            { 5, 4, 3, 2 }
        },
            b[][4] {
            { 1, 2, 3, 4 },
            { 5, 6, 7, 8 },
            { 9, 8, 7, 6 },
            { 5, 4, 3, 2 }
        };
    Matrix4x4 ma { a }, mb { b };
    ASSERT_EQ(ma, mb);
}

TEST(MatrixTest, MatrixEqualityWithDifferentMatrices) {
    double
        a[][4] {
            { 1, 2, 3, 4 },
            { 5, 6, 7, 8 },
            { 9, 8, 7, 6 },
            { 5, 4, 3, 2 }
        },
        b[][4] {
            { 1, 2, 3, 4 },
            { 5, 6, 7, 8 },
            { 9, 8, 7, 6 },
            { 5, 4, 0, 2 }
        };
    Matrix4x4 ma { a }, mb { b };
    ASSERT_NE(ma, mb);
}

TEST(MatrixTest, MultiplyingTwo4x4Matrices) {
    double
        a[][4] {
            { 1, 2, 3, 4 },
            { 5, 6, 7, 8 },
            { 9, 8, 7, 6 },
            { 5, 4, 3, 2 }
        },
        b[][4] {
            { -2, 1, 2, 3 },
            { 3, 2, 1, -1 },
            { 4, 3, 6, 5 },
            { 1, 2, 7, 8 }
        },
        axb[][4] {
            { 20, 22, 50, 48 },
            { 44, 54, 114, 108 },
            { 40, 58, 110, 102 },
            { 16, 26, 46, 42}
        };
    Matrix4x4 ma { a }, mb { b }, m_axb { axb };
    Matrix product = ma * mb;
    ASSERT_EQ(m_axb, product);
}

TEST(MatrixTest, MultiplyingTwoMatricesWithACommonDimension) {
    double
        a4x2[][2] {
            { 10, 8 },
            { 11, 9 },
            { -4, 42 },
            { 12, 10 }
        },
        b2x3[][3] {
            { 1, 2, 3 },
            { 4, 5, 16 }
        },
        c4x3[][3] {
            { 42, 60, 158 },
            { 47, 67, 177 },
            { 164, 202, 660 },
            { 52, 74, 196 }
        };
    Matrix ma { 4, 2 }, mb { 2, 3 };
    for (int i = 0; i < ma.Nrows(); i++) {
        for (int j = 0; j < ma.Ncolumns(); j++) {
            ma[i][j] = a4x2[i][j];
        }
    }
    for (int i = 0; i < mb.Nrows(); i++) {
        for (int j = 0; j < mb.Ncolumns(); j++) {
            mb[i][j] = b2x3[i][j];
        }
    }
    Matrix product = ma * mb;
    for (int i = 0; i < product.Nrows(); i++) {
        for (int j = 0; j < product.Ncolumns(); j++) {
            ASSERT_FLOAT_EQ(product.At(i, j), c4x3[i][j]);
        }
    }
}

TEST(MatrixTest, MultiplyingTwoMatricesWithoutACommonDimension) {
    Matrix2x2 m2x2;
    Matrix3x3 m3x3;
    ASSERT_ANY_THROW(m2x2 * m3x3);
}

TEST(MatrixTest, MultiplyingAMatrixByATuple) {
    double a[][4] {
        { 1, 2, 3, 4 },
        { 2, 4, 4, 2 },
        { 8, 6, 4, 1 },
        { 0, 0, 0, 1 }
    };
    Matrix4x4 ma { a };
    Tuple b { 1, 2, 3, 1 }, expected { 18, 24, 33, 1 },
        product = ma * b;
    ASSERT_EQ(product, expected);
}

TEST(MatrixTest, AccessingAMatrixValueByOperator) {
    double data[][3] {
        { -3, 5, 0 },
        { 1, -2, -7 },
        { 0, 1, 1}
    };
    Matrix3x3 m { data };
    ASSERT_FLOAT_EQ(m[0][0], data[0][0]);
    ASSERT_FLOAT_EQ(m[1][2], data[1][2]);
    ASSERT_FLOAT_EQ(m[2][1], data[2][1]);
    // Try setting by indexing operator
    double new_value = 42.0;
    m[2][0] = new_value;
    ASSERT_NE(m[2][0], data[2][0]);
    ASSERT_FLOAT_EQ(m[2][0], new_value);
}

TEST(MatrixTest, GeneratingIdentityMatrix) {
    double id[][4] {
        { 1, 0, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 1 }
    };
    Matrix4x4 expected { id };
    ASSERT_EQ(Matrix::Identity(expected.Nrows()), expected);
}

TEST(MatrixTest, MultiplyingAMatrixByTheIdentityMatrix) {
    double a[][4] {
        { 0, 1, 2, 4 },
        { 1, 2, 4, 8 },
        { 2, 4, 8, 16 },
        { 4, 8, 16, 32 }
    };
    Matrix4x4 ma { a };
    Matrix id = Matrix::Identity(ma.Nrows());
    ASSERT_EQ(id * ma, ma);
}

TEST(MatrixTest, MultiplyingTheIdentityMatrixByTuple) {
    Tuple a { 1, 2, 3, 4 };
    Matrix id = Matrix::Identity(4);
    ASSERT_EQ(id * a, a);
}

TEST(MatrixTest, TransposingAMatrix) {
    double
        a[][4] {
            { 0, 9, 3, 0 },
            { 9, 8, 0, 8 },
            { 1, 8, 5, 3 },
            { 0, 0, 5, 8 }
        },
        b[][4] {
            { 0, 9, 1, 0 },
            { 9, 8, 8, 0 },
            { 3, 0, 5, 5 },
            { 0, 8, 3, 8 }
        };
    Matrix4x4 ma { a }, expected { b };
    ASSERT_EQ(ma.Transpose(), expected);
}

TEST(MatrixTest, TransposingTheIdentityMatrix) {
    Matrix id = Matrix::Identity(4);
    ASSERT_EQ(id.Transpose(), id);
}

TEST(MatrixTest, TransposingANonSquareMatrix) {
    double
        a[][2] {
            { 10, 8 },
            { 11, 9 },
            { -4, 42 },
            { 12, 10 }
        },
        b[][4] {
            { 10, 11, -4, 12 },
            { 8, 9, 42, 10 }
        };
    Matrix ma { 4, 2 }, mb { 2, 4 };
    for (int i = 0; i < ma.Nrows(); i++) {
        for (int j = 0; j < ma.Ncolumns(); j++) {
            ma[i][j] = a[i][j];
        }
    }
    for (int i = 0; i < mb.Nrows(); i++) {
        for (int j = 0; j < mb.Ncolumns(); j++) {
            mb[i][j] = b[i][j];
        }
    }
    ASSERT_EQ(ma.Transpose(), mb);
}

TEST(MatrixTest, GeneratingDeterminantOfA2x2Matrix) {
    double a[][2] {
        { 1, 5 },
        { -3, 2 }
    };
    Matrix2x2 ma { a };
    ASSERT_DOUBLE_EQ(ma.Determinant(), 17);
}

TEST(MatrixTest, GeneratingSubmatrixOfA3x3Matrix) {
    double
        a[][3] {
            { 1, 5, 0 },
            { -3, 2, 7 },
            { 0, 6, -3 }
        },
        b[][2] {
            { -3, 2 },
            { 0, 6 }
        };
    Matrix3x3 ma { a };
    Matrix2x2 mb { b };
    ASSERT_EQ(ma.Submatrix(0, 2), mb);
}

TEST(MatrixTest, GeneratingSubmatrixOfA4x4Matrix) {
    double
        a[][4] {
            { -6, 1, 1, 6 },
            { -8, 5, 8, 6 },
            { -1, 0, 8, 2 },
            { -7, 1, -1, 1 }
        },
        b[][3] {
            { -6, 1, 6 },
            { -8, 8, 6 },
            { -7, -1, 1 }
        };
    Matrix4x4 ma { a };
    Matrix3x3 mb { b };
    ASSERT_EQ(ma.Submatrix(2, 1), mb);
}

TEST(MatrixTest, GeneratingSubmatrixOfAnAssymetricMatrix) {
    double
        a[][3] {
            { -6, 1, 6 },
            { -8, 5, 6 },
            { -1, 0, 42 },
            { -7, 1, 1 }
        },
        b[][2] {
            { 1, 6 },
            { 5, 6 },
            { 1, 1 }
        };
    Matrix ma { 4, 3 }, mb { 3, 2 };
    for (int i = 0; i < ma.Nrows(); i++) {
        for (int j = 0; j < ma.Ncolumns(); j++) {
            ma[i][j] = a[i][j];
        }
    }
    for (int i = 0; i < mb.Nrows(); i++) {
        for (int j = 0; j < mb.Ncolumns(); j++) {
            mb[i][j] = b[i][j];
        }
    }
    ASSERT_EQ(ma.Submatrix(2, 0), mb);
}

TEST(MatrixTest, CalculatingAMinorOfA3x3Matrix) {
    double a[][3] {
        { 3, 5, 0 },
        { 2, -1, -7 },
        { 6, -1, 5 }
    };
    Matrix3x3 ma { a };
    ASSERT_DOUBLE_EQ(ma.Minor(1, 0), 25);
}

TEST(MatrixTest, CalculatingACofactorOfA3x3Matrix) {
    double a[][3] {
        { 3, 5, 0 },
        { 2, -1, -7 },
        { 6, -1, 5 }
    };
    Matrix3x3 ma { a };
    ASSERT_DOUBLE_EQ(ma.Cofactor(0, 0), -12);
    ASSERT_DOUBLE_EQ(ma.Cofactor(1, 0), -25);
}

TEST(MatrixTest, CalculatingDeterminantOfA3x3Matrix) {
    double a[][3] {
        { 1 , 2, 6 },
        { -5, 8, -4 },
        { 2, 6, 4 }
    };
    Matrix3x3 ma { a };
    ASSERT_DOUBLE_EQ(ma.Determinant(), -196);
}

TEST(MatrixTest, CalculatingDeterminantOfA4x4Matrix) {
    double a[][4] {
        { -2, -8, 3, 5 },
        { -3, 1, 7, 3 },
        { 1, 2, -9, 6},
        { -6, 7, 7, -9 }
    };
    double b[][3] {
        { -3, 7, 3 },
        { 1, -9, 6 },
        { -6, 7, -9}
    };
    Matrix4x4 ma { a };
    ASSERT_DOUBLE_EQ(ma.Determinant(), -4071);
}