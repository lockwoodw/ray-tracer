#include <vector>

#include "scripts.h"
#include "plane.h"
#include "sphere.h"
#include "pattern.h"
#include "group.h"
#include "disc.h"

Matrix CameraTransform(double scale) {
    Point from { 0.3*scale, scale, -1.1*scale }, to { 0, scale, 0 };
    Vector up { 0, 1, 0 };
    return ViewTransform { from, to, up };
}

Material MirrorMaterial(const Colour& colour = Colour::kWhite) {
    return Material()
        .Diffuse(0.1)
        .Ambient(0.1)
        .Reflectivity(1.0)
        .Shininess(300)
        .Specular(1)
        .Surface(colour)
        .CastsShadow(false);
}

Pattern* BigPlanetSurfacePattern(double scale) {
    static SpeckledPattern speckled_ptn { Colour { 0.2, 0.4, 0.5 } };
    static SolidPattern solid_ptn  { Colour { 0, 0, 0 } };
    static StripePattern striped_ptn { &speckled_ptn, &solid_ptn };
    static PerturbedPattern perturbed_ptn { &striped_ptn };

    speckled_ptn.SetDarkThreshold(0.5);
    speckled_ptn.SetAttentuation(0.6);
    striped_ptn.SetTransform(Transformation().Scale(15*scale));

    return &perturbed_ptn;
}

Material BigPlanetMaterial(Pattern* ptn) {
    return Material()
        .SurfacePattern(ptn)
        .Ambient(0)
        .Diffuse(1)
        .Shininess(0)
        .Specular(0)
        .Reflectivity(1.0)
        .CastsShadow(false);
}

Material SmallPlanetMaterial() {
    static SpeckledPattern planet_ptn { Colour { 0.5, 1.0, 0.6 } };
    return Material()
        .SurfacePattern(&planet_ptn)
        .Shininess(0)
        .Specular(0)
        .Ambient(0)
        .Diffuse(1)
        .CastsShadow(false);
}

Material RingMaterial() {
    static SpeckledPattern ring_ptn { Colour { 1, 1, 0 } };
    ring_ptn.SetDarkThreshold(0.9);
    ring_ptn.SetLightThreshold(0.9);
    ring_ptn.SetAttentuation(0.6);

    return Material()
        .SurfacePattern(&ring_ptn)
        .CastsShadow(false)
        .Transparency(1.0)
        .Ambient(1)
        .Diffuse(0)
        .Shininess(300)
        .Specular(1);
}

int main(int argc, char** argv) {
    double scale = GetScale(argc, argv);

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

    Plane visible {};
    world.Add(&visible);

    visible.SetTransform(
        Transformation()
        .RotateX(M_PI/2)
        .Translate(0, 0, 20*scale)
    );

    visible.SetMaterial(MirrorMaterial(Colour { 0.5, 0.5, 0.5 }));

    Plane big_planet_surface {};
    world.Add(&big_planet_surface);

    big_planet_surface.SetTransform(
        Transformation()
        .Translate(0, -30*scale, 0)
    );

    big_planet_surface.SetMaterial(
        BigPlanetMaterial(
            BigPlanetSurfacePattern(scale)
        )
    );

    Plane hidden {};
    world.Add(&hidden);

    hidden.SetTransform(
        Transformation()
        .RotateX(M_PI/2)
        .RotateY(M_PI/12)
        .Translate(0, 0, -5*scale)
    );

    hidden.SetMaterial(MirrorMaterial(Colour::kBlack));

    ShapeGroup small_planet_group {};
    world.Add(&small_planet_group);

    Sphere planet {};
    small_planet_group << &planet;

    SpeckledPattern planet_ptn { Colour { 0.5, 1.0, 0.6 } };

    planet.SetTransform(Transformation().Scale(scale/2));

    planet.SetMaterial(SmallPlanetMaterial());

    Disc ring {};
    small_planet_group << &ring;

    ring.SetTransform(
        Transformation()
        .Scale(scale*0.9)
        .RotateZ(-M_PI/8)
        .RotateX(M_PI/6)
    );

    ring.SetMaterial(RingMaterial());

    small_planet_group.SetTransform(
        Transformation()
        .Translate(0, 0, 5*scale)
    );

    Camera camera = SceneCamera(scale, 108, 135, M_PI / 3, CameraTransform(scale));
    Canvas canvas = camera.Render(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;

    return 0;
}
