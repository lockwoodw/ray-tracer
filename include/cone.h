#ifndef RAY_TRACER_CONE_H
#define RAY_TRACER_CONE_H

#include "cylinder.h"

class Cone: public Cylinder {
    bool RayIntersectsEnd(const Ray&r, double t, double y) const;
    bool AddEndIntersects(IntersectionList& list, const Ray& ray) const;

    public:
        Cone(): Cylinder {} {}
        Cone(const Cone& c): Cylinder { c } {}
        Cone(double min, double max, bool closed): Cylinder { min, max, closed } {}
        Cone(Point origin, double min, double max, bool closed):
            Cylinder { origin, min, max, closed } {}

        bool operator==(const Shape& s) const;
        bool Intersect(IntersectionList& list, const Ray& ray) const override;
        Vector LocalNormalAt(const Point &object_point) const override;
        const BoundingBox BoundsOf() const override;
};

#endif