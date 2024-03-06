#ifndef RAY_TRACER_TYPES_H
#define RAY_TRACER_TYPES_H

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
        double magnitude() const;
        Tuple normalize() const;

        double x_;
        double y_;
        double z_;
        double w_;
};

class Point : public Tuple {
    public:
        Point(double x, double y, double z);
        Tuple operator+(const Tuple &t) const;
        double magnitude() const;
};

class Vector : public Tuple {
    public:
        Vector(double x, double y, double z);
        Tuple operator-(const Tuple &t) const;
};

#endif