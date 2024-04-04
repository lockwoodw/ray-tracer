#include "world.h"

void World::Add(const Shape* object) {
    objects_.insert(object);
}

void World::Add(const Light* light) {
    lights_.insert(light);
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