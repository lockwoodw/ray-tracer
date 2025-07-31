/*
g++ -I include/ -o build/pond-ppm src/utils.cc src/tuple.cc src/colour.cc \
src/space.cc src/matrix.cc src/transformations.cc src/shape.cc src/pattern.cc \
src/material.cc src/world.cc src/camera.cc src/canvas.cc src/plane.cc \
src/sphere.cc scripts/pond-ppm.cc
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
    Point origin { -2.5*scaled, 3 * scaled, -2*scaled };
    Colour colour { 1, 1, 1 };
    return Light { origin, colour };
}

Matrix CameraTransform(double scale) {
    Point from { 0.25*scale, 2.5*scale, -5 * scale }, to { 0, 0, 0 };
    Vector up { 0, 1, 0 };
    return ViewTransform { from, to, up };
}

Material FloorMaterial() {
    Material material {};
    material.Diffuse(1.0);
    material.Specular(0);
    material.Shininess(10);
    material.Reflectivity(0);
    material.Surface(Colour { 75.0/255, 54.0/255, 33.0/255 });
    return material;
}

Sphere LargePebble(double scale) {
    Sphere large_sphere {};
    Matrix transform = Transformation().Scale(scale, scale, scale).Translate(0, 0, 0.75*scale);
    large_sphere.SetTransform(transform);
    Material material {};
    Colour colour { 159.0/255, 129.0/255, 112.0/255 };
    material.Surface(colour);
    material.Diffuse(1.0);
    material.Specular(0.1);
    material.Shininess(10);
    large_sphere.SetMaterial(material);
    return large_sphere;
}

Sphere MediumPebble(double scale, Pattern* pattern) {
    double scaled = 0.75 * scale;
    Sphere medium_sphere {};
    Matrix transform = Transformation().Scale(scaled, scaled, scaled).Translate(-scale, -(scale - scaled), -scale);
    medium_sphere.SetTransform(transform);
    Material material {};
    material.Diffuse(1.0);
    material.Specular(0.1);
    material.Shininess(10);
    material.SurfacePattern(pattern);
    medium_sphere.SetMaterial(material);
    return medium_sphere;
}

Sphere SmallPebble(double scale) {
    double scaled = 0.5 * scale;
    Sphere small_sphere {};
    Matrix transform = Transformation().Scale(scaled, scaled, scaled).Translate(0.25*scale, -(scale - scaled), -scale);
    small_sphere.SetTransform(transform);
    Material material {};
    Colour colour { 40.0/255, 34.0/255, 14.0/255 };
    material.Surface(colour);
    material.Diffuse(1.0);
    material.Specular(0.1);
    material.Shininess(10);
    small_sphere.SetMaterial(material);
    return small_sphere;
}

Plane Horizon(double scale) {
    Plane wall {};
    Material material {};
    material.Surface(Colour(0.2, 0.6, 0.8));
    material.Specular(0);
    wall.SetMaterial(material);
    wall.SetTransform(Transformation().RotateX(M_PI/2).Translate(0, 0, scale * 70));
    return wall;
}

Sphere GlassMarble(double scale) {
    double scaled = 1 * scale;
    Sphere s {};
    Material m {};
    m.Transparency(1.0);
    m.RefractiveIndex(1.52);
    m.Diffuse(0.1);
    m.Ambient(0.1);
    m.Reflectivity(1.0);
    m.Shininess(300);
    m.Specular(1);
    m.Surface(Colour { 0.1, 0.1, 0.1 });
    s.SetMaterial(m);
    s.SetTransform(Transformation().Scale(scaled, scaled, scaled).Translate(1.5*scale, -(scale - scaled), 2*scale));
    return s;
}

Plane Water(double scale) {
    Plane water {};
    Material m {};
    m.Transparency(1.0);
    m.RefractiveIndex(1.0 + 1.0/3);
    m.Diffuse(0.1);
    m.Ambient(0.1);
    m.Reflectivity(0.9);
    m.Specular(1);
    m.Surface(Colour { 0.0, 0.5, 1 });
    m.CastsShadow(false);
    water.SetMaterial(m);
    water.SetTransform(Transformation().Translate(0, 0.15 * scale, 0));
    return water;
}

int main(int argc, char** argv) {
    double scale = GetScale(argc, argv);
    int scale_int = static_cast<int>(scale);

    World world {};

    Light light = WorldLight(scale);
    world.Add(&light);

    Plane floor {};
    floor.SetTransform(Transformation().Translate(0, -scale, 0));
    Material floor_material = FloorMaterial();
    floor.SetMaterial(floor_material);
    world.Add(&floor);

    Plane water = Water(scale);
    world.Add(&water);

    Sphere large_pebble = LargePebble(scale);
    world.Add(&large_pebble);

    Colour raw_umber { 130.0/255, 102.0/255, 68.0/255 },
           bistre { 128.0/255, 117.0/255, 90.0/255 };
    StripePattern stripe_pattern { raw_umber, bistre };
    stripe_pattern.SetTransform(Transformation().Scale(0.001, 1, 1));
    PerturbedPattern pptn = PerturbedPattern { &stripe_pattern };
    Sphere medium_pebble = MediumPebble(scale, &pptn);
    world.Add(&medium_pebble);

    Sphere small_pebble = SmallPebble(scale);
    world.Add(&small_pebble);

    Plane wall = Horizon(scale);
    world.Add(&wall);

    Sphere marble = GlassMarble(scale);
    world.Add(&marble);

    Camera camera { 108 * scale_int, 135 * scale_int, M_PI / 3 };
    camera.SetTransform(CameraTransform(scale));

    Canvas canvas = camera.Render(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;
    return 0;
}
