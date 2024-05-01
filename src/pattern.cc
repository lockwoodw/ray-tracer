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
    if (other == nullptr) { // Pattern is not a StripePattern?
        return false;
    }
    return (a_ == other->a_) && (b_ == other->b_);
}