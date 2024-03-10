#ifndef RAY_TRACER_TYPES_H
#define RAY_TRACER_TYPES_H

#include <string>

class Tuple {
    public:
        static const double kAbsEpsilon;
        static const double kRelEpsilon;
        static bool RelEqual(double, double, double);
        static bool AbsEqual(double, double, double, double);

        Tuple(double x, double y, double z, double w);
        bool operator==(const Tuple &t) const;
        bool operator!=(const Tuple &t) const;
        Tuple operator+(const Tuple &t) const;
        Tuple operator-(const Tuple &t) const;
        Tuple operator-() const;
        Tuple operator*(const double &d) const;
        Tuple operator/(const double &d) const;
        double Magnitude() const;
        Tuple Normalize() const;
        std::string ToString() const;
        virtual std::string ClassName() const;

        double x_;
        double y_;
        double z_;
        double w_;
};

class Vector : public Tuple {
    public:
        static double DotProduct(const Vector &v1, const Vector &v2);
        static Vector CrossProduct(const Vector &v1, const Vector &v2);
        Vector(double x, double y, double z);
        Tuple operator-(const Tuple &t) const;
        std::string ClassName() const;
        Vector Normalize() const;
        Vector operator*(const double &d) const;
        Vector operator+(const Vector &v) const;
};

class Point : public Tuple {
    public:
        Point(double x, double y, double z);
        Tuple operator+(const Tuple &v) const;
        Point operator+(const Vector &v) const;
        double Magnitude() const;
        std::string ClassName() const;
};

#endif