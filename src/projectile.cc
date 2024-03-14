#include "tuple.h"
#include <iostream>

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

Projectile tick(Environment e, Projectile p) {
    Point position = p.position + p.velocity;
    Vector velocity = p.velocity + e.gravity + e.wind;
    return Projectile { position, velocity };
}

int main(int argc, char**argv) {
    double scale_factor = 1.0;
    if (argc > 1) {
        scale_factor = atof(argv[1]);
    }

    Vector initial_velocity { 1, 1, 0 };
    Projectile p { Point { 0, 1, 0 }, initial_velocity.Normalize() * scale_factor };
    Environment e { Vector { 0, -0.1, 0 }, Vector { -0.01, 0, 0 } };

    int n_ticks = 0;
    while (p.position.y_ > 0.0) {
        std::cout << n_ticks++ << "," << p.position.y_ <<std::endl;
        p = tick(e, p);
    }

    return 0;
}