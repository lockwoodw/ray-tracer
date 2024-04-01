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

// Only for testing: this is an expensive operation
const Intersection* IntersectionList::operator[](unsigned int index) {
    if (index >= list_.size()) {
        throw std::out_of_range("Index does not exist in list");
    }
    unsigned int n = 0;
    std::list<const Intersection*>::const_iterator it = list_.begin();
    while (n++ != index && it != list_.end()) {
        it++;
    }
    return *it;
}

void IntersectionList::Add(const Intersection* i) {
    list_.push_back(i);
    list_.sort(IntersectionComparator());
    if (i->Distance() >= 0) {
        if (!hit_ || i->Distance() < hit_->Distance()) {
            hit_ = i;
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

// void Shape::AddIntersections(IntersectionList& list, const Ray& ray) const {
//     throw std::runtime_error("Not implemented in base class");
// }