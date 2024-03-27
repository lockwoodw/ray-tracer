/*
Generate a 12-point clockface in PPM format with a given radius.
*/

#define _USE_MATH_DEFINES // for M_PI
#include <cmath>
#include <iostream>
#include <array>
#include "transformations.h"
#include "canvas.h"
#include "space.h"
#include "utils.h"

const static double kMinRadius { 30 };

int ToInt(double x) {
    return static_cast<int>(std::nearbyint(x));
}

// Thicken point to 9-pixels.
// Note that the Y-coordinate provides the row index in the canvas.
void AddPoint(Canvas& canvas, int x, int y, Colour& point) {
    for (int row = y - 1; row <= y + 1; row++) {
        for (int column = x - 1; column <= x + 1; column++) {
            canvas[row][column] = point;
        }
    }
}

int main(int argc, char **argv) {
    // Use radius from command line, if given
    double radius = kMinRadius;

    if (argc > 1) {
        radius = atof(argv[1]);
    }

    if (radius < kMinRadius) {
        radius = kMinRadius;
    }

    // Set up canvas so it can hold clock with equal padding on all sides
    double clock_diameter = radius * 2,
           translation_offset = radius * 1.5;
    int canvas_dimension = clock_diameter + radius;
    Colour colour { 1, 0, 0 }, // red
           white { 1, 1, 1 };  // white
    Canvas canvas { canvas_dimension, canvas_dimension, white };
    Point origin { 0, radius, 0 }; // start at 12:00 position

    for (int i = 0; i < 12; i++) {
        // Apply transforms to origin to generate the 12 positions of the clock:
        // * Rotate point by pi/6 radians (2pi rad = 360 deg) each time.
        // * Shift point over and down to center it within the canvas, which
        //   has its origin in the top-left corner.
        Matrix transform = Transformation()
            .RotateZ(i * (-M_PI / 6)) // negative == clockwise
            .Translate(translation_offset, -translation_offset, 0);

        Point p = transform * origin;

        // Because the Y dimension increases downwards in the canvas, and the
        // points have been shifted into the bottom-right quadrant (all negative
        // Y coordinates), negate each Y-coordinate to get the correct value in the canvas.
        AddPoint(canvas, ToInt(p.X()), -ToInt(p.Y()), colour);
    }

    PPMv3 ppm { canvas };
    std::cout << ppm;

    return 0;
}