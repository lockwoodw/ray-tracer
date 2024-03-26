#include <iostream>
#include "projectile.h"

void tick(const Environment& e, Projectile& p) {
    p.position += p.velocity;
    p.velocity += e.gravity + e.wind;
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
    while (p.position.Y() > 0.0) {
        std::cout << n_ticks++ << "," << p.position.Y() <<std::endl;
        tick(e, p);
    }

    return 0;
}