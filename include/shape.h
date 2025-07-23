#ifndef RAY_TRACER_SHAPE_H
#define RAY_TRACER_SHAPE_H

#include <list>
#include <stdexcept>
#include <iterator>
#include <cmath> // for sqrt
#include "space.h"
#include "ray.h"
#include "matrix.h"
#include "colour.h"
#include "material.h"

class IntersectionList;

class Shape {
    protected:
        Point origin_;
        Matrix transform_;
        Material material_;

    public:
        Shape(const Point& p):
            origin_ { p },
            transform_ { Matrix::Identity(4) },
            material_ { Material() } {}

        Shape(const Shape& s):
            origin_ { s.origin_ },
            transform_ { s.transform_ },
            material_ { s.material_ } {}

        virtual ~Shape() {} // required for abstract base class

        virtual void Intersect(IntersectionList& list, const Ray& ray) const = 0;
        const Ray AddIntersections(IntersectionList& list, const Ray& ray) const;

        virtual Vector LocalNormalAt(const Point &object_point) const = 0;
        Vector NormalAt(const Point &world_point) const;

        virtual bool operator==(const Shape&) const  = 0;

        const Point Origin() const { return origin_; }

        bool operator!=(const Shape& s) const {
            return !operator==(s);
        }

        void SetTransform(const Matrix& m) {
            transform_ *= m;
        }

        const Matrix& Transform() const {
            return transform_;
        }

        void SetMaterial(const Material& m) {
            material_ = m;
        }

        const Material& ShapeMaterial() const {
            return material_;
        }

        Colour ApplyLightAt(const Light& light, const Point& point,
            const Vector& eye_vector, const Vector& normal_vector, bool in_shadow = false) const;
};

class Intersection {
    const Shape* object_;
    double distance_;

    public:
        Intersection(double d, const Shape* s): distance_ { d }, object_ { s } {}
        Intersection(const Intersection& i): distance_ { i.distance_ }, object_ { i.object_ } {}
        const Shape* Object() const { return object_; }
        double Distance() const { return distance_; }
        Intersection& operator=(const Intersection& i);
        bool operator==(const Intersection& i) const {
            return object_ == i.object_ && distance_ == i.distance_;
        }
        bool operator==(const Intersection* i) const {
            return object_ == i->object_ && distance_ == i->distance_;
        }
};

class IntersectionComputation {
    const Shape* object_;
    double distance_;
    Point point_;
    Vector eye_vector_;
    Vector normal_vector_;
    bool inside_;
    Point over_point_;
    Vector reflection_vector_;
    // refractive indices
    double n1_;
    double n2_;
    Point under_point_;

    public:
        static const double kEpsilon;
        IntersectionComputation(const Intersection& i, const Ray& r,
            IntersectionList* xs = nullptr);
        const Shape* Object() const { return object_; }
        double Distance() const { return distance_; }
        const Point WorldPoint() const { return point_; }
        const Vector EyeVector() const { return eye_vector_; }
        const Vector NormalVector() const { return normal_vector_; }
        bool Inside() const { return inside_; }
        const Point OverPoint() const { return over_point_; }
        const Vector ReflectionVector() const { return reflection_vector_; }
        const double N1() const { return n1_; }
        const double N2() const { return n2_; }
        const Point UnderPoint() const { return under_point_; }
        const double Reflectance() const;
};

class IntersectionComparator {
    public:
        bool operator()(const Intersection* a, const Intersection* b) {
            return a->Distance() < b->Distance();
        }
};

class IntersectionList {
    std::list<const Intersection*> list_;
    const Intersection* hit_;
    const Intersection* shadow_hit_;

    public:
        IntersectionList(): hit_ { nullptr }, shadow_hit_ { nullptr } {}
        ~IntersectionList();
        const Intersection* operator[](unsigned int index);
        void Add(const Intersection* i);
        void Add(double d, const Shape* s);
        void Add(const Intersection& i);
        int Size() const { return list_.size(); }
        const Intersection* Hit() const;
        const Intersection* ShadowHit() const;
        IntersectionList& operator<<(const Intersection* i);
        IntersectionList& operator<<(const Intersection& i);
        std::list<const Intersection*>::iterator begin() {
            return list_.begin();
        }
        std::list<const Intersection*>::iterator end() {
            return list_.end();
        }
};

#endif