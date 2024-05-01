#ifndef RAY_TRACER_PATTERN_H
#define RAY_TRACER_PATTERN_H

#include "shape.h"
#include "matrix.h"
#include "colour.h"
#include "space.h"

class Pattern {
    protected:
        Matrix transform_;

    public:
        Pattern(): transform_ { Matrix::Identity(4) } {}
        virtual ~Pattern() {}

        void SetTransform(const Matrix& m) {
            transform_ *= m;
        }

        const Matrix& Transform() const {
            return transform_;
        }

        const Colour ObjectColourAt(const Shape* object, const Point& world_point) const;
        virtual const Colour ColourAt(const Point& p) const = 0;
        virtual bool operator==(const Pattern& p) const = 0;
};

class StripePattern: public Pattern {
    Colour a_;
    Colour b_;

    public:
        StripePattern(const Colour& a, const Colour& b): Pattern {}, a_ { a }, b_ { b } {}
        const Colour ColourAt(const Point& p) const override;
        const Colour A() const {
            return a_;
        }
        const Colour B() const {
            return b_;
        }
        bool operator==(const Pattern& p) const override;
};

#endif