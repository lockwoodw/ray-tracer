/*
The Ray Tracer Challenge: Chapter 13

Render a scene using cylinders.

Supply a scaling factor at the command line to increase the image dimensions.
*/

#include <vector>

#include "challenges.h"
#include "plane.h"
#include "sphere.h"
#include "pattern.h"
#include "cylinder.h"
#include "disc.h"

Light WorldLight(double scale) {
    Point origin { 2*scale, 10*scale, 0};
    Colour colour { 1, 1, 1 };
    return Light { origin, colour };
}

Matrix CameraTransform(double scale) {
    Point from { 7*scale, 7*scale, -11 * scale }, to { 0, 7*scale, 0 };
    Vector up { 0, 1, 0 };
    return ViewTransform { from, to, up };
}

Material CounterMaterial(double scale) {
    static SpeckledPattern speckled_ptn { Colour { 0.75, 0.75, 0.75 } };
    speckled_ptn.SetDarkThreshold(0.5);
    speckled_ptn.SetAttentuation(0.3);

    static SolidPattern solid_ptn { Colour { 0.9, 0.9, 0.9 } };

    static StripePattern striped_ptn { &speckled_ptn, &solid_ptn };
    striped_ptn.SetTransform(Transformation().Scale(0.05*scale, 1, 1).RotateY(M_PI/2));

    static PerturbedPattern perturbed_ptn { &striped_ptn };

    return Material ()
        .SurfacePattern(&perturbed_ptn)
        .Reflectivity(0.3);
}

Material MirrorMaterial() {
    return Material ()
        .Surface(Colour { 0.5, 0.5, 0.5 })
        .Reflectivity(1.0)
        .Diffuse(0.1)
        .Shininess(300)
        .Specular(0);
}

Material WallMaterial() {
    return Material()
        .Surface(Colour { 0.2, 0.2, 0.3 })
        .Reflectivity(0)
        .Shininess(10)
        .Specular(0);
}

Material CapMaterial() {
    static StripePattern cap_ptn { Colour::kWhite,
        Colour { 220.0 / 255, 220.0 / 255, 220.0 / 255 } };

    cap_ptn.SetTransform(
        Transformation()
        .Scale(0.04)
    );
    return Material()
        .Shininess(10)
        .Specular(0.1)
        .SurfacePattern(&cap_ptn);
}

Material BottleBottomMaterial(const Colour& colour) {
    return Material()
        .Surface(colour)
        .Transparency(1.0)
        .Reflectivity(0)
        .Shininess(10)
        .Specular(0)
        .Ambient(0.1)
        .Diffuse(0.1);
}

int main(int argc, char** argv) {
    double scale = GetScale(argc, argv);

    World world {};

    Light light = WorldLight(scale);
    world.Add(&light);

    Plane counter {};
    world.Add(&counter);

    counter.SetMaterial(CounterMaterial(scale));

    Plane mirror {};
    world.Add(&mirror);

    mirror.SetTransform(
        Transformation()
        .RotateX(M_PI/2)
        .Translate(0, 0, 15*scale)
    );

    mirror.SetMaterial(MirrorMaterial());

    Plane back_wall {};
    world.Add(&back_wall);

    back_wall.SetTransform(
        Transformation()
        .RotateX(M_PI/2)
        .Translate(0, 0, -15*scale)
    );

    back_wall.SetMaterial(WallMaterial());

    double x_offset = -2*scale,
           z_offset = scale;

    Sphere red_pill {};
    world.Add(&red_pill);

    red_pill.SetTransform(
        Transformation()
        .Scale(0.25 * scale, 0.5 * scale, 0.25 * scale)
        .RotateZ(M_PI/2)
        .Translate(x_offset, scale*(0.25), z_offset)
    );

    red_pill.SetMaterial(Material().Surface(Colour { 1.0, 0, 0 }));

    Sphere blue_pill {};
    world.Add(&blue_pill);

    blue_pill.SetTransform(
        Transformation()
        .Scale(0.25 * scale, 0.5 * scale, 0.25 * scale)
        .RotateZ(M_PI/2)
        .RotateY(M_PI/2)
        .Translate(0, scale*(0.25), 0)
    );

    blue_pill.SetMaterial(Material().Surface(Colour { 0, 0, 1.0 }));

    Cylinder bottle { 0, 4, true };
    world.Add(&bottle);

    bottle.SetTransform(
        Transformation()
        .Scale(scale)
        .Translate(x_offset, 0, z_offset)
    );

    Colour orange { 204.0 / 255, 85.0 / 255, 0 };
    bottle.SetMaterial(GlassMaterial(orange));

    Cylinder bottle_air { 0, 4, true };
    world.Add(&bottle_air);

    bottle_air.SetTransform(
        Transformation()
        .Scale(scale*0.90, scale, scale*0.9)
        .Translate(x_offset, scale*0.1, z_offset)
    );

    bottle_air.SetMaterial(AirBubbleMaterial());

    Cylinder cap { 0, 1, true };
    world.Add(&cap);

    cap.SetTransform(
        Transformation()
        .Scale(1.1* scale, 0.9 * scale, 1.1 * scale)
        .Translate(x_offset, 4*scale, z_offset)
    );

    cap.SetMaterial(CapMaterial());

    Disc cap_label {};
    world.Add(&cap_label);

    cap_label.SetTransform(
        Transformation()
        .Scale(scale*0.9, scale*1.1, scale*0.9)
        .Translate(x_offset, 4*scale + 0.91 * scale, z_offset)
    );

    cap_label.SetMaterial(
        Material(Colour { 1.0 , 1.0, 1.0 }, 0.1, 0.9, 0.9, 200.0)
        .CastsShadow(false)
    );

    // Illuminate the inside of the bottle so the pill is visible
    Light inner { Point { x_offset, 3.93*scale, z_offset }, Colour { 0.5, 0.5, 0.5 } };
    world.Add(&inner);

    // Disc required to prevent the bottom floor of the bottle reflecting the
    // inner light
    Disc bottle_floor {};
    world.Add(&bottle_floor);

    bottle_floor.SetTransform(
        Transformation()
        .Scale(scale, 1, scale)
        .Translate(x_offset, scale*0.05, z_offset)
    );

    bottle_floor.SetMaterial(BottleBottomMaterial(orange));

    Camera camera = SceneCamera(scale, 108, 135, M_PI / 3, CameraTransform(scale));
    Canvas canvas = camera.RenderConcurrent(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;

    return 0;
}