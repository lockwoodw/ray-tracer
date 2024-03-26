#ifndef RAY_TRACER_PROJECTILE_H
#define RAY_TRACER_PROJECTILE_H

#include "space.h"

class Projectile {
    public:
        Point position;
        Vector velocity;
        Projectile(Point position, Vector velocity) : position { position }, velocity { velocity } {}
};

class Environment {
    public:
        Vector gravity;
        Vector wind;
        Environment(Vector gravity, Vector wind) : gravity { gravity }, wind { wind } {}
};

#endif