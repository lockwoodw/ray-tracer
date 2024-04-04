#ifndef RAY_TRACER_SHAPE_H
#define RAY_TRACER_SHAPE_H

#include <list>
#include <stdexcept>
#include "space.h"
#include "ray.h"
#include "matrix.h"
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

        ~Shape() {} // required for abstract base class

        virtual void AddIntersections(IntersectionList& list, const Ray& ray) const = 0;
        virtual bool operator==(const Shape&) const  = 0;
        virtual Vector NormalAt(const Point &world_point) const = 0;

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

using IntersectionPtr = Intersection*;

class IntersectionComparator {
    public:
        bool operator()(const Intersection* a, const Intersection* b) {
            return a->Distance() < b->Distance();
        }
};

class IntersectionList {
    std::list<const Intersection*> list_;
    const Intersection* hit_;

    public:
        IntersectionList(): hit_ { nullptr } {}
        ~IntersectionList();
        const Intersection* operator[](unsigned int index);
        void Add(const Intersection* i);
        void Add(double d, const Shape* s);
        void Add(const Intersection& i);
        int Size() const { return list_.size(); }
        const Intersection* Hit() const;
        IntersectionList& operator<<(const Intersection* i);
        IntersectionList& operator<<(const Intersection& i);
};

#endif