/*

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

Plane Horizon(double scale) {
    Plane wall {};
    Material material {};
    Colour colour { 1, 0.9, 0.9 };
    material.Surface(colour);
    material.Specular(0);
    wall.SetMaterial(material);
    Matrix transform = Transformation().RotateX(M_PI/2).RotateY(M_PI/6).Translate(0, 0, 8 * scale);
    wall.SetTransform(transform);
    return wall;
}

Sphere LargeSphere(double scale, Pattern* pattern) {
    Sphere large_sphere {};
    Matrix transform = Transformation().Scale(scale, scale, scale).Translate(-0.5 * scale, scale, 0.5 * scale);
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
    Matrix transform = Transformation().Scale(scale, scale, scale).RotateY(-M_PI/4).Translate(1.5 * scale, scale, -scale);
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

Sphere SmallestSphere(double scale, Pattern* pattern) {
    Sphere sphere;
    double third = scale / 3;
    Matrix transform = Transformation().Scale(third, third, third).Translate(-1.5 * scale, third, -0.75 * scale);
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

    World world {};

    Plane floor = Floor(), wall = Horizon(scale);
    RingPattern pattern2 { Colour { 1, 0, 0 }, Colour { 0, 0, 1 } };
    pattern2.SetTransform(Transformation().Scale(scale, scale, scale));

    Material m = wall.ShapeMaterial();
    m.SurfacePattern(&pattern2);
    wall.SetMaterial(m);

    CheckerPattern pattern3 { Colour { 1, 0.5, 0.2 }, Colour { 0, 1, 0 }};
    // pattern3.SetTransform(pattern2.Transform());
    PerturbedPattern floor_pattern { &pattern3 };
    m.SurfacePattern(&floor_pattern);
    floor.SetMaterial(m);

    world.Add(&floor);
    world.Add(&wall);

    StripePattern pattern1 { Colour { 1, 1, 1 }, Colour { 1, 0, 0 } };
    pattern1.SetTransform(Transformation().Scale(1.0 / 3.0, 1, 1).RotateZ(M_PI / 3));
    PerturbedPattern perturbed_pattern { &pattern1 };

    RadialGradientPattern pattern4 { Colour { 1, 1, 0 }, Colour { 1, 0, 1 } };
    pattern4.SetTransform(Transformation().Scale(0.25, 1, 1));
    PerturbedPattern p4 { &pattern4 };

    Sphere objects[] = {
        LargeSphere(scale, &perturbed_pattern),
        SmallerSphere(scale, &p4),
        // SmallestSphere(scale, &pattern)
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