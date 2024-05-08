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

        virtual const Colour ObjectColourAt(const Shape* object, const Point& world_point) const;
        virtual const Colour ColourAt(const Point& p) const = 0;
        virtual bool operator==(const Pattern& p) const = 0;
};

class TwoColourPattern: public Pattern {
    protected:
        Colour a_;
        Colour b_;

    public:
        TwoColourPattern(const Colour& a, const Colour& b): Pattern {}, a_ { a }, b_ { b } {}
        TwoColourPattern(const TwoColourPattern& tcp): Pattern { tcp } {
            a_ = tcp.a_;
            b_ = tcp.b_;
        }
        virtual ~TwoColourPattern() {}

        const Colour A() const {
            return a_;
        }
        const Colour B() const {
            return b_;
        }

        virtual const Colour ColourAt(const Point& p) const = 0;
        virtual bool operator==(const Pattern& p) const = 0;
};

class StripePattern: public TwoColourPattern {
    public:
        StripePattern(const Colour& a, const Colour& b): TwoColourPattern { a, b } {}
        StripePattern(const StripePattern& sp): TwoColourPattern { sp } {}
        const Colour ColourAt(const Point& p) const override;
        bool operator==(const Pattern& p) const override;
};

class GradientPattern: public TwoColourPattern {
    public:
        GradientPattern(const Colour& a, const Colour& b): TwoColourPattern { a, b } {}
        GradientPattern(const GradientPattern& gp): TwoColourPattern { gp } {}
        const Colour ColourAt(const Point& p) const override;
        bool operator==(const Pattern& p) const override;
};

class RingPattern: public TwoColourPattern {
    public:
        RingPattern(const Colour& a, const Colour& b): TwoColourPattern { a, b } {}
        RingPattern(const RingPattern& rp): TwoColourPattern { rp } {}
        const Colour ColourAt(const Point& p) const override;
        bool operator==(const Pattern& p) const override;
};

class CheckerPattern: public TwoColourPattern {
    public:
        CheckerPattern(const Colour& a, const Colour& b): TwoColourPattern { a, b } {}
        CheckerPattern(const CheckerPattern& cp): TwoColourPattern { cp } {}
        const Colour ColourAt(const Point& p) const override;
        bool operator==(const Pattern& p) const override;
};

class RadialGradientPattern: public TwoColourPattern {
    public:
        RadialGradientPattern(const Colour& a, const Colour& b): TwoColourPattern { a, b } {}
        RadialGradientPattern(const RadialGradientPattern& rgp): TwoColourPattern { rgp } {}
        const Colour ColourAt(const Point& p) const override;
        bool operator==(const Pattern& p) const override;
};

class BlendedPattern: public Pattern {
    protected:
        Pattern* a_;
        Pattern* b_;

    public:
        BlendedPattern(Pattern* a, Pattern* b): Pattern {}, a_ { a }, b_ { b } {}
        BlendedPattern(const BlendedPattern& bp): Pattern { bp } {
            a_ = bp.a_;
            b_ = bp.b_;
        }

        Pattern* A() const {
            return a_;
        }
        Pattern* B() const {
            return b_;
        }

        const Colour ObjectColourAt(const Shape* object, const Point& world_point) const override;
        const Colour ColourAt(const Point& p) const override;
        bool operator==(const Pattern& p) const override;
};

class SolidPattern: public Pattern {
    Colour colour_;

    public:
        SolidPattern(): Pattern {}, colour_ { Colour { 0, 0, 0 } } {}
        SolidPattern(const Colour& colour): Pattern {}, colour_ { colour } {}
        SolidPattern(const SolidPattern& sp): Pattern { sp }, colour_ { sp.colour_ } {}
        const Colour ColourAt(const Point& p) const override { return colour_; }
        bool operator==(const Pattern& p) const override;
};



#endif