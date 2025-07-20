#include "world.h"

const int World::kMaxReflections = 5;

void World::Add(const Shape* object) {
    objects_.insert(object);
}

void World::Add(const Light* light) {
    lights_.insert(light);
}

std::size_t World::Remove(const Shape* object) {
    return objects_.erase(object);
}

std::size_t World::ClearObjects() {
    std::size_t n = 0;
    std::set<const Shape *>::iterator it = objects_.begin(), end = objects_.end();
    while (it != end) {
        delete *it;
        n++;
        it++;
    }
    objects_.clear();
    return n;
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

const Colour World::ColourAt(const IntersectionComputation& ic, const int max_depth) const {
    Colour colour {};
    for (const Light* light: lights_) {
        Point point = ic.OverPoint();
        bool in_shadow = InShadow(point, light);
        colour += ic.Object()->ApplyLightAt(*light, point, ic.EyeVector(), ic.NormalVector(), in_shadow);
    }
    Colour reflected = ReflectedColour(ic, max_depth);
    return colour + reflected;
}

const Colour World::ColourAt(const Ray& ray, const int max_depth) const {
    Colour colour = Colour::kBlack; // black: default if there is no hit
    IntersectionList xs = Intersect(ray);
    const Intersection* hit = xs.Hit();
    if (hit) {
        const IntersectionComputation ic { *hit, ray, &xs };
        colour += ColourAt(ic, max_depth);
    }
    return colour;
}

bool World::InShadow(const Point& point, const Light* light) const {
    Vector v = light->Position() - point;
    double distance = v.Magnitude();
    Vector direction = v.Normalize();

    Ray ray { point, direction };
    IntersectionList xs = Intersect(ray);
    const Intersection* hit = xs.Hit();
    return hit && hit->Distance() < distance;
}

bool World::InShadow(const Point& point) const {
    // The point is shaded only if it is in the shadow for all lights
    bool result { true };
    for (const Light* light: lights_) {
        result = result && InShadow(point, light);
        if (!result) {
            // Short circuit if the point is not in the shadow of any light
            break;
        }
    }
    return result;
}

const Colour World::ReflectedColour(const IntersectionComputation& ic, const int max_depth) const {
    if (max_depth <= 0) {
        // Go no further processing reflections
        return Colour::kBlack;
    }

    double reflectivity = ic.Object()->ShapeMaterial().Reflectivity();
    if (reflectivity == 0) {
        return Colour::kBlack;
    }

    Ray reflected_ray { ic.OverPoint(), ic.ReflectionVector() };
    // Reduce max_depth to prevent endless recursion of reflected rays
    const Colour colour = ColourAt(reflected_ray, max_depth - 1);
    return colour * reflectivity;
}