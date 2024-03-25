#ifndef RAY_TRACER_SPACE_H
#define RAY_TRACER_SPACE_H

#include "tuple.h"

class Vector: public Tuple {
    public:
        enum { kX, kY, kZ, kW };
        static double DotProduct(const Vector& v1, const Vector& v2);
        static Vector CrossProduct(const Vector& v1, const Vector& v2);

        Vector(): Tuple { 4 } {
            elements_[kW] = 0.0; // by definition
        }

        Vector(double x, double y, double z): Tuple { 4 } {
            elements_[kX] = x;
            elements_[kY] = y;
            elements_[kZ] = z;
            elements_[kW] = 0.0;
        }

        Vector(const Tuple& t);

        double X() { return elements_[kX]; }
        double Y() { return elements_[kY]; }
        double Z() { return elements_[kZ]; }

        double Magnitude() const;
        Vector Normalize() const;
};

class Point: public Tuple {
    public:
        enum { kX, kY, kZ, kW };

        Point(): Tuple { 4 } {
            elements_[kW] = 1.0; // by definition
        }

        Point(double x, double y, double z): Tuple { 4 } {
            elements_[kX] = x;
            elements_[kY] = y;
            elements_[kZ] = z;
            elements_[kW] = 1.0;
        }

        Point(const Tuple& t);

        double X() { return elements_[kX]; }
        double Y() { return elements_[kY]; }
        double Z() { return elements_[kZ]; }
};

#endif