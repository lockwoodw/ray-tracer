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

Light WorldLight(double scale) {
    double scaled = 10 * scale;
    Point origin { 0, scaled, scaled };
    Colour colour { 1, 1, 1 };
    return Light { origin, colour };
}

Matrix CameraTransform(double scale) {
    Point from { 0, scale, -5 * scale }, to { 0, 0, 0 };
    Vector up { 0, 1, 0 };
    return ViewTransform { from, to, up };
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

    Cube floor {};

    Colour red { 1, 0, 0 }, pink { 1, 182.0 / 255, 193.0 /255 }, white { 1, 1, 1 }, black { 0, 0, 0 };
    SolidPattern red_p { red }, pink_p { pink }, white_p { white }, black_p { black };
    StripePattern striped_1 { &red_p, &pink_p }, striped_2 { white, black };
    double rotation { M_PI / 4 };
    striped_1.SetTransform(Transformation().Scale(0.25, 1, 1).RotateY(rotation));
    striped_2.SetTransform(Transformation().Scale(0.25, 1, 1).RotateY(-rotation));
    CheckerPattern checked { &striped_1, &striped_2 };
    // checked.SetTransform(Transformation().RotateY(rotation));

    Material m1 {};
    m1.Specular(0);
    m1.SurfacePattern(&checked);
    floor.SetMaterial(m1);

    world.Add(&floor);


    Light light = WorldLight(scale);
    world.Add(&light);

    Camera camera { 100 * static_cast<int>(scale), 50 * static_cast<int>(scale), M_PI / 12 };
    camera.SetTransform(CameraTransform(scale));

    Canvas canvas = camera.Render(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;

    return 0;
}