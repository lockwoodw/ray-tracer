#include "scripts.h"
#include "plane.h"
#include "sphere.h"

Light WorldLight(double scale) {
    Point origin { -10 * scale, 5 * scale, -10 * scale };
    Colour colour { 1, 1, 1 };
    return Light { origin, colour };
}

Matrix CameraTransform(double scale) {
    Point from { -scale, 3 * scale, -4 * scale }, to { -scale, 0, 0 };
    Vector up { 0, 1, 0 };
    return ViewTransform { from, to, up };
}

int main(int argc, char** argv) {
    double scale = GetScale(argc, argv);

    World world {};

    Light light = WorldLight(scale);
    world.Add(&light);

    Plane p {};
    world.Add(&p);

    Sphere s1 {};
    world.Add(&s1);
    s1.SetTransform(
        Transformation()
        .Scale(scale)
        .Translate(0, scale, 0)
    );
    s1.SetMaterial(Material().Surface(Colour(1, 0, 0)));

    Sphere s2 {};
    world.Add(&s2);
    s2.SetTransform(
        Transformation()
        .Scale(scale)
        .Translate(-2*scale, scale, 0)
    );
    s2.SetMaterial(Material().Surface(Colour(0, 0, 1)));

    Camera camera = SceneCamera(scale, 108, 135, M_PI / 3, CameraTransform(scale));
    Canvas canvas = camera.Render(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;
    return 0;
}