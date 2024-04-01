#include <algorithm>
#include "shape.h"

Intersection& Intersection::operator=(const Intersection& i) {
    object_ = i.object_;
    distance_ = i.distance_;
    return *this;
}

IntersectionList::~IntersectionList() {
    for(auto i: list_) {
        delete i;
    }
}

const Intersection* IntersectionList::operator[](unsigned int index) {
    if (index >= list_.size()) {
        throw std::out_of_range("Index does not exist in list");
    }
    return list_[index];
}

void IntersectionList::Add(const Intersection* i) {
    list_.push_back(i);
    std::sort(list_.begin(), list_.end(), IntersectionComparator());
    if (i->Distance() >= 0) {
        if (!hit_) {
            hit_ = i;
            // hit_ = new Intersection(i);
        }
        else if (i->Distance() < hit_->Distance()) {
            hit_ = i;
            // Intersection* tmp = hit_;
            // hit_ = new Intersection(i);
            // delete tmp;
        }
    }
}

IntersectionList& IntersectionList::operator<<(const Intersection* i) {
    Add(i);
    return *this;
}

const Intersection* IntersectionList::Hit() const {
    return hit_;
}

void Shape::AddIntersections(IntersectionList& list, const Ray& ray) const {
    throw std::runtime_error("Not implemented in base class");
}