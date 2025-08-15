/*
The Ray Tracer Challenge: Chapter 9

Render a hexagonal-shaped room in PPM format.

Supply a scaling factor at the command line to increase the image dimensions.
*/

#define _USE_MATH_DEFINES // for M_PI

#include <cmath>
#include <iostream>

#include "camera.h"
#include "matrix.h"
#include "transformations.h"
#include "material.h"
#include "world.h"
#include "space.h"
#include "plane.h"

Plane* Floor() {
    Plane* floor = new Plane();
    Material material {};
    Colour colour { 1, 0.9, 0.9 };
    material.Surface(colour);
    material.Specular(0);
    floor->SetMaterial(material);
    return floor;
}

Light WorldLight(double scale) {
    double scaled = 5 * scale;
    Point origin { -scaled, scaled, -scaled };
    Colour colour { 1, 1, 1 };
    return Light { origin, colour };
}

Matrix CameraTransform(double scale) {
    // Camera points down almost directly towards origin
    Point from { 0, 6 * scale, -0.5 }, to { 0, 0, 0 };
    Vector up { 0, 1, 0 };
    return ViewTransform { from, to, up };
}

int main(int argc, char** argv) {
    double scale { 1 }, max_scale { 20 };

    if (argc > 1) {
        scale = atof(argv[1]);
    }

    if (scale <= 0 || scale > max_scale) {
        std::cerr << "Given scale invalid (1-" << max_scale << ")" << std::endl;
        return 1;
    }

    int scale_int = static_cast<int>(scale);

    World world {};
    world.Add(Floor());

    Material wall_material {};
    wall_material.Surface(Colour { 0.5, 0.9, 0.9 });
    wall_material.Specular(0);

    // Properties for walls:
    int n_sides { 6 };
    double base_y_radians = M_PI / 3, // 60 degrees
           x_radians = M_PI / 2,      // 90 degrees
           circumradius = 20,         // radius of circle that runs through each vertex
           // calculate apothem (aka, inradius) using Pythagorean theorem
           apothem = std::sqrt(std::pow(circumradius, 2) - std::pow(circumradius / 2, 2));

    // x,z offsets per wall--e.g., the first wall is moved apothem units into +z;
    // the second wall is moved circumradius units into +x; etc.
    double x_offsets[n_sides] { 0, circumradius, circumradius, 0, -circumradius, -circumradius },
           z_offsets[n_sides] { apothem, 0, 0, -apothem, 0, 0 };

    // Rotate xz plane in x to create wall
    Transformation base_transform = Transformation().RotateX(x_radians);

    // Create a wall, rotate it in y in increasing increments of 60 degrees,
    // and shift it in either x or z
    for (int i = 0; i < n_sides; i++) {
        Plane* wall = new Plane();
        wall->SetMaterial(wall_material);
        Transformation transform = base_transform;
        transform.RotateY(i * base_y_radians).Translate(x_offsets[i] * scale, 0, z_offsets[i] * scale);
        wall->SetTransform(transform);
        world.Add(wall);
    }

    Light light = WorldLight(scale);
    world.Add(&light);

    Camera camera { 100 * scale_int, 100 * scale_int, M_PI / (1.5 * scale) };
    camera.SetTransform(CameraTransform(scale));

    Canvas canvas = camera.Render(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;

    // clean-up heap
    world.ClearObjects();

    return 0;
}