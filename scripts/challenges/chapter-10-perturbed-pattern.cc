/*
The Ray Tracer Challenge: Chapter 10

Render a scene using perturbed patterns.

Supply a scaling factor at the command line to increase the image dimensions.
*/

#define _USE_MATH_DEFINES // for M_PI

#include <cmath>
#include <iostream>

#include "camera.h"
#include "sphere.h"
#include "transformations.h"
#include "material.h"
#include "world.h"
#include "plane.h"
#include "pattern.h"

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
    Colour colour { 1, 0.9, 0.9 };
    material.Surface(colour);
    material.Specular(0);
    wall.SetMaterial(material);
    Matrix transform = Transformation()
        .RotateX(M_PI/2)
        .RotateY(M_PI/6)
        .Translate(0, 0, 8 * scale);
    wall.SetTransform(transform);
    return wall;
}

Sphere LargeSphere(double scale, Pattern* pattern) {
    Sphere large_sphere {};
    Matrix transform = Transformation()
        .Scale(scale, scale, scale)
        .Translate(-0.5 * scale, scale, 0.5 * scale);
    large_sphere.SetTransform(transform);
    Material material {};
    Colour colour { 1, 1, 1 };
    material.Surface(colour);
    material.Diffuse(0.7);
    material.Specular(0.3);
    material.Shininess(10);
    material.SurfacePattern(pattern);
    large_sphere.SetMaterial(material);
    return large_sphere;
}

Sphere SmallerSphere(double scale, Pattern* pattern) {
    Sphere sphere;
    double halved = scale * 0.5;
    Matrix transform = Transformation()
        .Scale(scale, scale, scale)
        .RotateY(-M_PI/4)
        .Translate(1.5 * scale, scale, -scale);
    sphere.SetTransform(transform);
    Material material {};
    Colour colour { 1, 1, 1 };
    material.Surface(colour);
    material.Diffuse(0.7);
    material.Specular(0.3);
    material.SurfacePattern(pattern);
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

StripePattern FloorPattern(double scale) {
    StripePattern pattern { Colour { 1, 1, 1 }, Colour { 1, 0.867, 0.957 } };
    pattern.SetTransform(Transformation().Translate(scale, scale, scale));
    return pattern;
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

    Plane floor = Floor(),
          wall = Wall(scale);

    RingPattern wall_ptn { Colour { 1, 0, 0 }, Colour { 0, 0, 1 } };
    wall_ptn.SetTransform(Transformation().Scale(scale, scale, scale));
    Material m = wall.ShapeMaterial();
    m.SurfacePattern(&wall_ptn);
    wall.SetMaterial(m);

    CheckerPattern checker_ptn { Colour { 1, 0.5, 0.2 }, Colour { 0, 1, 0 }};
    PerturbedPattern floor_pattern { &checker_ptn };
    m.SurfacePattern(&floor_pattern);
    floor.SetMaterial(m);

    world.Add(&floor);
    world.Add(&wall);

    StripePattern stripe_ptn { Colour { 1, 1, 1 }, Colour { 1, 0, 0 } };
    stripe_ptn.SetTransform(Transformation().Scale(1.0 / 3.0, 1, 1).RotateZ(M_PI / 3));
    PerturbedPattern large_sphere_ptn { &stripe_ptn };

    RadialGradientPattern radial_gradient_ptn { Colour { 1, 1, 0 }, Colour { 1, 0, 1 } };
    radial_gradient_ptn.SetTransform(Transformation().Scale(0.25, 1, 1));
    PerturbedPattern small_sphere_ptn { &radial_gradient_ptn };

    Sphere objects[] = {
        LargeSphere(scale, &large_sphere_ptn),
        SmallerSphere(scale, &small_sphere_ptn)
    };

    for (int i = 0; i < sizeof(objects) /  sizeof(Sphere); i++) {
        world.Add(&objects[i]);
    }

    Light light = WorldLight(scale);
    world.Add(&light);

    Camera camera { 100 * scale_int, 50 * scale_int, M_PI / 3 };
    camera.SetTransform(CameraTransform(scale));

    Canvas canvas = camera.Render(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;

    return 0;
}