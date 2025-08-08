/*
g++ -I include/ -o build/pill-bottle-ppm src/utils.cc src/tuple.cc src/colour.cc \
src/space.cc src/matrix.cc src/transformations.cc src/shape.cc src/pattern.cc \
src/material.cc src/world.cc src/camera.cc src/canvas.cc src/plane.cc \
src/sphere.cc src/disc.cc src/cylinder.cc scripts/pill-bottle-ppm.cc
*/

#define _USE_MATH_DEFINES // for M_PI

#include <cmath>
#include <iostream>
#include <vector>

#include "space.h"
#include "colour.h"
#include "material.h"
#include "world.h"
#include "plane.h"
#include "camera.h"
#include "canvas.h"
#include "sphere.h"
#include "pattern.h"
#include "cylinder.h"
#include "sheet.h"
#include "disc.h"

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
    Point origin { 2*scale, 10*scale, 0};
    Colour colour { 1, 1, 1 };
    return Light { origin, colour };
}

Matrix CameraTransform(double scale) {
    Point from { 3*scale, 6*scale, -6 * scale }, to { 0, 6*scale, 0 };
    // Point from { 0, 6*scale, -7 * scale }, to { 0, 6*scale, 0 };
    // Point from { 5*scale, 6*scale, -7 * scale }, to { 0, 6*scale, -3*scale };
    // Point from { 2*scale, 0.25*scale, -scale }, to { 0, 0.25*scale, 0 };
    Vector up { 0, 1, 0 };
    return ViewTransform { from, to, up };
}

Material GlassMaterial(const Colour& colour = Colour::kWhite) {
    Material m {};
    m.Transparency(1.0);
    m.RefractiveIndex(1.52);
    m.Diffuse(0.1);
    m.Ambient(0.1);
    m.Reflectivity(1.0);
    m.Shininess(300);
    m.Specular(1);
    m.Surface(colour);
    m.CastsShadow(true);
    return m;
}

Material AirBubbleMaterial() {
    Material m {};
    m.Transparency(1.0);
    m.RefractiveIndex(1.00029);
    m.Diffuse(0);
    m.Ambient(0);
    m.Specular(0);
    m.Reflectivity(0);
    m.Shininess(10);
    m.CastsShadow(false);
    return m;
}

int main(int argc, char** argv) {
    double scale = GetScale(argc, argv);
    int scale_int = static_cast<int>(scale);

    World world {};

    Light light = WorldLight(scale);
    world.Add(&light);

    Plane floor {};

    SpeckledPattern* speckled_ptn = new SpeckledPattern(Colour { 0.75, 0.75, 0.75 });
    speckled_ptn->SetDarkThreshold(0.5);
    speckled_ptn->SetAttentuation(0.3);
    SolidPattern* solid_ptn = new SolidPattern(Colour { 0.9, 0.9, 0.9 });
    StripePattern striped_ptn { speckled_ptn, solid_ptn };
    striped_ptn.SetTransform(Transformation().Scale(0.4*scale, 1, 1).RotateY(M_PI/2));
    PerturbedPattern pptn = PerturbedPattern { &striped_ptn };
    Material floor_m {};
    floor_m.SurfacePattern(&pptn);
    floor.SetMaterial(floor_m);
    world.Add(&floor);

    Plane wall {};
    wall.SetTransform(
        Transformation()
        .RotateX(M_PI/2)
        .Translate(0, 0, 15*scale)
    );
    world.Add(&wall);

    // Sheet mirror {};
    // mirror.SetTransform(
    //     Transformation()
    //     .Scale(scale*10)
    //     .RotateX(M_PI/2)
    //     .Translate(0, scale*6, 2*scale)
    // );
    Material mirror_m {};
    mirror_m.Surface(Colour { 0.5, 0.5, 0.5 });
    // mirror_m.Transparency(1.0);
    mirror_m.Reflectivity(1.0);
    // mirror_m.RefractiveIndex(1.52);
    mirror_m.Diffuse(0.1);
    mirror_m.Shininess(300);
    mirror_m.Specular(0);
    wall.SetMaterial(mirror_m);
    // world.Add(&mirror);

    Plane back_wall {};
    back_wall.SetTransform(
        Transformation()
        .RotateX(M_PI/2)
        .Translate(0, 0, -15*scale)
    );
    Material back_wall_m {};
    back_wall_m.Surface(Colour { 0.2, 0.2, 0.3 });
    back_wall_m.Reflectivity(0);
    back_wall_m.Shininess(10);
    back_wall_m.Specular(0);
    back_wall.SetMaterial(back_wall_m);
    world.Add(&back_wall);

    double x_offset = -2*scale,
           z_offset = scale;

    Sphere red_pill {};
    red_pill.SetTransform(
        Transformation()
        .Scale(0.25 * scale, 0.5 * scale, 0.25 * scale)
        .RotateZ(M_PI/2)
        .Translate(x_offset, scale*(0.25), z_offset)
    );
    Material red_pill_m {};
    red_pill_m.Surface(Colour { 1.0, 0, 0 });
    red_pill.SetMaterial(red_pill_m);
    world.Add(&red_pill);

    Sphere blue_pill {};
    blue_pill.SetTransform(
        Transformation()
        .Scale(0.25 * scale, 0.5 * scale, 0.25 * scale)
        .RotateZ(M_PI/2)
        .RotateY(M_PI/2)
        .Translate(0, scale*(0.25), 0)
    );
    Material blue_pill_m {};
    blue_pill_m.Surface(Colour { 0, 0, 1.0 });
    blue_pill.SetMaterial(blue_pill_m);
    world.Add(&blue_pill);

    Cylinder bottle { 0, 4, true };
    bottle.SetTransform(
        Transformation()
        .Scale(scale)
        .Translate(x_offset, 0, z_offset)
    );
    Colour orange { 204.0 / 255, 85.0 / 255, 0 };
    bottle.SetMaterial(GlassMaterial(orange));
    world.Add(&bottle);

    Cylinder bottle_air { 0, 4, true };
    bottle_air.SetTransform(
        Transformation()
        .Scale(scale*0.90, scale, scale*0.9)
        .Translate(x_offset, scale*0.1, z_offset)
    );
    bottle_air.SetMaterial(AirBubbleMaterial());
    world.Add(&bottle_air);


    Cylinder cap { 0, 1, true };
    cap.SetTransform(
        Transformation()
        .Scale(1.1* scale, 0.9 * scale, 1.1 * scale)
        .Translate(x_offset, 4*scale, z_offset)
    );
    StripePattern cap_ptn { Colour::kWhite, Colour { 220.0 / 255, 220.0 / 255, 220.0 / 255 } };
    cap_ptn.SetTransform(
        Transformation()
        .Scale(0.04)
    );
    Material cap_m {};
    cap_m.Shininess(10);
    cap_m.Specular(0.1);
    cap_m.SurfacePattern(&cap_ptn);
    // cap_m.CastsShadow(false);
    cap.SetMaterial(cap_m);
    // Colour orange { 1.0, 140.0 / 255, 0 };
    // cylinder.SetMaterial(GlassMaterial(orange));
    world.Add(&cap);

    Disc cap_label {};
    cap_label.SetTransform(
        Transformation()
        .Scale(scale*0.9, scale*1.1, scale*0.9)
        .Translate(x_offset, 4*scale + 0.91 * scale, z_offset)
    );
    Material cap_label_m { Colour { 1.0 , 1.0, 1.0 }, 0.1, 0.9, 0.9, 200.0 };
    cap_label_m.CastsShadow(false);
    cap_label.SetMaterial(cap_label_m);
    world.Add(&cap_label);


    Light inner { Point { x_offset, 3.93*scale, z_offset }, Colour { 0.5, 0.5, 0.5 } };
    world.Add(&inner);

    Disc bottle_floor {};
    bottle_floor.SetTransform(
        Transformation()
        .Scale(scale, 1, scale)
        .Translate(x_offset, scale*0.05, z_offset)
    );
    Material bottle_floor_m;
    bottle_floor_m.Surface(orange);
    bottle_floor_m.Transparency(1.0);
    bottle_floor_m.Reflectivity(0);
    bottle_floor_m.Shininess(10);
    bottle_floor_m.Specular(0);
    bottle_floor_m.Ambient(0.1);
    bottle_floor_m.Diffuse(0.1);
    bottle_floor.SetMaterial(bottle_floor_m);
    world.Add(&bottle_floor);


    Camera camera { 108 * scale_int, 135 * scale_int, M_PI / 2 };
    camera.SetTransform(CameraTransform(scale));

    Canvas canvas = camera.Render(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;

    return 0;
}