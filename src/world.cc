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
    Colour refracted = RefractedColour(ic, max_depth);

    // Apply Schlick approximation if the material is both transparent and
    // reflective
    const Material m = ic.Object()->ShapeMaterial();
    if (m.Reflectivity() > 0 && m.Transparency() > 0) {
        double reflectance = ic.Reflectance();
        return colour + reflected * reflectance + refracted * (1 - reflectance);
    }
    return colour + reflected + refracted;
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
    if (simple_floating_point_compare(reflectivity, 0)) {
        return Colour::kBlack;
    }

    Ray reflected_ray { ic.OverPoint(), ic.ReflectionVector() };
    // Reduce max_depth to prevent endless recursion of reflected rays
    const Colour colour = ColourAt(reflected_ray, max_depth - 1);
    return colour * reflectivity;
}

const Colour World::RefractedColour(const IntersectionComputation& ic, const int max_depth) const {
    // Snell's Law: given two angles, the angle of the incoming ray (Θi) and
    // the angle of the refracted ray (Θt), then sin(Θi)/sin(Θt) = n2/n1, where
    // n1,n2 are the refractive indices of the materials on either side of the
    // ray-object intersection.

    // The ratio of n1/n2
    double n_ratio = ic.N1() / ic.N2();

    // cos(Θi) is the same as the dot product of these two vectors:
    double cos_i = Vector::DotProduct(ic.EyeVector(), ic.NormalVector());

    // Solve for sin(Θt) using Snell's Law and the Pythagorean identity,
    // sin(Θ)^2 + cos(Θ)^2 = 1
    double sin_t_squared = n_ratio*n_ratio * (1 - cos_i*cos_i);

    double transparency = ic.Object()->ShapeMaterial().Transparency();

    // Return black for these cases:
    if (sin_t_squared > 1 || max_depth == 0 ||
            simple_floating_point_compare(transparency, 0)) {
        return Colour::kBlack;
    }

    // All other cases: spawn a second ray in the refracted direction and return
    // its colour.

    // Compute direction of the refracted ray
    double cos_t = std::sqrt(1.0 - sin_t_squared);
    Vector direction = ic.NormalVector() * (n_ratio * cos_i - cos_t) - ic.EyeVector() * n_ratio;

    // Create the refracted ray
    Ray refracted_ray { ic.UnderPoint(), direction };

    // Find the colour of the refracted ray and multiply by the transparency
    // of the material to account for its opacity
    return ColourAt(refracted_ray, max_depth - 1) * transparency;
}