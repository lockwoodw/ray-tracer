#include "shape.h"
#include "group.h"

const double Shape::kEpsilon = 1e-5;

Vector Shape::NormalAt(const Point &world_point) const {
    // convert world_point into point in object space
    Point object_point = ConvertWorldPointToObjectSpace(world_point);
    // get local normal
    Vector object_normal = LocalNormalAt(object_point);
    // convert local normal back to world space
    return ConvertObjectNormalToWorldSpace(object_normal);
}

Colour Shape::ApplyLightAt(const Light& light, const Point& point,
        const Vector& eye_vector, const Vector& normal_vector, bool in_shadow) const
{
    return material_.ApplyLightAt(this, light, point, eye_vector, normal_vector, in_shadow);
}

const Point Shape::ConvertWorldPointToObjectSpace(const Point& world_point) const {
    // Recursively apply transformations to world point to convert it to
    // object space; apply parent transformations first
    Point object_point = (parent_ != nullptr) ? parent_->ConvertWorldPointToObjectSpace(world_point)
        : world_point;
    return transform_.Inverse() * object_point;
}

const Vector Shape::ConvertObjectNormalToWorldSpace(const Vector& object_normal) const {
    // Recursively transpose transformations for object normal to convert it to
    // world space
    Vector world_normal = transform_.Inverse().Transpose() * object_normal;
    // hack to mitigate the effect of any translation operation on the w element
    world_normal[world_normal.kW] = 0.0;
    world_normal = world_normal.Normalize();

    if (parent_ != nullptr) {
        world_normal = parent_->ConvertObjectNormalToWorldSpace(world_normal);
    }
    return world_normal;
}

int TestShape::count_ = 0;

bool TestShape::operator==(const Shape& s) const {
    const TestShape* other = dynamic_cast<const TestShape*>(&s);
    if (other == nullptr) { // Shape is not a TestShape?
        return false;
    }
    return origin_ == other->origin_ && id_ == other->id_;
}

const Ray TestShape::TestAddIntersections(IntersectionList& list, const Ray& ray) const {
    // Same logic as Shape::AddIntersections() but returns the intersected ray
    Ray local_ray = ray.Transform(transform_.Inverse());
    Intersect(list, local_ray);
    return local_ray;
}

const BoundingBox TestShape::BoundsOf() const {
    return BoundingBox { Point { -1, -1, -1 }, Point { 1, 1, 1 } };
}

Intersection& Intersection::operator=(const Intersection& i) {
    object_ = i.object_;
    distance_ = i.distance_;
    return *this;
}

const double IntersectionComputation::kEpsilon = 1e-5;

IntersectionComputation::IntersectionComputation(const Intersection& i, const Ray& r,
    IntersectionList* xs):
        object_ { i.Object() },
        distance_ { i.Distance() },
        point_ { r.Position(i.Distance()) },
        eye_vector_ { -r.Direction() },
        normal_vector_ { i.Object()->NormalAt(point_) },
        inside_ { false } {
    if (Vector::DotProduct(normal_vector_, eye_vector_) < 0) {
        inside_ = true;
        normal_vector_ = -normal_vector_;
    }
    over_point_ = point_ + normal_vector_ * IntersectionComputation::kEpsilon;
    under_point_ = point_ - normal_vector_ * IntersectionComputation::kEpsilon;
    reflection_vector_ = Vector::Reflect(r.Direction(), normal_vector_);

    // Determine refractive indices
    IntersectionList* intersections = xs;

    auto free_fn = [xs, &intersections] () {
        if (xs == nullptr && intersections != nullptr) {
            // free allocated memory
            delete intersections;
        }
    };

    try
    {
        if (intersections == nullptr) {
            // If no intersection list was given, just add the given intersection
            intersections = new IntersectionList();
            intersections->Add(i);
        }

        std::list<const Shape *> objects {};
        for (const Intersection* to_test: *intersections) {
            if (i == to_test) {
                if (objects.size() == 0) {
                    // this is the first intersection encountered
                    n1_ = 1.0;
                }
                else {
                    n1_ = objects.back()->ShapeMaterial().RefractiveIndex();
                }
            }

            bool found { false };
            const Shape *to_test_object = to_test->Object();
            for (auto e: objects) {
                if (to_test_object == e) {
                    found = true;
                    break;
                }
            }

            if (found) {
                objects.remove(to_test_object);
            }
            else {
                objects.push_back(to_test_object);
            }

            if (i == to_test) {
                if (objects.size() == 0) {
                    // this is the last intersection encountered
                    n2_ = 1.0;
                }
                else {
                    n2_ = objects.back()->ShapeMaterial().RefractiveIndex();
                }
                break;
            }
        }

        free_fn();
    }
    catch(const std::exception& e)
    {
        free_fn();
    }
}

const double IntersectionComputation::Reflectance() const {
    // Implement Christophe Schlick's algorithm to approximate Fresnel's
    // equations…

    // Find the cosine of the angle between the eye and normal vectors
    // This is cos(Θi) from Snell's Law.
    double cos = Vector::DotProduct(eye_vector_, normal_vector_);

    // Total internal reflection occurs only if n1 > n2
    if (n1_ > n2_) {
        double n = n1_ / n2_;
        double sin_t_squared = n*n * (1.0 - cos * cos);
        if (sin_t_squared > 1.0) {
            return 1.0;
        }
        // When n1 > n2, use cos(Θt) instead of cos(Θi)
        cos = std::sqrt(1.0 - sin_t_squared);
    }

    double r0 = (n1_ - n2_) / (n1_ + n2_);
    r0 *= r0;

    return r0 + (1 - r0) * (std::pow(1 - cos, 5));
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
    auto it = list_.begin();
    while (n++ != index && it != list_.end()) {
        it++;
    }
    return *it;
}

void IntersectionList::Add(const Intersection* i) {
    list_.insert(i);
    if (i->Distance() >= 0) {
        if (!hit_ || i->Distance() < hit_->Distance()) {
            hit_ = i;
        }
        // This object will also be the hit for shadows only if it casts
        // shadows
        if ((!shadow_hit_ || i->Distance() < shadow_hit_->Distance()) &&
                i->Object()->ShapeMaterial().CastsShadow()) {
            shadow_hit_ = i;
        }
    }
}

void IntersectionList::Add(double d, const Shape* s) {
    Add(new Intersection(d, s));
}

void IntersectionList::Add(const Intersection& i) {
    Add(new Intersection(i.Distance(), i.Object()));
}

IntersectionList& IntersectionList::operator<<(const Intersection* i) {
    Add(i);
    return *this;
}

IntersectionList& IntersectionList::operator<<(const Intersection& i) {
    Add(i);
    return *this;
}

const Intersection* IntersectionList::Hit() const {
    return hit_;
}

const Intersection* IntersectionList::ShadowHit() const {
    return shadow_hit_;
}