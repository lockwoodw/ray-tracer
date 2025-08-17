/*
The Ray Tracer Challenge: Chapter 11

Render a scene that observes the surface of a pond from above, looking down
at the rocks beneath (p. 165).

Supply a scaling factor at the command line to increase the image dimensions.
*/

#define _USE_MATH_DEFINES // for M_PI

#include <cmath>

#include "challenges.h"
#include "world.h"
#include "plane.h"
#include "camera.h"
#include "canvas.h"
#include "sphere.h"
#include "pattern.h"

Light WorldLight(double scale) {
    double scaled = 10 * scale;
    Point origin { -2.5 * scaled, 3 * scaled, -2 * scaled };
    Colour colour { 1, 1, 1 };
    return Light { origin, colour };
}

Matrix CameraTransform(double scale) {
    Point from { 0.25 * scale, 2.5 * scale, -5 * scale }, to { 0, 0, 0 };
    Vector up { 0, 1, 0 };
    return ViewTransform { from, to, up };
}

Material FloorMaterial() {
    Material material {};
    material.Diffuse(1.0);
    material.Specular(0);
    material.Shininess(10);
    material.Reflectivity(0);
    return material;
}

Sphere LargePebble(double scale) {
    Sphere large_sphere {};
    Matrix transform = Transformation()
        .Scale(scale)
        .Translate(0, 0, 0.75 * scale);
    large_sphere.SetTransform(transform);
    Material material {};
    Colour colour { 159.0 / 255, 129.0 / 255, 112.0 / 255 };
    material.Surface(colour);
    material.Diffuse(1.0);
    material.Specular(0.1);
    material.Shininess(10);
    large_sphere.SetMaterial(material);
    return large_sphere;
}

Sphere MediumPebble(double scale) {
    double scaled = 0.75 * scale;
    Sphere medium_sphere {};
    Matrix transform = Transformation()
        .Scale(scaled)
        .Translate(-scale, -(scale - scaled), -scale);
    medium_sphere.SetTransform(transform);
    Material material {};
    Colour colour { 128.0 / 255, 117.0 / 255, 90.0 / 255 };
    material.Surface(colour);
    material.Diffuse(1.0);
    material.Specular(0.1);
    material.Shininess(10);
    medium_sphere.SetMaterial(material);
    return medium_sphere;
}

Sphere SmallPebble(double scale) {
    double scaled = 0.5 * scale;
    Sphere small_sphere {};
    Matrix transform = Transformation()
        .Scale(scaled)
        .Translate(0.25 * scale, -(scale - scaled), -scale);
    small_sphere.SetTransform(transform);
    Material material {};
    Colour colour { 40.0 / 255, 34.0 / 255, 14.0 / 255 };
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
    wall.SetTransform(Transformation()
        .RotateX(M_PI/2)
        .Translate(0, 0, scale * 70)
    );
    return wall;
}

Sphere GlassMarble(double scale) {
    double scaled = 0.8 * scale;
    Sphere s {};
    s.SetMaterial(GlassMarbleMaterial(Colour { 0.1, 0.1, 0.1 }));
    s.SetTransform(Transformation()
        .Scale(scaled)
        .Translate(1.5*scale, -(scale - scaled), 2*scale)
    );
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

    Plane sand {};
    sand.SetTransform(Transformation().Translate(0, -scale, 0));
    SpeckledPattern speckled_ptn { Colour { 193.0 / 255, 154.0 / 255, 107.0 / 255 } };
    speckled_ptn.SetDarkThreshold(0.8);
    speckled_ptn.SetAttentuation(0.3);
    Material sand_material = FloorMaterial();
    sand_material.SurfacePattern(&speckled_ptn);
    sand.SetMaterial(sand_material);
    world.Add(&sand);

    Plane water = Water(scale);
    world.Add(&water);

    Sphere large_pebble = LargePebble(scale);
    world.Add(&large_pebble);

    Sphere medium_pebble = MediumPebble(scale);
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
