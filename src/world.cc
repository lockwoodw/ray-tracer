#include "world.h"

void World::Add(const Shape* object) {
    objects_.insert(object);
}

void World::Add(const Light* light) {
    lights_.insert(light);
}

std::size_t World::Remove(const Shape* object) {
    return objects_.erase(object);
}

std::size_t World::Remove(const Light* light) {
    return lights_.erase(light);
}

bool World::Contains(const Shape* object) const {
    std::set<const Shape *>::iterator it = objects_.find(object);
    return it != objects_.end();
}

bool World::Contains(const Light* light) const {
    std::set<const Light *>::iterator it = lights_.find(light);
    return it != lights_.end();
}

IntersectionList World::Intersect(const Ray& ray) const {
    IntersectionList xs {};
    std::set<const Shape *>::iterator it = objects_.begin(),
                                      end = objects_.end();
    while (it != end) {
        (*it)->AddIntersections(xs, ray);
        it++;
    }
    return xs;
}

const Colour World::ColourAt(const IntersectionComputation& ic) const {
    Colour colour {};
    for (const Light* light: lights_) {
        colour += ic.Object()->ShapeMaterial().ApplyLightAt(*light, ic.WorldPoint(),
            ic.EyeVector(), ic.NormalVector());
    }
    return colour;
}

const Colour World::ColourAt(const Ray& ray) const {
    Colour colour { 0, 0, 0 }; // black: default if there is no hit
    IntersectionList xs = Intersect(ray);
    const Intersection* hit = xs.Hit();
    if (hit) {
        const IntersectionComputation ic { *hit, ray };
        colour += ColourAt(ic);
    }
    return colour;
}