/*
g++ -I include/ -o build/bvh-test src/utils.cc src/tuple.cc src/colour.cc \
src/space.cc src/matrix.cc src/transformations.cc src/bounds.cc src/shape.cc \
src/pattern.cc src/material.cc src/world.cc src/camera.cc src/canvas.cc \
src/plane.cc src/group.cc src/sphere.cc src/sheet.cc scripts/bvh-test.cc
*/

#define _USE_MATH_DEFINES // for M_PI

#include <cmath>
#include <iostream>
#include <vector>

#include "space.h"
#include "colour.h"
#include "material.h"
// #include "shape.h"
#include "world.h"
#include "plane.h"
#include "camera.h"
#include "canvas.h"
#include "sphere.h"
#include "pattern.h"
#include "sphere.h"
#include "group.h"
#include "sheet.h"
// #include "disc.h"

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
    Point origin { 30*scale, 30 * scale, -30*scale };
    Colour colour { 1, 1, 1 };
    return Light { origin, colour };
}

Matrix CameraTransform(double scale) {
    Point from { 30*scale, 30*scale, -40 * scale }, to { 20*scale, 25*scale, 0 };
    Vector up { 0, 1, 0 };
    return ViewTransform { from, to, up };
}

Camera SceneCamera(double scale, int width, int height, double fov) {
    int scale_int = static_cast<int>(scale);
    Camera camera { width * scale_int, height * scale_int, fov };
    camera.SetTransform(CameraTransform(scale));
    return camera;
}

int main(int argc, char** argv) {
    double scale = GetScale(argc, argv);

    World world {};

    Light light = WorldLight(scale);
    world.Add(&light);

    std::vector<Colour> colours = {
        Colour { 1, 0, 0 },
        Colour { 0, 1, 0 },
        Colour { 0, 0, 1 },
        Colour { 1, 1, 0 },
        Colour { 0, 1, 1 },
        Colour { 1, 0, 1 }
    };

    ShapeGroup shapes {};
    world.Add(&shapes);

    std::vector<Shape*> objects {};
    int dim { 10 };
    for (int y = 0; y < dim; y++) {
        // ShapeGroup* zgroup = new ShapeGroup();
        // objects.push_back(zgroup);
        // shapes.Add(zgroup);
        for (int z = 0; z < dim; z++) {
            // ShapeGroup* ygroup = new ShapeGroup();
            // objects.push_back(ygroup);
            // zgroup->Add(ygroup);
            for (int x = 0; x < dim; x++) {
                Sphere* s = new Sphere();
                objects.push_back(s);
                // ygroup->Add(s);
                shapes << s;
                // world.Add(s);
                s->SetTransform(
                    Transformation()
                    .Scale(scale)
                    .Translate(2*x*scale, 2*y*scale, 2*z*scale)
                );
                Material m {};
                m.Surface(colours[(x + y + z) % colours.size()]);
                s->SetMaterial(m);
            }
        }
    }

    // BrickFactory bf {};
    // int n_bricks { 5 };
    // int brick_sz { 5 };
    // for (int i = 0; i < n_bricks; i++) {
    //     Colour c = colours[i % colours.size()];
    //     ShapeGroup* brick = bf.Generate(brick_sz, brick_sz, brick_sz, c);
    //     brick->SetTransform(Transformation().Translate(0, 1 + i*2*brick_sz, 0));
    //     world.Add(brick);
    // }
    shapes.Divide(200);

    // Plane ground {};
    // world.Add(&ground);

    Camera camera = SceneCamera(scale, 108, 135, M_PI / 3);
    Canvas canvas = camera.Render(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;
 
    for (auto o: objects) {
        delete o;
    }

    return 0;
}

