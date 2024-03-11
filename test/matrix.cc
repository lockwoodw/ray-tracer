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
