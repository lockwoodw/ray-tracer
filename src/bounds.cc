#include "bounds.h"

const int BoundingBox::kIndices[3] { Point::Coordinates::kX, Point::Coordinates::kY,
    Point::Coordinates::kZ  };

void BoundingBox::Add(const Point& p) {
    // resize bounding box so its min/max contain the given point
    for (auto index: kIndices) {
        double coord = p.At(index);
        if (coord < min_[index]) {
            min_[index] = coord;
        }
        if (coord > max_[index]) {
            max_[index] = coord;
        }
    }
}

void BoundingBox::Add(const BoundingBox& b) {
    // resize bounding box so its min/max contain the given box
    for (auto index: kIndices) {
        double min_coord = b.min_.At(index),
               max_coord = b.max_.At(index);
        if (min_coord < min_[index]) {
            min_[index] = min_coord;
        }
        if (max_coord > max_[index]) {
            max_[index] = max_coord;
        }
    }
}

bool BoundingBox::Contains(const Point& p) {
    for (auto index: kIndices) {
        double coord = p.At(index);
        if (coord < min_[index] || coord > max_[index]) {
            return false;
        }
    }
    return true;
}

bool BoundingBox::Contains(const BoundingBox& b) {
    for (auto index: kIndices) {
        double min_coord = b.min_.At(index),
               max_coord = b.max_.At(index);
        if (min_coord < min_[index] || max_coord > max_[index]) {
            return false;
        }
    }
    return true;
}

const BoundingBox BoundingBox::Transform(const Matrix& m) const {
    // Apply given transform to each corner of the bounding box and return the
    // result
    BoundingBox transformed {};
    std::array<Point, 8> corners = {
        min_,
        { min_.X(), min_.Y(), max_.Z() },
        { min_.X(), max_.Y(), min_.Z() },
        { min_.X(), max_.Y(), max_.Z() },
        { max_.X(), min_.Y(), min_.Z() },
        { max_.X(), min_.Y(), max_.Z() },
        { max_.X(), max_.Y(), min_.Z() },
        max_
    };

    for (auto corner: corners) {
        transformed.Add(m * corner);
    }

    return transformed;
}