#ifndef RAY_TRACER_TUPLE_H
#define RAY_TRACER_TUPLE_H

#include <array>
#include <stdexcept>
#include <iostream>

class Tuple {
    protected:
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
        Tuple& operator+=(const Tuple& t);
        Tuple operator-(const Tuple& t) const;
        Tuple operator-() const;
        Tuple operator*(double d) const;
        // Tuple& operator*=(double d);
        Tuple operator/(double d) const;
        double& operator[](std::size_t index);
        Tuple& operator=(const Tuple& t); // Rule of 3

        double At(std::size_t index) const;
        int Size() const { return size_; }

        friend std::ostream& operator<<(std::ostream& os, const Tuple& t);
};

// factory method for common tuple size
Tuple Create4DTuple(double x, double y, double z, double w);

#endif