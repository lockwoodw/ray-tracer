/*
Render transformed sphere in PPM format.
*/

#define _USE_MATH_DEFINES // for M_PI
#include <cmath>
#include <iostream>
#include "canvas.h"
#include "colour.h"
#include "sphere.h"
#include "ray.h"
#include "transformations.h"

double DegreesToRadians(int degrees) {
    return degrees * M_PI / 180;
}

int main(int argc, char** argv) {
    // Set up a sphere with radius of 100 pixels and a ray positioned well behind
    // it in the z axis
    Point sphere_origin { 0, 0, 0 };
    double radius { 100 };
    Sphere sphere { sphere_origin, radius };
    Point ray_origin { 0, 0, -radius * 2 };

    // Configure canvas large enough to hold sphere with padding around it
    Colour background { 1, 1, 1 },
           sphere_colour { 1, 0, 0 },
           default_colour { 0, 0, 0 };
    double quad_dimension { radius * 1.5 };
    int canvas_dimension { static_cast<int>(2 * quad_dimension) };
    Canvas canvas { canvas_dimension, canvas_dimension, default_colour };

    // Transform sphere
    Matrix transform = Transformation()
        .Scale(1.25, 0.5, 0.5)
        .RotateZ(-DegreesToRadians(45))
        .Translate(quad_dimension / 4, 0, 0);
    sphere.SetTransform(transform);

    // From the ray origin, create a ray directed to each pixel in the canvas
    // and then confirm if it hits the sphere
    for (int row = 0; row < canvas_dimension; row++) {
        for (int column = 0; column < canvas_dimension; column++) {
            // Convert canvas coordinates into world coordinates
            double x { column - quad_dimension },
                   y { quad_dimension - row };
            Point p { x, y, 0};
            Vector v { p - ray_origin };
            // Create ray pointing from ray_origin to point (x, y)
            Ray ray { ray_origin, v };
            // Confirm hit
            IntersectionList xs {};
            sphere.AddIntersections(xs, ray);
            const Intersection* i = xs.Hit();
            canvas[row][column] = (i != nullptr) ? sphere_colour : background;
        }
    }

    PPMv3 ppm { canvas };
    std::cout << ppm;

    return 0;
}