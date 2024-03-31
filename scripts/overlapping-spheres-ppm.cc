/*
Render overlapping spheres in PPM format.
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
    // Set up some spheres of various sizes and colours
    Point sphere_origin { 0, 0, 0 };
    double radius { 100 };
    Sphere sphere1 { sphere_origin, radius },
           sphere2 { sphere_origin, radius / 4 },
           sphere3 { sphere_origin, radius / 6 };
    Point ray_origin { 0, 0, -radius * 2 };

    // Configure canvas large enough to hold spheres with padding around them
    Colour background { 1, 1, 1 },
           sphere1_colour { 1, 0, 0 },
           sphere2_colour { 0, 0, 1 },
           sphere3_colour { 1, 1, 0 },
           default_colour { 0, 0, 0 };
    double quad_dimension { radius * 1.5 };
    int canvas_dimension { static_cast<int>(2 * quad_dimension) };
    Canvas canvas { canvas_dimension, canvas_dimension, default_colour };

    // Transform spheres so they overlap: bring spheres 2 and 3 closer to the
    // ray origin; move sphere 1 further away. Make sphere2 an ellipsoid.
    Matrix transform1 = Transformation().Translate(-25, 0, 20),
           transform2 = Transformation().Scale(2, 0.5, 0.5).Translate(50, 0, -50).RotateY(DegreesToRadians(35)),
           transform3 = Transformation().Translate(-50, 40, -60);
    sphere1.SetTransform(transform1);
    sphere2.SetTransform(transform2);
    sphere3.SetTransform(transform3);

    // From the ray origin, create a ray directed to each pixel in the canvas
    // and then confirm if it hits the spheres
    for (int row = 0; row < canvas_dimension; row++) {
        for (int column = 0; column < canvas_dimension; column++) {
            // Convert canvas coordinates into world coordinates
            double x { column - quad_dimension },
                   y { quad_dimension - row };
            Point p { x, y, 0};
            Vector v { p - ray_origin };
            // Create ray pointing from ray_origin to point (x, y)
            Ray ray { ray_origin, v };
            // Confirm which sphere the ray hits (if any)
            IntersectionList xs {};
            sphere1.AddIntersections(xs, ray);
            sphere2.AddIntersections(xs, ray);
            sphere3.AddIntersections(xs, ray);
            const Intersection* i = xs.Hit();
            Colour pixel = background;
            if (i) {
                const Shape* match = i->Object();
                if (match == &sphere1) {
                    pixel = sphere1_colour;
                }
                else if (match == &sphere2) {
                    pixel = sphere2_colour;
                }
                else if (match == &sphere3) {
                    pixel = sphere3_colour;
                }
            }
            canvas[row][column] = pixel;
        }
    }

    PPMv3 ppm { canvas };
    std::cout << ppm;

    return 0;
}