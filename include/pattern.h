#ifndef RAY_TRACER_PATTERN_H
#define RAY_TRACER_PATTERN_H

#include <vector>

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

class SolidPattern: public Pattern {
    Colour colour_;

    public:
        SolidPattern(): Pattern {}, colour_ { Colour { 0, 0, 0 } } {}
        SolidPattern(const Colour& colour): Pattern {}, colour_ { colour } {}
        SolidPattern(const SolidPattern& sp): Pattern { sp }, colour_ { sp.colour_ } {}
        const Colour ColourAt(const Point& p) const override { return colour_; }
        bool operator==(const Pattern& p) const override;
};

class TwoColourMetaPattern: public Pattern {
    protected:
        const Pattern* a_;
        const Pattern* b_;
        const bool owned_;

    public:
        TwoColourMetaPattern(const Colour& a, const Colour& b);
        TwoColourMetaPattern(const Pattern* a, const Pattern* b): Pattern {}, a_ { a }, b_ { b }, owned_ { false } {}
        TwoColourMetaPattern(const TwoColourMetaPattern& mp): Pattern { mp }, a_ { mp.a_ }, b_ { mp.b_ },  owned_ { false } {}
        virtual ~TwoColourMetaPattern();

        const Pattern* A() const {
            return a_;
        }
        const Pattern* B() const {
            return b_;
        }

        const Colour ObjectColourAt(const Shape* object, const Point& world_point) const override;
        virtual const Colour ColourAt(const Point& p) const = 0;
        virtual bool operator==(const Pattern& p) const = 0;
};

class StripePattern: public TwoColourMetaPattern {
    public:
        StripePattern(const Colour& a, const Colour& b): TwoColourMetaPattern { a, b } {}
        StripePattern(const Pattern* a, const Pattern* b): TwoColourMetaPattern { a, b } {}
        StripePattern(const StripePattern& sp): TwoColourMetaPattern { sp } {}
        const Colour ColourAt(const Point& p) const override;
        bool operator==(const Pattern& p) const override;
};

class GradientPattern: public TwoColourMetaPattern {
    public:
        GradientPattern(const Colour& a, const Colour& b): TwoColourMetaPattern { a, b } {}
        GradientPattern(const Pattern* a, const Pattern* b): TwoColourMetaPattern { a, b } {}
        GradientPattern(const GradientPattern& gp): TwoColourMetaPattern { gp } {}
        const Colour ColourAt(const Point& p) const override;
        bool operator==(const Pattern& p) const override;
};

class RingPattern: public TwoColourMetaPattern {
    public:
        RingPattern(const Colour& a, const Colour& b): TwoColourMetaPattern { a, b } {}
        RingPattern(const Pattern* a, const Pattern* b): TwoColourMetaPattern { a, b } {}
        RingPattern(const RingPattern& rp): TwoColourMetaPattern { rp } {}
        const Colour ColourAt(const Point& p) const override;
        bool operator==(const Pattern& p) const override;
};

class CheckerPattern: public TwoColourMetaPattern {
    public:
        CheckerPattern(const Colour& a, const Colour& b): TwoColourMetaPattern { a, b } {}
        CheckerPattern(const Pattern* a, const Pattern* b): TwoColourMetaPattern { a, b } {}
        CheckerPattern(const CheckerPattern& cp): TwoColourMetaPattern { cp } {}
        const Colour ColourAt(const Point& p) const override;
        bool operator==(const Pattern& p) const override;
};

class RadialGradientPattern: public TwoColourMetaPattern {
    public:
        RadialGradientPattern(const Colour& a, const Colour& b): TwoColourMetaPattern { a, b } {}
        RadialGradientPattern(const Pattern* a, const Pattern* b): TwoColourMetaPattern { a, b } {}
        RadialGradientPattern(const RadialGradientPattern& rgp): TwoColourMetaPattern { rgp } {}
        const Colour ColourAt(const Point& p) const override;
        bool operator==(const Pattern& p) const override;
};

class PatternBlender {
    public:
        PatternBlender() {}
        virtual ~PatternBlender() {}
        virtual Colour Blend(std::vector<Pattern*> patterns, const Point& object_point) const = 0;
        virtual bool operator==(const PatternBlender& pb) const = 0;
};

class AveragePatternBlender: public PatternBlender {
    public:
        AveragePatternBlender(): PatternBlender {} {}
        Colour Blend(std::vector<Pattern*> patterns, const Point& object_point) const override;
        bool operator==(const PatternBlender& pb) const override;
};

class BlendedPattern: public Pattern {
    protected:
        PatternBlender* blender_;
        std::vector<Pattern*> patterns_;

    public:
        BlendedPattern(PatternBlender* blender): Pattern {}, patterns_ {}, blender_ { blender } {}
        BlendedPattern(const BlendedPattern& bp): Pattern { bp }, patterns_ {}, blender_ { bp.blender_ } {
            for (Pattern* pattern : bp.patterns_) {
                patterns_.push_back(pattern);
            }
        }

        BlendedPattern& Add(Pattern* pattern) {
            patterns_.push_back(pattern);
            return *this;
        }

        const Colour ObjectColourAt(const Shape* object, const Point& world_point) const override;
        const Colour ColourAt(const Point& p) const override;
        bool operator==(const Pattern& p) const override;
};

// Implement Ken Perlin's improved noise algorithm, copyrighted 2002
// See https://mrl.cs.nyu.edu/~perlin/noise/
class PerlinNoise {
    const static int kPermutation[256];
    static int kP[512];

    public:
        PerlinNoise();
        double Fade(double t) const {
            return t * t * t * (t * (t * 6 - 15) + 10);
        }
        double Lerp(double t, double a, double b) const {
            return a + t * (b - a);
        }
        double Grad(int hash, double x, double y, double z) const;
        double Noise(const Point& p) const;
};

class PerturbedPattern: public Pattern {
    protected:
        const Pattern* pattern_;
        PerlinNoise generator_;

    public:
        PerturbedPattern(const Pattern* pattern): Pattern {}, pattern_ { pattern }, generator_ { PerlinNoise() } {}
        const Colour ObjectColourAt(const Shape* object, const Point& world_point) const override;
        const Colour ColourAt(const Point& p) const override;
        bool operator==(const Pattern& p) const override;
};

class TestPattern: public Pattern {
    public:
        TestPattern(): Pattern {} {}
        const Colour ColourAt(const Point& p) const {
            return Colour { p.X(), p.Y(), p.Z() };
        }
        bool operator==(const Pattern& p) const {
            const TestPattern* other = dynamic_cast<const TestPattern*>(&p);
            return other != nullptr;
        }
};

#endif