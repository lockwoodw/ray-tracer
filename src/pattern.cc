#include "pattern.h"

#include <cmath>

const Colour Pattern::ObjectColourAt(const Shape* object, const Point& world_point) const {
    Point object_point = object->Transform().Inverse() * world_point,
          pattern_point = transform_.Inverse() * object_point;
    return ColourAt(pattern_point);
}

const Colour StripePattern::ColourAt(const Point& p) const {
    return (static_cast<int>(std::floor(p.X())) % 2) == 0 ? a_ : b_;
}

bool StripePattern::operator==(const Pattern& p) const {
    const StripePattern* other = dynamic_cast<const StripePattern*>(&p);
    return (other != nullptr) ? ((a_ == other->a_) && (b_ == other->b_)) : false;
}

const Colour GradientPattern::ColourAt(const Point& p) const {
    // blending function: calculate distance between colours a_ and b_;
    // multiply distance by fractional portion of x coordinate;
    // add this product to the base colour
    return a_ + ((b_ - a_) * (p.X() - std::floor(p.X())));
}

bool GradientPattern::operator==(const Pattern& p) const {
    const GradientPattern* other = dynamic_cast<const GradientPattern*>(&p);
    return (other != nullptr) ? ((a_ == other->a_) && (b_ == other->b_)) : false;
}

const Colour RingPattern::ColourAt(const Point& p) const {
    // test distance between a_ and b_ in both x and z dimensions
    double x = p.X(),
           z = p.Z(),
           distance = std::floor(std::sqrt(x * x + z * z));
    return (static_cast<int>(distance) % 2 == 0) ? a_ : b_;
}

bool RingPattern::operator==(const Pattern& p) const {
    const RingPattern* other = dynamic_cast<const RingPattern*>(&p);
    return (other != nullptr) ? ((a_ == other->a_) && (b_ == other->b_)) : false;
}

const Colour CheckerPattern::ColourAt(const Point& p) const {
    // apply stripe algorithm but in all three dimensions
    return (static_cast<int>(std::floor(p.X()) + std::floor(p.Y()) + std::floor(p.Z())) % 2 == 0) ? a_ : b_;
}

bool CheckerPattern::operator==(const Pattern& p) const {
    const CheckerPattern* other = dynamic_cast<const CheckerPattern*>(&p);
    return (other != nullptr) ? ((a_ == other->a_) && (b_ == other->b_)) : false;
}

const Colour RadialGradientPattern::ColourAt(const Point& p) const {
    // Combine ring and gradient patterns: each band in the ring will be a gradient.
    // Determine the starting pattern for the gradient, based on the band.
    double x = p.X(),
           z = p.Z(),
           distance = std::floor(std::sqrt(x * x + z * z));
    Colour starting_colour = (static_cast<int>(distance) % 2 == 0) ? a_ : b_,
           ending_colour = starting_colour == a_ ? b_ : a_;
    return starting_colour + ((ending_colour - starting_colour) * (p.X() - std::floor(p.X())));
}

bool RadialGradientPattern::operator==(const Pattern& p) const {
    const RadialGradientPattern* other = dynamic_cast<const RadialGradientPattern*>(&p);
    return (other != nullptr) ? ((a_ == other->a_) && (b_ == other->b_)) : false;
}

// Override the base class method because we need to calculate the world point
// in pattern space for both patterns
const Colour BlendedPattern::ObjectColourAt(const Shape* object, const Point& world_point) const {
    Point object_point = object->Transform().Inverse() * world_point;
    return ColourAt(object_point);
}

const Colour BlendedPattern::ColourAt(const Point& p) const {
    // In this case, p is an object point, so we need to convert it to the
    // pattern space, once per pattern.
    Point pattern_point_a = a_->Transform().Inverse() * p,
          pattern_point_b = b_->Transform().Inverse() * p;
    // Take average of two patterns.
    Colour ca = a_->ColourAt(pattern_point_a),
           cb = b_->ColourAt(pattern_point_b),
           sum = ca + cb;
    return sum / 2.0;
}

bool BlendedPattern::operator==(const Pattern& p) const {
    const BlendedPattern* other = dynamic_cast<const BlendedPattern*>(&p);
    return (other != nullptr) ? ((*a_ == *(other->a_)) && (*b_ == *(other->b_))) : false;
}

bool SolidPattern::operator==(const Pattern& p) const {
    const SolidPattern* other = dynamic_cast<const SolidPattern*>(&p);
    return (other != nullptr) ? (colour_ == other->colour_) : false;
}