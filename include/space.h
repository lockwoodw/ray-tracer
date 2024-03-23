#ifndef RAY_TRACER_SPACE_H
#define RAY_TRACER_SPACE_H

#include "tuple.h"

class Point; // friend of Vector

class Vector {
    Tuple tuple_;

    public:
        enum { x_, y_, z_, w_ };
        static double DotProduct(const Vector& v1, const Vector& v2);
        static Vector CrossProduct(const Vector& v1, const Vector& v2);

        Vector(): tuple_ { 4 } {}

        Vector(double x, double y, double z): tuple_ { 4 } {
            tuple_[x_] = x;
            tuple_[y_] = y;
            tuple_[z_] = z;
            tuple_[w_] = 0.0; // by definition
        }

        Vector(const Tuple& t);

        Vector operator+(const Vector& v) const;
        Vector operator-(const Vector& v) const;
        Vector operator*(double d) const;
        Vector operator/(double d) const;
        double& operator[](int index);
        double At(int index) const;

        bool operator==(const Tuple& t) const;
        bool operator!=(const Tuple& t) const;
        bool operator==(const Vector& v) const;
        bool operator!=(const Vector& v) const;

        double Magnitude() const;
        Vector Normalize() const;

        friend Point;
};

class Point {
    Tuple tuple_;

    public:
        enum { x_, y_, z_, w_ };

        Point(): tuple_ { 4 } {
            tuple_[w_] = 1.0;
        }

        Point(double x, double y, double z): tuple_ { 4 } {
            tuple_[x_] = x;
            tuple_[y_] = y;
            tuple_[z_] = z;
            tuple_[w_] = 1.0; // by definition
        }

        Point(const Tuple& t);

        Point operator+(const Vector& v) const;
        Point operator-(const Vector& v) const;
        Vector operator-(const Point& p) const;
        Point operator*(double d) const;
        Point operator/(double d) const;
        double& operator[](int index);
        double At(int index) const;

        bool operator==(const Tuple& t) const;
        bool operator!=(const Tuple& t) const;
        bool operator==(const Point& p) const;
        bool operator!=(const Point& p) const;
};

#endif