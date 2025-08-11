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

const std::array<std::vector<Shape *>, 2> ShapeGroup::Partition() {
    // Returns two vectors containing the children that fit into the
    // the halves of the group's bounding box; any partioned children
    // are removed from group.
    using ShapeVector = std::vector<Shape *>;
    std::array<ShapeVector, 2> partitions { ShapeVector {}, ShapeVector {} };
    auto buckets = BoundsOf().Split();

    for (auto it = shapes_.begin(); it != shapes_.end();) {
        auto shape_box = (*it)->BoundsOfInParentSpace();
        bool partitioned { false };
        for (int index = 0; index < buckets.size(); index++) {
            if (buckets[index].Contains(shape_box)) {
                partitions[index].push_back(*it);
                partitioned = true;
                break;
            }
        }
        if (partitioned) {
            it = shapes_.erase(it);
        }
        else {
            ++it;
        }
    }
    return partitions;
}

void ShapeGroup::AddSubgroup(std::vector<Shape *> shapes) {
    // Creates a new subgroup and adds each shape to it, then adds the subgroup
    // to the group
    ShapeGroup* subgroup = new ShapeGroup();
    subgroups_.push_back(subgroup);
    for (auto s: shapes) {
        subgroup->Add(s);
    }
    Add(subgroup);
}

Shape* ShapeGroup::operator[](int i) {
    return shapes_[i];
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