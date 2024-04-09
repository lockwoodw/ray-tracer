#ifndef RAY_TRACER_SPACE_H
#define RAY_TRACER_SPACE_H

#include "tuple.h"

class SpatialTuple: public Tuple {
    public:
        enum { kX, kY, kZ, kW };
        SpatialTuple(): Tuple { 4 } {}
        SpatialTuple(const Tuple& t);
        double X() const { return At(kX); }
        double Y() const { return At(kY); }
        double Z() const { return At(kZ); }
};

class Vector: public SpatialTuple {
    public:
        static double DotProduct(const Vector& v1, const Vector& v2);
        static Vector CrossProduct(const Vector& v1, const Vector& v2);
        static Vector Reflect(const Vector& in, const Vector& normal);

        Vector() {
            elements_[kW] = 0.0; // by definition
        }

        Vector(double x, double y, double z) {
            elements_[kX] = x;
            elements_[kY] = y;
            elements_[kZ] = z;
            elements_[kW] = 0.0;
        }

        Vector(const Tuple& t): SpatialTuple { t } {
            elements_[kW] = 0.0;
        }

        double Magnitude() const;
        Vector Normalize() const;
};

class Point: public SpatialTuple {
    public:
        Point() {
            elements_[kW] = 1.0; // by definition
        }

        Point(double x, double y, double z) {
            elements_[kX] = x;
            elements_[kY] = y;
            elements_[kZ] = z;
            elements_[kW] = 1.0;
        }

        Point(const Tuple& t): SpatialTuple { t } {
            elements_[kW] = 1.0;
        }
};

#endif