#include <stdexcept>
#include "group.h"

void ShapeGroup::Add(Shape* s) {
    s->Parent(this);
    shapes_.push_back(s);
}

ShapeGroup& ShapeGroup::operator<<(Shape *s) {
    Add(s);
    return *this;
}

bool ShapeGroup::Contains(Shape* shape) const {
    for (auto& s: shapes_) {
        if (s == shape) {
            return true;
        }
    }
    return false;
}

bool ShapeGroup::operator==(const Shape& s) const {
    const ShapeGroup* other = dynamic_cast<const ShapeGroup*>(&s);
    if (other == nullptr) { // Shape is not an ShapeGroup?
        return false;
    }
    return origin_ == other->origin_ && shapes_ == other->shapes_;
}

bool ShapeGroup::Intersect(IntersectionList& list, const Ray& ray) const {
    bool intersected { false };
    Ray local_ray = ray.Transform(inverse_transform_);
    if (BoundsOf().Intersects(local_ray)) {
        for (auto s: shapes_) {
            if (s->Intersect(list, local_ray)) {
                intersected = true;
            }
        }
    }
    return intersected;
}

Vector ShapeGroup::LocalNormalAt(const Point &object_point) const {
    throw std::runtime_error("Can't call LocalNormalAt() on a group!");
}

const BoundingBox ShapeGroup::BoundsOf() const {
    BoundingBox box {};
    for (auto s: shapes_) {
        box.Add(s->BoundsOfInParentSpace());
    }
    return box;
}