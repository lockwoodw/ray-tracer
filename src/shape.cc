#include <algorithm>
#include "shape.h"

Intersection& Intersection::operator=(const Intersection& i) {
    object_ = i.object_;
    distance_ = i.distance_;
    return *this;
}

Intersection& IntersectionList::operator[](int index) {
    if (index < 0 || index >= list_.size()) {
        throw std::out_of_range("Index does not exist in list");
    }
    return list_[index];
}

void IntersectionList::Add(const Intersection& i) {
    list_.push_back(i);
    std::sort(list_.begin(), list_.end(), IntersectionComparator());
    if (i.Distance() >= 0) {
        if ((hit_ == nullptr) || (i.Distance() < hit_->Distance())) {
            hit_ = &i;
        }
    }
}

const Intersection* IntersectionList::Hit() const {
    return hit_;
}

IntersectionList Shape::Intersections(const Ray& r) {
    throw std::runtime_error("Not implemented in base class");
}