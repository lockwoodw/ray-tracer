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
    ASSERT_DOUBLE_EQ(data[0][0], m.At(0, 0));
    ASSERT_DOUBLE_EQ(data[0][3], m.At(0, 3));
    ASSERT_DOUBLE_EQ(data[1][2], m.At(1, 2));
    ASSERT_DOUBLE_EQ(data[3][2], m.At(3, 2));
}

TEST(MatrixTest, SettingAnElementInA4x4Matrix) {
    Matrix4x4 m;
    m.At(0, 0) = 2.5;
    m.At(3, 3) = 42;
    ASSERT_DOUBLE_EQ(2.5, m.At(0, 0));
    ASSERT_DOUBLE_EQ(42, m.At(3, 3));
    ASSERT_DOUBLE_EQ(0, m.At(1, 1));
}

TEST(MatrixTest, SettingAnElementInA4x4MatrixOutOfBounds) {
    Matrix4x4 m;
    ASSERT_ANY_THROW(m.At(0, 4) = 2.5);
    ASSERT_ANY_THROW(m.At(-1, 2) = 3.5);
    ASSERT_ANY_THROW(m.At(4, 0) = 4.5);
    ASSERT_ANY_THROW(m.At(0, -4) = 42.5);
}

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
    Matrix4x4 product = ma * mb;
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
            ma.At(i, j) = a4x2[i][j];
        }
    }
    for (int i = 0; i < mb.Nrows(); i++) {
        for (int j = 0; j < mb.Ncolumns(); j++) {
            mb.At(i, j) = b2x3[i][j];
        }
    }
    Matrix *product = ma * mb;
    for (int i = 0; i < product->Nrows(); i++) {
        for (int j = 0; j < product->Ncolumns(); j++) {
            ASSERT_FLOAT_EQ(product->At(i, j), c4x3[i][j]);
        }
    }
    delete product;
}

TEST(MatrixTest, MultiplyingTwoMatricesWithoutACommonDimension) {
    Matrix2x2 m2x2;
    Matrix3x3 m3x3;
    Matrix *product = nullptr;
    ASSERT_ANY_THROW(product = m2x2 * m3x3);
    delete product;
}

TEST(MatrixTest, MultiplyingAMatrixByATuple) {
    double a[][4] {
        { 1, 2, 3, 4 },
        { 2, 4, 4, 2 },
        { 8, 6, 4, 1 },
        { 0, 0, 0, 1 }
    };
    Matrix4x4 ma { a };
    Tuple b { 1, 2, 3, 1 }, expected { 18, 24, 33, 1 };
    ASSERT_EQ(ma * b, expected);
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