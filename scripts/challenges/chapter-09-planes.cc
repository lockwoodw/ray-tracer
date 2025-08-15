/*
The Ray Tracer Challenge: Chapter 9

Render the scene described on Chapter 7 (pp. 105–107), replacing the
sphere-based floor and wall with planes.

Supply a scaling factor at the command line to increase the image dimensions.
*/

#include <iostream>
#include "chapter-07-scene.h"
#include "camera.h"
#include "world.h"
#include "plane.h"

Plane Floor() {
    Plane floor {};
    floor.SetMaterial(FloorMaterial());
    return floor;
}

Plane LeftWall(double scale) {
    Plane left_wall {};
    Matrix transform = Transformation()
        .RotateX(M_PI / 2)
        .RotateY(-M_PI / 4)
        .Translate(0, 0, 5 * scale);
    left_wall.SetTransform(transform);
    left_wall.SetMaterial(FloorMaterial());
    return left_wall;
}

Plane RightWall(double scale) {
    Plane right_wall {};
    Matrix transform = Transformation()
        .RotateX(M_PI / 2)
        .RotateY(M_PI / 4)
        .Translate(0, 0, 5 * scale);
    right_wall.SetTransform(transform);
    right_wall.SetMaterial(FloorMaterial());
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

    World world {};
    Plane floor = Floor(),
          left_wall = LeftWall(scale),
          right_wall = RightWall(scale);
    world.Add(&floor);
    world.Add(&left_wall);
    world.Add(&right_wall);

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

    Camera camera { 100 * scale_int, 50 * scale_int, M_PI / 3 };
    camera.SetTransform(CameraTransform(scale));

    Canvas canvas = camera.Render(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;

    return 0;
}