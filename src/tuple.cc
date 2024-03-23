#include <cmath>
#include <stdexcept>
#include <cstring> // memset
#include "tuple.h"
#include "utils.h"

Tuple::Tuple(std::size_t n, const double* src) : size_ { n } {
    if (n < 0) {
        throw std::invalid_argument("Tuple size must be > 0");
    }
    elements_ = new double[n];
    for (int i = 0; i < n; i++) {
        elements_[i] = src[i];
    }
}

Tuple::Tuple(std::size_t n): size_ { n } {
    if (n < 0) {
        throw std::invalid_argument("Tuple size must be > 0");
    }
    elements_ = new double[n];
    // initialize all values to zero
    memset(elements_, 0, n * sizeof(double));
}

Tuple::Tuple(const Tuple& t): size_ { t.size_ } {
    elements_ = new double[size_];
    for (int i = 0; i < size_; i++) {
        elements_[i] = t.At(i);
    }
}

bool Tuple::operator==(const Tuple& t) const {
    if (size_ != t.size_) {
        return false;
    }
    for (int i = 0; i < size_; i++) {
        if (!floating_point_compare(elements_[i], t.elements_[i])) {
            return false;
        }
    }
    return true;
}

bool Tuple::operator!=(const Tuple& t) const {
    return ! operator==(t);
}

Tuple Tuple::operator+(const Tuple& t) const {
    if (size_ != t.size_) {
        throw std::invalid_argument("Addition not possible: tuples are not same size");
    }
    Tuple sum { size_ };
    for (int i = 0; i < size_; i++) {
        sum.elements_[i] = elements_[i] + t.elements_[i];
    }
    return sum;
}

Tuple Tuple::operator-(const Tuple& t) const {
    if (size_ != t.size_) {
        throw std::invalid_argument("Subtraction not possible: tuples are not same size");
    }
    Tuple difference { size_ };
    for (int i = 0; i < size_; i++) {
        difference.elements_[i] = elements_[i] - t.elements_[i];
    }
    return difference;
}

Tuple Tuple::operator-() const {
    Tuple negated { size_ };
    for (int i = 0; i < size_; i++) {
        negated.elements_[i] = -elements_[i];
    }
    return negated;
}

Tuple Tuple::operator*(double d) const {
    Tuple scaled { size_ };
    for (int i = 0; i < size_; i++) {
        scaled.elements_[i] = elements_[i] * d;
    }
    return scaled;
}

// Tuple& Tuple::operator*=(double d) {
//     for (int i = 0; i < size_; i++) {
//         elements_[i] *= d;
//     }
//     return *this;
// }

Tuple Tuple::operator/(double d) const {
    if (d == 0.0) {
        throw std::invalid_argument("Divide by zero attempted");
    }
    Tuple scaled { size_ };
    for (int i = 0; i < size_; i++) {
        scaled.elements_[i] = elements_[i] / d;
    }
    return scaled;
}

double& Tuple::operator[](std::size_t index) {
    if (index < 0 || index >= size_) {
        throw std::out_of_range("Requested index is out of range");
    }
    return elements_[index];
}

double Tuple::At(std::size_t index) const {
    if (index < 0 || index >= size_) {
        throw std::out_of_range("Requested index is out of range");
    }
    return elements_[index];
}

std::ostream& operator<<(std::ostream& os, const Tuple& t) {
    os << "[ ";
    for (int i = 0; i < t.size_; i++) {
        os << (i > 0 ? ", " : "") << t.elements_[i];
    }
    os << " ]" << std::endl;
    return os;
}

Tuple get_4_tuple(double x, double y, double z, double w) {
    Tuple p { 4 };
    p[0] = x;
    p[1] = y;
    p[2] = z;
    p[3] = w;
    return p;
}