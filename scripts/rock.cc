/*
g++ -I include/ -o build/rock src/utils.cc src/tuple.cc src/colour.cc \
src/space.cc src/matrix.cc src/transformations.cc src/bounds.cc src/shape.cc src/pattern.cc \
src/material.cc src/world.cc src/camera.cc src/canvas.cc src/plane.cc src/group.cc \
src/sphere.cc scripts/rock.cc
*/

#define _USE_MATH_DEFINES // for M_PI

#include <cmath>
#include <iostream>

#include "scripts.h"
#include "plane.h"
#include "sphere.h"
#include "pattern.h"
#include "group.h"

Light WorldLight(double scale) {
    Point origin { -5*scale, 5 * scale, -5*scale };
    Colour colour { 1, 1, 1 };
    return Light { origin, colour };
}

Matrix CameraTransform(double scale) {
    Point from { -0.5*scale, 3*scale, -6.5* scale }, to { -0.5*scale, 2*scale, 0 };
    Vector up { 0, 1, 0 };
    return ViewTransform { from, to, up };
}

int main(int argc, char** argv) {
    double scale = GetScale(argc, argv);
    int scale_int = static_cast<int>(scale);

    World world {};

    Light light = WorldLight(scale);
    world.Add(&light);

    Plane ground {};
    world.Add(&ground);

    ShapeGroup rocks {};
    world.Add(&rocks);

    Sphere rock_1 {};
    rock_1.SetTransform(
        Transformation()
        .Scale(2.2*scale, scale, 1.5*scale)
        // .Scale(scale)
        // .Shear(2, 0, 0, 0, 0, 2)
        .Translate(-2*scale, scale, scale)
    );
    rocks.Add(&rock_1);

    Colour pale_brown { 152.0 / 255, 118.0 / 255, 84.0 / 255 },
           wood_brown { 193.0 / 255, 154.0 / 255, 107.0 / 255 };

    
    SpeckledPattern pale_ptn { pale_brown };
    pale_ptn.SetDarkThreshold(0.8);
    SpeckledPattern wood_ptn { wood_brown };
    wood_ptn.SetDarkThreshold(0.8);
    // umber_ptn.SetDarkThreshold(0.8);
    wood_ptn.SetLightThreshold(0.8);
    wood_ptn.SetAttentuation(0.8);

    // GradientPattern gradient_ptn { pale_brown, wood_brown };
    RadialGradientPattern gradient_ptn { &pale_ptn, &wood_ptn };
    // gradient_ptn.SetTransform(Transformation().Scale(scale/1.2, scale, scale/1.5));
    gradient_ptn.SetTransform(rock_1.InverseTransform());
    // gradient_ptn.SetTransform(rock_1.Transform());

    PerturbedPattern perturbed_ptn { &gradient_ptn };
    // perturbed_ptn.SetTransform(rock_1.InverseTransform());

    // SpeckledPattern speckled_ptn { Colour { 193.0/255, 154.0/255, 107.0/255 } };
    // speckled_ptn.SetDarkThreshold(0.8);
    // speckled_ptn.SetAttentuation(0.3);

    // AveragePatternBlender blender {};
    // BlendedPattern blended_ptn { &blender };
    // blended_ptn.Add(&perturbed_ptn).Add(&speckled_ptn);

    Material rock_m = Material()
        .SurfacePattern(&perturbed_ptn)
        .Shininess(10)
        .Specular(0.1)
        .Diffuse(1);
    rock_1.SetMaterial(rock_m);

    Sphere rock_2 {};
    rock_2.SetTransform(
        Transformation()
        .Scale(scale)
        .Translate(scale, scale, -scale)
        .Shear(0, 0, 0, 0, 0, 2)
    );
    rocks.Add(&rock_2);

    Colour spanish_bistre { 128.0 / 255, 117.0 / 255, 90.0 / 255 },
           raw_umber      { 130.0 / 255, 102.0 / 255, 68.0 / 255 };
    
    SpeckledPattern bistre_ptn { spanish_bistre };
    bistre_ptn.SetDarkThreshold(0.8);
    SpeckledPattern umber_ptn { raw_umber };
    umber_ptn.SetDarkThreshold(0.8);
    umber_ptn.SetLightThreshold(0.8);
    umber_ptn.SetAttentuation(0.8);

    RadialGradientPattern gradient_ptn2 { &bistre_ptn, &umber_ptn };
    gradient_ptn2.SetTransform(rock_2.InverseTransform());

    PerturbedPattern perturbed_ptn2 { &gradient_ptn2 };

    Material rock2_m = Material()
        .SurfacePattern(&perturbed_ptn2)
        .Shininess(10)
        .Specular(0.1)
        .Diffuse(1);
    rock_2.SetMaterial(rock2_m);

    // ground.SetTransform(
    //     Transformation()
    //     .Scale(scale)
    //     .Shear(5, 0, 0, 0, 0, 0)
    // );

    Colour almond          { 239.0 / 255, 222.0 / 255, 205.0 / 255 },
           blanched_almond { 1.0, 235.0 / 255, almond.Blue() };

    SpeckledPattern almond_ptn { almond };
    almond_ptn.SetDarkThreshold(0.8);
    almond_ptn.SetAttentuation(0.8);
    SpeckledPattern blanched_ptn { blanched_almond };



    RadialGradientPattern ground_gradient_ptn { &almond_ptn, &blanched_ptn };
    // gradient_ptn.SetTransform(Transformation().Scale(scale/1.2, scale, scale/1.5));
    ground_gradient_ptn.SetTransform(ground.InverseTransform());
    // gradient_ptn.SetTransform(rock_1.Transform());

    PerturbedPattern ground_perturbed_ptn { &ground_gradient_ptn };
    Material ground_m = Material()
        .SurfacePattern(&ground_perturbed_ptn);
    ground.SetMaterial(ground_m);

    Camera camera = SceneCamera(scale, 108, 135, M_PI / 3, CameraTransform(scale));
    Canvas canvas = camera.Render(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;
    return 0;
}
