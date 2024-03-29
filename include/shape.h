#ifndef RAY_TRACER_SHAPE_H
#define RAY_TRACER_SHAPE_H

#include <vector>
#include <stdexcept>
#include "space.h"
#include "ray.h"

class Shape {
    protected:
        Point origin;

    public:
        Shape(const Point& p): origin { p } {}
        ~Shape() {}
        virtual std::vector<double> Intersections(const Ray& r) {
            throw std::runtime_error("Not implemented in base class");
        }
        const Point Origin() const { return origin; }
        virtual bool operator==(const Shape&) const {
            throw std::runtime_error("Not implemented in base class");
        }
        bool operator!=(const Shape& s) const {
            return !operator==(s);
        }
};

class Intersection {
    const Shape& object;
    double distance;

    public:
        Intersection(double d, const Shape& s): distance { d }, object { s } {}
        const Shape& Object() const { return object; }
        double Distance() const { return distance; }
};

class IntersectionList {
    std::vector<Intersection> list;

    public:
        IntersectionList() {}
        Intersection& operator[](int index) {
            if (index < 0 || index >= list.size()) {
                throw std::out_of_range("Index does not exist in list");
            }
            return list[index];
        }
        int Add(const Intersection& i) {
            list.push_back(i);
            return list.size();
        }
        int Size() { return list.size(); }
};

#endif