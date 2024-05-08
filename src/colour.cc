#include "colour.h"

Colour::Colour(const Tuple& t): tuple_ { t } {
    if (t.Size() != 3) {
        throw std::invalid_argument("Incorrect tuple size for Colour");
    }
}

Colour Colour::operator+(const Colour& c) const {
    return Colour { tuple_ + c.tuple_ };
}

Colour& Colour::operator+=(const Colour& c) {
    tuple_ += c.tuple_;
    return *this;
}

Colour Colour::operator-(const Colour& c) const {
    return Colour { tuple_ - c.tuple_ };
}

Colour Colour::operator*(double d) const {
    return Colour { tuple_ * d };
}

Colour Colour::operator*(const Colour& c) const {
    return Colour {
        tuple_.At(kRed) * c.tuple_.At(kRed),
        tuple_.At(kGreen) * c.tuple_.At(kGreen),
        tuple_.At(kBlue) * c.tuple_.At(kBlue)
    };
}

Colour Colour::operator/(double d) const {
    return Colour { tuple_ / d };
}

bool Colour::operator==(const Colour& c) const {
    return tuple_ == c.tuple_;
}

bool Colour::operator!=(const Colour& c) const {
    return tuple_ != c.tuple_;
}

Colour& Colour::operator=(const Colour& c) {
    for (int i = 0; i < tuple_.Size(); i++) {
        tuple_[i] = c.tuple_.At(i);
    }
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Colour& c) {
    os << c.tuple_;
    return os;
}