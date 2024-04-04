#ifndef RAY_TRACER_WORLD_H
#define RAY_TRACER_WORLD_H

#include <set>
#include "shape.h"
#include "ray.h"
#include "material.h"

class World {
    std::set<const Shape*> objects_;
    std::set<const Light*> lights_;

    public:
        World(): objects_ {}, lights_ {} {}
        void Add(const Shape* object);
        void Add(const Light* light);
        bool Contains(const Shape* object) const;
        bool Contains(const Light* light) const;
        IntersectionList Intersect(const Ray& ray) const;
        std::size_t NObjects() { return objects_.size(); }
        std::size_t NLights() { return lights_.size(); }
};

#endif