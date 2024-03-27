/*
Plot projectile in PPM format given an initial position and velocity, including
the effects of gravity and wind resistance.

To improve legibility, each point is thickened to a 9-pixel square.

To simulate the effect of different initial velocities, you can supply a scaling
factor from the command line.
*/

#include <iostream>
#include <vector>
#include <cmath>
#include "projectile.h"
#include "canvas.h"

// Recalculate the projectile position and velocity at each tick of time.
void Tick(const Environment& e, Projectile& p) {
    p.position += p.velocity;
    p.velocity += e.gravity + e.wind;
}

// Thicken point to 9-pixels
void AddPoint(Canvas& canvas, int x, int y, Colour& point) {
    for (int row = y - 3; row <= y - 1; row++) {
        for (int column = x - 1; column <= x + 1; column++) {
            canvas[row][column] = point;
        }
    }
}

int main(int argc, char**argv) {
    double velocity_scale = 1.0;

    if (argc > 1) {
        velocity_scale = atof(argv[1]);
    }

    if (velocity_scale <= 0) {
        std::cerr << "Velocity factor must be positive" << std::endl;
        exit(1);
    }

    Vector initial_velocity { 1, 1, 0 };
    Projectile p { Point { 0, 1, 0 }, initial_velocity.Normalize() * velocity_scale };
    Environment e { Vector { 0, -0.1, 0 }, Vector { -0.01, 0, 0 } };

    std::vector<int> location_by_tick;
    double y = p.position.Y(), max_height = y;

    // Center each point within an odd number of pixels (e.g., 3)
    int tick_width = 3, padding = tick_width / 2;
    double y_scale = 1.0; // increase to separate points vertically, if desired

    // Collect the points and determine max y position
    while (y > 0.0) {
        location_by_tick.push_back(static_cast<int>(std::nearbyint(y * y_scale)));
        if (y > max_height) {
            max_height = y;
        }
        Tick(e, p);
        y = p.position.Y();
    }

    // Canvas dimensions: width is number-of-ticks * tick_width, plus extra padding
    // because each point is centered within tick_width pixels.
    // Height is max_height + 3 pixels because each point is 3-pixels deep.
    int width = location_by_tick.size() * tick_width + padding,
        height = static_cast<int>(std::nearbyint(max_height * y_scale)) + 3;

    Colour point { 1, 0, 0 }, // red
           white { 1, 1, 1 }; // white
    Canvas canvas { width, height, white };

    int x = 0;
    for (int y: location_by_tick) {
        // Center each point halfway within the tick.
        // Because the canvas's Y dimension increases downwards, the highest
        // point is actually in row 0, so we need to subtract each point's y
        // coordinate from the the canvas height.
        AddPoint(canvas, x + padding, height - y, point);
        x += tick_width;
    }

    PPMv3 ppm { canvas };
    std::cout << ppm;

    return 0;
}