/*
Render the scene described on Chapter 7 (pp. 105–107), replacing the
sphere-based floor and wall with planes.

Supply a scaling factor at the command line to increase the resolution.
*/

#define _USE_MATH_DEFINES // for M_PI

#include <cmath>
#include <iostream>

#include "camera.h"
#include "sphere.h"
#include "transformations.h"
#include "material.h"
#include "world.h"
#include "space.h"
#include "plane.h"

Plane Floor() {
    Plane floor {};
    Material material {};
    Colour colour { 1, 0.9, 0.9 };
    material.Surface(colour);
    material.Specular(0);
    floor.SetMaterial(material);
    return floor;
}

Plane Wall(double scale) {
    Plane wall {};
    Material material {};
    Colour colour { 0.5, 0.9, 0.9 };
    material.Surface(colour);
    material.Specular(0);
    wall.SetMaterial(material);
    Matrix transform = Transformation().RotateX(M_PI/2).RotateY(M_PI/6).Translate(0, 0, 4 * scale);
    wall.SetTransform(transform);
    return wall;
}

Sphere LargeSphere(double scale) {
    Sphere large_sphere {};
    Matrix transform = Transformation().Scale(scale, scale, scale).Translate(-0.5 * scale, scale, 0.5 * scale);
    large_sphere.SetTransform(transform);
    Material material {};
    Colour colour { 0.1, 1, 0.5 };
    material.Surface(colour);
    material.Diffuse(0.7);
    material.Specular(0.3);
    material.Shininess(10);
    large_sphere.SetMaterial(material);
    return large_sphere;
}

Sphere SmallerSphere(double scale) {
    Sphere sphere;
    double halved = scale * 0.5;
    Matrix transform = Transformation().Scale(halved, halved, halved).Translate(1.5 * scale, halved, -halved);
    sphere.SetTransform(transform);
    Material material {};
    Colour colour { 0.5, 1, 0.1 };
    material.Surface(colour);
    material.Diffuse(0.7);
    material.Specular(0.3);
    sphere.SetMaterial(material);
    return sphere;
}

Sphere SmallestSphere(double scale) {
    Sphere sphere;
    double third = scale / 3;
    Matrix transform = Transformation().Scale(third, third, third).Translate(-1.5 * scale, third, -0.75 * scale);
    sphere.SetTransform(transform);
    Material material {};
    Colour colour { 1, 0.8, 0.1 };
    material.Surface(colour);
    material.Diffuse(0.7);
    material.Specular(0.3);
    sphere.SetMaterial(material);
    return sphere;
}

Light WorldLight(double scale) {
    double scaled = 10 * scale;
    Point origin { -scaled, scaled, -scaled };
    Colour colour { 1, 1, 1 };
    return Light { origin, colour };
}

Matrix CameraTransform(double scale) {
    Point from { 0, 1.5 * scale, -5 * scale }, to { 0, scale, 0 };
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

    World world {};
    Plane floor = Floor(), wall = Wall(scale);
    world.Add(&floor);
    world.Add(&wall);

    Sphere objects[] = {
        LargeSphere(scale),
        SmallerSphere(scale),
        SmallestSphere(scale)
    };

    for (int i = 0; i < sizeof(objects) /  sizeof(Sphere); i++) {
        world.Add(&objects[i]);
    }

    Light light = WorldLight(scale);
    world.Add(&light);

    Camera camera { 100 * static_cast<int>(scale), 50 * static_cast<int>(scale), M_PI / 3 };
    camera.SetTransform(CameraTransform(scale));

    Canvas canvas = camera.Render(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;

    return 0;
}