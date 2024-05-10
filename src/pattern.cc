#include "pattern.h"

#include <cmath>

const Colour Pattern::ObjectColourAt(const Shape* object, const Point& world_point) const {
    Point object_point = object->Transform().Inverse() * world_point,
          pattern_point = transform_.Inverse() * object_point;
    return ColourAt(pattern_point);
}

bool SolidPattern::operator==(const Pattern& p) const {
    const SolidPattern* other = dynamic_cast<const SolidPattern*>(&p);
    return (other != nullptr) ? (colour_ == other->colour_) : false;
}

TwoColourMetaPattern::TwoColourMetaPattern(const Colour& a, const Colour& b): Pattern {}, owned_ { true } {
    a_ = new SolidPattern(a);
    b_ = new SolidPattern(b);
}

TwoColourMetaPattern::~TwoColourMetaPattern() {
    if (owned_) {
        delete a_;
        delete b_;
    }
}

// Override the base class method because we need to calculate the world point
// in pattern space for all patterns
const Colour TwoColourMetaPattern::ObjectColourAt(const Shape* object, const Point& world_point) const {
    Point object_point = object->Transform().Inverse() * world_point;
    return ColourAt(object_point);
}

const Colour StripePattern::ColourAt(const Point& object_point) const {
    Point p = transform_.Inverse() * object_point;
    const Pattern* pattern = (static_cast<int>(std::floor(p.X())) % 2) == 0 ? a_ : b_;
    return pattern->ColourAt(object_point);
}

bool StripePattern::operator==(const Pattern& p) const {
    const StripePattern* other = dynamic_cast<const StripePattern*>(&p);
    return (other != nullptr) && (owned_ == other->owned_) && (*a_ == *other->a_) && (*b_ == *other->b_);
}

const Colour GradientPattern::ColourAt(const Point& object_point) const {
    // Blending function: calculate distance between colours a and b;
    // multiply distance by fractional portion of x coordinate;
    // add this product to the base colour
    Point p = transform_.Inverse() * object_point;
    Colour a = a_->ColourAt(object_point),
           b = b_->ColourAt(object_point);
    return a + ((b - a) * (p.X() - std::floor(p.X())));
}

bool GradientPattern::operator==(const Pattern& p) const {
    const GradientPattern* other = dynamic_cast<const GradientPattern*>(&p);
    return (other != nullptr) && (owned_ == other->owned_) && (*a_ == *other->a_) && (*b_ == *other->b_);
}

const Colour RingPattern::ColourAt(const Point& object_point) const {
    // Test distance between a_ and b_ in both x and z dimensions
    Point p = transform_.Inverse() * object_point;
    double x = p.X(),
           z = p.Z(),
           distance = std::floor(std::sqrt(x * x + z * z));
    const Pattern* pattern = (static_cast<int>(distance) % 2 == 0) ? a_ : b_;
    return pattern->ColourAt(object_point);
}

bool RingPattern::operator==(const Pattern& p) const {
    const RingPattern* other = dynamic_cast<const RingPattern*>(&p);
    return (other != nullptr) && (owned_ == other->owned_) && (*a_ == *other->a_) && (*b_ == *other->b_);
}

const Colour CheckerPattern::ColourAt(const Point& object_point) const {
    // Apply stripe algorithm but in all three dimensions
    Point p = transform_.Inverse() * object_point;
    const Pattern* pattern = (static_cast<int>(std::floor(p.X()) + std::floor(p.Y()) + std::floor(p.Z())) % 2 == 0)
        ? a_ : b_;
    return pattern->ColourAt(object_point);
}

bool CheckerPattern::operator==(const Pattern& p) const {
    const CheckerPattern* other = dynamic_cast<const CheckerPattern*>(&p);
    return (other != nullptr) && (owned_ == other->owned_) && (*a_ == *other->a_) && (*b_ == *other->b_);
}

const Colour RadialGradientPattern::ColourAt(const Point& object_point) const {
    // Combine ring and gradient patterns: each band in the ring will be a gradient.
    // Determine the starting pattern for the gradient, based on the band.
    Point p = transform_.Inverse() * object_point;
    double x = p.X(),
           z = p.Z(),
           distance = std::floor(std::sqrt(x * x + z * z));
    Colour a = a_->ColourAt(object_point),
           b = b_->ColourAt(object_point),
           starting_colour = (static_cast<int>(distance) % 2 == 0) ? a : b,
           ending_colour = (starting_colour == a) ? b : a;
    return starting_colour + ((ending_colour - starting_colour) * (p.X() - std::floor(p.X())));
}

bool RadialGradientPattern::operator==(const Pattern& p) const {
    const RadialGradientPattern* other = dynamic_cast<const RadialGradientPattern*>(&p);
    return (other != nullptr) && (owned_ == other->owned_) && (*a_ == *other->a_) && (*b_ == *other->b_);
}

Colour AveragePatternBlender::Blend(std::vector<Pattern*> patterns, const Point& object_point) const {
    Colour sum { 0, 0, 0 };
    for (Pattern* pattern: patterns) {
        sum += pattern->ColourAt(object_point);
    }
    return sum / patterns.size();
}

bool AveragePatternBlender::operator==(const PatternBlender& pb) const {
    const AveragePatternBlender* other = dynamic_cast<const AveragePatternBlender*>(&pb);
    return other != nullptr;
}

// Override the base class method because we need to calculate the world point
// in pattern space for both patterns
const Colour BlendedPattern::ObjectColourAt(const Shape* object, const Point& world_point) const {
    Point object_point = object->Transform().Inverse() * world_point;
    return ColourAt(object_point);
}

const Colour BlendedPattern::ColourAt(const Point& p) const {
    return blender_->Blend(patterns_, p);
}

bool BlendedPattern::operator==(const Pattern& p) const {
    const BlendedPattern* other = dynamic_cast<const BlendedPattern*>(&p);
    return (other != nullptr) && (*blender_ == *other->blender_) && (patterns_ == other->patterns_);
}
