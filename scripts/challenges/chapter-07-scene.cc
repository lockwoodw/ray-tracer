/*
The Ray Tracer Challenge: Chapter 7 (and Chapter 8)

Render the scene described on pp. 105–107.

Supply a scaling factor at the command line to increase the image dimensions.
*/

#include <iostream>
#include "chapter-07-scene.h"
#include "camera.h"
#include "world.h"

Sphere Floor(const Material& material, double scale) {
    Sphere floor {};
    Matrix transform = Transformation().Scale(10 * scale, 0.01, 10 * scale);
    floor.SetTransform(transform);
    floor.SetMaterial(material);
    return floor;
}

Sphere LeftWall(const Material& material, double scale) {
    Sphere left_wall {};
    Matrix transform = Transformation()
        .Scale(10 * scale, 0.01, 10 * scale)
        .RotateX(M_PI / 2)
        .RotateY(-M_PI / 4)
        .Translate(0, 0, 5 * scale);
    left_wall.SetTransform(transform);
    left_wall.SetMaterial(material);
    return left_wall;
}

Sphere RightWall(const Material& material, double scale) {
    Sphere right_wall {};
    Matrix transform = Transformation()
        .Scale(10 * scale, 0.01, 10 * scale)
        .RotateX(M_PI / 2)
        .RotateY(M_PI / 4)
        .Translate(0, 0, 5 * scale);
    right_wall.SetTransform(transform);
    right_wall.SetMaterial(material);
    return right_wall;
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

    Material floor_material = FloorMaterial();
    Sphere objects[] = {
        Floor(floor_material, scale),
        LeftWall(floor_material, scale),
        RightWall(floor_material, scale),
        LargeSphere(scale),
        SmallerSphere(scale),
        SmallestSphere(scale)
    };

    World world {};

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