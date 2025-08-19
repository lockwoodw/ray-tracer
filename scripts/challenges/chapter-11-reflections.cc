/*
The Ray Tracer Challenge: Chapter 11

Render a scene demonstrating reflections (p. 149).

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

Material FloorMaterial(Pattern* pattern) {
    Material material {};
    material.Specular(0);
    material.Shininess(10);
    material.Reflectivity(0.5);
    material.SurfacePattern(pattern);
    return material;
}

Plane Horizon(double scale) {
    Plane wall {};
    Material material {};
    material.Surface(Colour(25.0 / 255, 25.0 / 255, 112.0 / 255));
    material.Specular(0);
    wall.SetMaterial(material);
    wall.SetTransform(Transformation()
        .RotateX(M_PI/2)
        .Translate(0, 0, scale * 50));
    return wall;
}

Sphere LargeSphere(double scale) {
    Sphere large_sphere {};
    Matrix transform = Transformation()
        .Scale(scale, scale, scale)
        .Translate(scale, scale, scale);
    large_sphere.SetTransform(transform);
    Material material {};
    Colour colour { 1, 1, 0 };
    material.Surface(colour);
    material.Diffuse(0.7);
    material.Specular(0.3);
    material.Shininess(10);
    material.Reflectivity(0.1);
    large_sphere.SetMaterial(material);
    return large_sphere;
}

Sphere SmallerSphere(double scale) {
    Sphere sphere;
    double halved = scale * 0.5;
    Matrix transform = Transformation()
        .Scale(halved, halved, halved)
        .Translate(-halved, halved, 0);
    sphere.SetTransform(transform);
    Material material {};
    Colour colour { 1, 0, 0 };
    material.Surface(colour);
    material.Reflectivity(0.05);
    sphere.SetMaterial(material);
    return sphere;
}

Matrix CameraTransform(double scale) {
    Point from { 0, 2 * scale, -5 * scale }, to { 0, scale, 0 };
    Vector up { 0, 1, 0 };
    return ViewTransform { from, to, up };
}

CheckerPattern FloorPattern(double scale) {
    double scaled = 2 * scale;
    CheckerPattern pattern { Colour::kBlack, Colour::kWhite };
    pattern.SetTransform(Transformation()
        .Scale(scaled)
        .RotateY(M_PI / 3)
        .Translate(scale, scale, 0)
    );
    return pattern;
}

int main(int argc, char** argv) {
    double scale = GetScale(argc, argv);

    World world {};

    Light light = WorldLight(scale);
    world.Add(&light);

    Plane floor {};
    CheckerPattern floor_pattern = FloorPattern(scale);
    Material floor_material = FloorMaterial(&floor_pattern);
    floor.SetMaterial(floor_material);
    world.Add(&floor);

    Plane horizon = Horizon(scale);
    world.Add(&horizon);

    Sphere large_sphere = LargeSphere(scale);
    world.Add(&large_sphere);

    Sphere smaller_sphere = SmallerSphere(scale);
    world.Add(&smaller_sphere);

    Camera camera = SceneCamera(scale, 108, 135, M_PI / 3, CameraTransform(scale));
    Canvas canvas = camera.Render(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;

    return 0;
}