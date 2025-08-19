/*
The Ray Tracer Challenge: Chapter 11

Render a glass sphere containing an air bubble, like the one described on p. 159

Supply a scaling factor at the command line to increase the image dimensions.
*/

#include "challenges.h"
#include "plane.h"
#include "sphere.h"
#include "pattern.h"

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
    wall.SetMaterial(material);
    wall.SetTransform(Transformation().RotateX(M_PI/2).Translate(0, 0, scale * 50));
    return wall;
}

Sphere GlassMarble(double scale) {
    Sphere s {};
    s.SetMaterial(GlassMaterial(Colour { 0.5, 0.05, 0.05 }));
    s.SetTransform(Transformation().Scale(scale, scale, scale).Translate(0, scale, 0));
    return s;
}

Sphere AirBubble(double scale) {
    double scaled = scale * 0.98;
    Sphere s {};
    s.SetMaterial(AirBubbleMaterial());
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

    Camera camera = SceneCamera(scale, 108, 135, M_PI / 3, CameraTransform(scale));
    Canvas canvas = camera.Render(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;

    return 0;
}
