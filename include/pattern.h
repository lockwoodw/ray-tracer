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
        Pattern(const Pattern& p): transform_ { p.transform_ } {}
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
        StripePattern(const StripePattern& sp): Pattern { sp } {
            a_ = sp.a_;
            b_ = sp.b_;
        }
        const Colour ColourAt(const Point& p) const override;
        bool operator==(const Pattern& p) const override;
        const Colour A() const {
            return a_;
        }
        const Colour B() const {
            return b_;
        }
};

class GradientPattern: public Pattern {
    Colour a_;
    Colour b_;

    public:
        GradientPattern(const Colour& a, const Colour& b): Pattern {}, a_ { a }, b_ { b } {}
        GradientPattern(const GradientPattern& sp): Pattern { sp } {
            a_ = sp.a_;
            b_ = sp.b_;
        }
        const Colour ColourAt(const Point& p) const override;
        bool operator==(const Pattern& p) const override;
        const Colour A() const {
            return a_;
        }
        const Colour B() const {
            return b_;
        }
};

#endif