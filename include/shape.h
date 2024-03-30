#ifndef RAY_TRACER_SHAPE_H
#define RAY_TRACER_SHAPE_H

#include <vector>
#include <stdexcept>
#include "space.h"
#include "ray.h"
#include "matrix.h"

class IntersectionList;

class Shape {
    protected:
        Point origin_;
        Matrix transform_;

    public:
        Shape(const Point& p): origin_ { p }, transform_ { Matrix::Identity(4) } {}
        ~Shape() {}
        virtual IntersectionList Intersections(const Ray& r);
        const Point Origin() const { return origin_; }
        virtual bool operator==(const Shape&) const {
            throw std::runtime_error("Not implemented in base class");
        }
        bool operator!=(const Shape& s) const {
            return !operator==(s);
        }
        void SetTransform(const Matrix& m) {
            transform_ *= m;
        }
        const Matrix& Transform() const {
            return transform_;
        }
};

class Intersection {
    const Shape* object_;
    double distance_;

    public:
        Intersection(double d, const Shape& s): distance_ { d }, object_ { &s } {}
        const Shape* Object() const { return object_; }
        double Distance() const { return distance_; }
        Intersection& operator=(const Intersection& i);
        bool operator==(const Intersection& i) const {
            return *object_ == *i.object_ && distance_ == i.distance_;
        }
};

class IntersectionComparator {
    public:
        bool operator()(const Intersection& a, const Intersection& b) {
            return a.Distance() < b.Distance();
        }
};

class IntersectionList {
    std::vector<Intersection> list_;
    const Intersection* hit_;

    public:
        IntersectionList(): hit_ { nullptr } {}
        Intersection& operator[](int index);
        void Add(const Intersection& i);
        int Size() const { return list_.size(); }
        const Intersection* Hit() const;
};

#endif