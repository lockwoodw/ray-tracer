/*
g++ -I include/ -o build/will-o-wisps src/utils.cc src/tuple.cc src/colour.cc \
src/space.cc src/matrix.cc src/transformations.cc src/bounds.cc src/shape.cc \
src/pattern.cc src/material.cc src/world.cc src/camera.cc src/canvas.cc \
src/plane.cc src/group.cc src/disc.cc src/sphere.cc scripts/will-o-wisps.cc
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
// #include "sheet.h"
#include "group.h"
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

Matrix CameraTransform(double scale) {
    Point from { 0.25*scale, scale, -scale }, to { 0, scale, 0 };
    Vector up { 0, 1, 0 };
    return ViewTransform { from, to, up };
}

Camera SceneCamera(double scale, int width, int height, double fov) {
    int scale_int = static_cast<int>(scale);
    Camera camera { width * scale_int, height * scale_int, fov };
    camera.SetTransform(CameraTransform(scale));
    return camera;
}

Material WallMaterial(const Colour& colour = Colour::kWhite) {
    Material m = Material()
        // .Transparency(1.0)
        // .RefractiveIndex(1.52)
        .Diffuse(0.1)
        .Ambient(0.1)
        .Reflectivity(1.0)
        .Shininess(300)
        .Specular(1)
        .Surface(colour)
        .CastsShadow(false);
    return m;
}

int main(int argc, char** argv) {
    double scale = GetScale(argc, argv);
    int scale_int = static_cast<int>(scale);

    World world {};

    Light light_1 { Point { 10*scale, 10*scale, 0 }, Colour { 0.5, 0.5, 0.5 } };
    light_1.CastsShadow(false);
    world.Add(&light_1);

    Light light_2 { Point { -10*scale, 5*scale, 0 }, Colour { 0.5, 0, 0 } };
    light_2.CastsShadow(false);
    world.Add(&light_2);

    Light light_3 { Point { -5*scale, 10*scale, -1*scale }, Colour { 0.2, 0, 0.2 } };
    light_3.CastsShadow(false);
    world.Add(&light_3);

    Light light_4 { Point { 15*scale, 25*scale, 0 }, Colour { 0, 0, 0.2 } };
    light_4.CastsShadow(false);
    world.Add(&light_4);

    Plane ground {};
    Material ground_m {};
    ground_m.Surface(Colour { 1.0, 0, 0, });
    ground.SetMaterial(ground_m);
    // world.Add(&ground);

    Plane visible {};
    visible.SetTransform(
        Transformation()
        .RotateX(M_PI/2)
        .Translate(0, 0, 20*scale)
    );
    visible.SetMaterial(WallMaterial(Colour { 0.5, 0.5, 0.5 }));
    world.Add(&visible);
    
    Plane middle {};
    Material middle_m = WallMaterial(Colour { 0.2, 0.4, 1.0 });
    
    SpeckledPattern* speckled_ptn = new SpeckledPattern(Colour { 0.2, 0.4, 0.5 });
    speckled_ptn->SetDarkThreshold(0.5);
    speckled_ptn->SetAttentuation(0.6);
    SolidPattern* solid_ptn = new SolidPattern(Colour { 0, 0, 0 });
    StripePattern striped_ptn { speckled_ptn, solid_ptn };
    striped_ptn.SetTransform(Transformation().Scale(15*scale));//.RotateY(M_PI/4)
    PerturbedPattern pptn = PerturbedPattern { &striped_ptn };
    middle_m.SurfacePattern(&pptn);
    middle_m.Ambient(0);
    middle_m.Diffuse(1);
    middle_m.Shininess(0);
    middle_m.Specular(0);
    // middle_m.Transparency(1);
    // middle_m.RefractiveIndex(1);
    middle.SetMaterial(middle_m);
    middle.SetTransform(
        Transformation()
        // .RotateX(M_PI/24)
        .Translate(0, -30*scale, 0)
    );
    world.Add(&middle);
    
    // Light light_5 { Point { 0, -29*scale, 0 }, Colour { 0.2, 0.2, 0.2 } };
    // world.Add(&light_5);

    Plane hidden {};
    hidden.SetTransform(
        Transformation()
        .RotateX(M_PI/2)
        .RotateY(M_PI/12)
        .Translate(0, 0, -5*scale)
    );
    hidden.SetMaterial(WallMaterial(Colour::kBlack));
    world.Add(&hidden);



    ShapeGroup planet_group {};
    Sphere planet {};

    SpeckledPattern planet_ptn { Colour { 0.5, 1.0, 0.6 } };
    // planet_ptn.SetDarkThreshold(0.5);
    // planet_ptn.SetLightThreshold(0.9);
    // planet_ptn.SetAttentuation(0.5);

    Material planet_m {};
    planet_m
        .SurfacePattern(&planet_ptn)
        .Shininess(0)
        .Specular(0)
        .Ambient(0)
        .Diffuse(1)
        .CastsShadow(false);
    // planet_m.Surface(Colour::kBlack);

    planet.SetMaterial(planet_m);

    planet.SetTransform(Transformation().Scale(scale/2));

    planet_group << &planet;

    Disc ring {};

    SpeckledPattern ring_ptn { Colour { 1, 1, 0 } };
    ring_ptn.SetDarkThreshold(0.9);
    ring_ptn.SetLightThreshold(0.9);
    ring_ptn.SetAttentuation(0.6);
    // ring_ptn.SetTransform(Transformation().Scale(0.1*scale, 1, 1));

    Material ring_m {};
    ring_m.SurfacePattern(&ring_ptn);
    ring_m.CastsShadow(false);
    ring_m.Transparency(1.0);
    ring_m.Ambient(1);
    // ring_m.Diffuse(0);
    ring_m.Shininess(300);
    ring_m.Specular(1);
    ring.SetMaterial(ring_m);

    ring.SetTransform(Transformation().Scale(scale*0.9).RotateZ(-M_PI/8).RotateX(M_PI/6));
    planet_group << &ring;

    planet_group.SetTransform(
        Transformation()
        .Translate(0, 0, 5*scale)
    );

    world.Add(&planet_group);



    Camera camera = SceneCamera(scale, 108, 135, M_PI / 3);
    Canvas canvas = camera.Render(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;
 
    // for (auto o: objects) {
    //     delete o;
    // }

    return 0;
}
