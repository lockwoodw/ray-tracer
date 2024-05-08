#ifndef RAY_TRACER_COLOUR_H
#define RAY_TRACER_COLOUR_H

#include <iostream>
#include "tuple.h"

class Colour {
    Tuple tuple_;

    public:
        enum { kRed, kGreen, kBlue };

        Colour(): tuple_ { 3 } {}

        Colour(double red, double green, double blue): tuple_ { 3 } {
            tuple_[kRed] = red;
            tuple_[kGreen] = green;
            tuple_[kBlue] = blue;
        }

        Colour(const Tuple& t);

        Colour(const Colour& c): tuple_ { c.tuple_ } {}

        double Red() { return tuple_[kRed]; }
        double Green() { return tuple_[kGreen]; }
        double Blue() { return tuple_[kBlue]; }

        Colour operator+(const Colour& c) const;
        Colour& operator+=(const Colour& c);
        Colour operator-(const Colour& c) const;
        Colour operator*(double d) const;
        Colour operator*(const Colour& c) const; // Hadamard or Schur product
        Colour operator/(double d) const;
        bool operator==(const Colour& c) const;
        bool operator!=(const Colour& c) const;
        Colour& operator=(const Colour& c);

        friend std::ostream& operator<<(std::ostream& os, const Colour& c);
};

#endif