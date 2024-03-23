#ifndef RAY_TRACER_TYPES_H
#define RAY_TRACER_TYPES_H

#include <array>
#include <stdexcept>
#include <iostream>

class Tuple {
    double *elements_;
    std::size_t size_;

    public:
        template <std::size_t N>
        Tuple(std::array<double, N> src) : size_ { N } {
            if (N < 0) {
                throw std::invalid_argument("Tuple size must be > 0");
            }
            elements_ = new double[N];
            for (int i = 0; i < N; i++) {
                elements_[i] = src[i];
            }
        }

        Tuple(std::size_t n, const double* src);

        Tuple(std::size_t n);

        Tuple(const Tuple& t);

        ~Tuple() {
            delete[] elements_;
        }

        bool operator==(const Tuple& t) const;
        bool operator!=(const Tuple& t) const;
        Tuple operator+(const Tuple& t) const;
        Tuple operator-(const Tuple& t) const;
        Tuple operator-() const;
        Tuple operator*(double d) const;
        // Tuple& operator*=(double d);
        Tuple operator/(double d) const;
        double& operator[](std::size_t index);
        double At(std::size_t index) const;

        int Size() const { return size_; }

        friend std::ostream& operator<<(std::ostream& os, const Tuple& t);

        // // Tuple(double x, double y, double z, double w);
        // bool operator==(const Tuple& t) const;
        // bool operator!=(const Tuple& t) const;
        // Tuple operator+(const Tuple& t) const;
        // Tuple operator-(const Tuple& t) const;
        // Tuple operator-() const;
        // Tuple operator*(const double& d) const;
        // Tuple operator/(const double& d) const;
        // double Magnitude() const;
        // Tuple Normalize() const;
        // std::string ToString() const;

        // double x_;
        // double y_;
        // double z_;
        // double w_;
};

// factory method for common tuple size
Tuple get_4_tuple(double x, double y, double z, double w);

/*
class Vector : public Tuple {
    public:
        static double DotProduct(const Vector& v1, const Vector& v2);
        static Vector CrossProduct(const Vector& v1, const Vector& v2);
        Vector(double x, double y, double z);
        Tuple operator-(const Tuple& t) const;
        Vector Normalize() const;
        Vector operator*(const double& d) const;
        Vector operator+(const Vector& v) const;
};

class Point : public Tuple {
    public:
        Point(double x, double y, double z);
        Tuple operator+(const Tuple& v) const;
        Point operator+(const Vector& v) const;
        double Magnitude() const;
};
*/

#endif