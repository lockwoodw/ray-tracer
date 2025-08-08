/*
g++ -I include/ -o build/glass-marble-ppm src/utils.cc src/tuple.cc src/colour.cc \
src/space.cc src/matrix.cc src/transformations.cc src/shape.cc src/pattern.cc \
src/material.cc src/world.cc src/camera.cc src/canvas.cc src/plane.cc \
src/sphere.cc scripts/glass-marble-ppm.cc
*/

#define _USE_MATH_DEFINES // for M_PI

#include <cmath>
#include <iostream>

#include "space.h"
#include "colour.h"
#include "material.h"
#include "world.h"
#include "plane.h"
#include "camera.h"
#include "canvas.h"
#include "sphere.h"
#include "pattern.h"

static double kMaxScale { 20.0 };

double GetScale(int argc, char** argv) {
    double scale { 1.0 };

    if (argc > 1) {
        scale = atof(argv[1]);
    }

    if (scale <= 0 || scale > kMaxScale) {
        std::cerr << "Given scale invalid (1-" << kMaxScale << ")" << std::endl;
        exit(-1);
    }
    return scale;
}

Light WorldLight(double scale) {
    double scaled = 10 * scale;
    Point origin { -scaled, 2 * scaled, -scaled };
    Colour colour { 1, 1, 1 };
    return Light { origin, colour };
}

Matrix CameraTransform(double scale) {
    Point from { 0, 2 * scale, -3.5 * scale }, to { 0, 0, 0 };
    Vector up { 0, 1, 0 };
    return ViewTransform { from, to, up };
}

CheckerPattern FloorPattern(double scale) {
    double scaled = 2 * scale;
    CheckerPattern pattern { Colour { 0.5, 0, 1 }, Colour::kWhite };
    pattern.SetTransform(Transformation()
        .Scale(scaled, scaled, scaled)
        .RotateY(M_PI / 3)
        // .Translate(scale, scale, 0)
    );
    return pattern;
}

Material FloorMaterial(Pattern* pattern) {
    Material material {};
    material.Specular(0);
    material.Shininess(10);
    material.Reflectivity(0.2);
    material.SurfacePattern(pattern);
    return material;
}

Plane Horizon(double scale) {
    Plane wall {};
    Material material {};
    material.Surface(Colour(0.2, 0.1, 0.8));
    material.Specular(0);
    // material.Diffuse(0);
    wall.SetMaterial(material);
    wall.SetTransform(Transformation().RotateX(M_PI/2).Translate(0, 0, scale * 50));
    return wall;
}

Sphere GlassMarble(double scale) {
    Sphere s {};
    Material m {};
    m.Transparency(1.0);
    m.RefractiveIndex(1.52);
    m.Diffuse(0.1);
    m.Ambient(0.1);
    m.Reflectivity(1.0);
    m.Shininess(300);
    m.Specular(1);
    m.Surface(Colour { 0.5, 0.05, 0.05 });
    // m.CastsShadow(false);
    s.SetMaterial(m);
    s.SetTransform(Transformation().Scale(scale, scale, scale).Translate(0, scale, 0));
    return s;
}

Sphere AirBubble(double scale) {
    double scaled = scale * 0.98;
    Sphere s {};
    Material m {};
    m.Transparency(1.0);
    m.RefractiveIndex(1.00029);
    m.Diffuse(0);
    m.Ambient(0);
    m.Specular(0);
    m.Reflectivity(0);
    m.Shininess(10);
    // m.Surface(Colour { 0.1, 0.1, 0.1 });
    m.CastsShadow(false);
    s.SetMaterial(m);
    s.SetTransform(Transformation().Scale(scaled, scaled, scaled).Translate(0, scale, 0));
    return s;
}

int main(int argc, char** argv) {
    double scale = GetScale(argc, argv);
    int scale_int = static_cast<int>(scale);

    World world {};

    Light light = WorldLight(scale);
    world.Add(&light);

    Plane floor {};
    CheckerPattern floor_pattern = FloorPattern(scale);
    Material floor_material = FloorMaterial(&floor_pattern);
    floor.SetMaterial(floor_material);
    world.Add(&floor);

    Plane wall = Horizon(scale);
    world.Add(&wall);

    Sphere marble = GlassMarble(scale);
    world.Add(&marble);

    Sphere bubble = AirBubble(scale);
    world.Add(&bubble);

    Camera camera { 108 * scale_int, 135 * scale_int, M_PI / 3 };
    camera.SetTransform(CameraTransform(scale));

    Canvas canvas = camera.Render(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;
    return 0;
}
