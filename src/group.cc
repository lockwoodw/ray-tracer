#include <stdexcept>
#include "group.h"

void ShapeGroup::AddShape(Shape* s) {
    s->Parent(this);
    shapes_.push_back(s);
}

ShapeGroup& ShapeGroup::operator<<(Shape *s) {
    AddShape(s);
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

void ShapeGroup::Intersect(IntersectionList& list, const Ray& ray) const {
    Ray local_ray = ray.Transform(transform_.Inverse());
    for (auto& s: shapes_) {
        s->AddIntersections(list, local_ray);
    }
}

Vector ShapeGroup::LocalNormalAt(const Point &object_point) const {
    throw std::runtime_error("Can't call LocalNormalAt() on a group!");
}