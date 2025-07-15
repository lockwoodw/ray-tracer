#ifndef RAY_TRACER_WORLD_H
#define RAY_TRACER_WORLD_H

#include <set>
#include "shape.h"
#include "ray.h"
#include "material.h"
#include "colour.h"
#include "space.h"

class World {
    std::set<const Shape*> objects_;
    std::set<const Light*> lights_;
    bool InShadow(const Point& point, const Light* light) const;

    public:
        static const int kMaxReflections;
        World(): objects_ {}, lights_ {} {}
        void Add(const Shape* object);
        void Add(const Light* light);
        std::size_t Remove(const Shape* object);
        std::size_t Remove(const Light* light);
        std::size_t ClearObjects();
        bool Contains(const Shape* object) const;
        bool Contains(const Light* light) const;
        IntersectionList Intersect(const Ray& ray) const;
        std::size_t NObjects() const { return objects_.size(); }
        std::size_t NLights() const { return lights_.size(); }
        const Colour ColourAt(const IntersectionComputation& ic, const int max_depth = World::kMaxReflections) const;
        const Colour ColourAt(const Ray& ray, const int max_depth = World::kMaxReflections) const;
        bool InShadow(const Point& point) const;
        const Colour ReflectedColour(const IntersectionComputation& ic, const int max_depth = World::kMaxReflections) const;
};

#endif