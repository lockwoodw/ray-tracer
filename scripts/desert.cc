#include "scripts.h"
#include "plane.h"
#include "porous-sheet.h"
#include "polyhedron.h"
#include "pattern.h"

static Colour
    kAlmond { 239.0 / 255, 222.0 / 255, 205.0 / 255 },
    kBlanchedAlmond { 1.0 ,235.0 / 255, 205.0 / 255 },
    kRed { 1.0, 0, 0 },
    kBlue { 0, 0, 1.0 },
    kGreen { 0, 1.0, 0 }
;

static SpeckledPattern kSpeckledPtn { kAlmond };

static double kHorizonDepth { 70 };

Light WorldLight(double scale) {
    double scaled = 10 * scale;
    Point origin { -5 * scaled, 4 * scaled, -2 * scaled };
    Colour colour { 1, 1, 0.8 };
    return Light { origin, colour };
}

Matrix CameraTransform(double scale) {
    // Point from { 1.5*scale, 0.5*scale, -7 * scale }, to { 0, scale, 0 };
    Point from { 0.25 * scale, 2.5 * scale, -5 * scale }, to { 0, scale, 0 };
    Vector up { 0, 1, 0 };
    return ViewTransform { from, to, up };
}

Plane Horizon(double scale) {
    Plane horizon {};
    Material material {};
    material.Surface(Colour(0.2, 0.6, 0.8));
    material.Specular(0);
    horizon.SetMaterial(material);
    horizon.SetTransform(Transformation()
        .RotateX(M_PI/2)
        .Translate(0, 0, scale * kHorizonDepth)
    );
    return horizon;
}

Material SandMaterial() {
    kSpeckledPtn.SetDarkThreshold(0.8);
    kSpeckledPtn.SetAttentuation(0.3);

    Material material {};
    material.Diffuse(1.0);
    material.Specular(0);
    material.Shininess(10);
    material.Reflectivity(0);
    material.SurfacePattern(&kSpeckledPtn);
    return material;
}

Plane Sand() {
    Plane sand {};
    sand.SetMaterial(SandMaterial());
    return sand;
}

Material WaterMaterial() {
    Material material {};
    material.Transparency(1.0);
    material.RefractiveIndex(1.0 + 1.0/3);
    material.Diffuse(0.1);
    material.Ambient(0.1);
    material.Reflectivity(0.9);
    material.Specular(1);
    material.Surface(Colour { 0.0, 0.5, 1 });
    material.CastsShadow(false);
    return material;
}

int main(int argc, char** argv) {
    double scale = GetScale(argc, argv);
    int width { 108 },
        height { 135 };

    World world {};

    Light light = WorldLight(scale);
    world.Add(&light);

    Light moon { Point(10*scale, 4*scale, scale * (kHorizonDepth-5e-1)),
        Colour(0.3, 0.3, 0.3) };
    world.Add(&moon);

    Plane sand = Sand();
    world.Add(&sand);

    Plane horizon = Horizon(scale);
    world.Add(&horizon);

    PassThroughFunction sigmoid_fn = [] (const Point& p)->bool {
        return floating_point_compare(p.X(), 1 / (1 + std::exp(-p.Z())), 1e-1);
    };

    PorousSheet river = PorousSheet(sigmoid_fn, 5, 150);
    river.SetTransform(
        Transformation()
        .Translate(0, 1e-6, 0)
        .Scale(5*scale, 1, scale)
    );
    river.SetMaterial(WaterMaterial());
    world.Add(&river);

    PolyhedronFactory factory {};

    Polyhedron large_pyramid = factory.Pyramid();
    large_pyramid->SetTransform(
        Transformation()
        .RotateY(M_PI/12)
        .Translate(-2.5, 0, 4)
        .Scale(scale*3)
    );
    StripePattern pyramid_ptn { kAlmond, kBlanchedAlmond };
    pyramid_ptn.SetTransform(
        Transformation()
        .RotateZ(k90DegreesRad)
        .Scale(scale/15)
    );
    Material pyramid_m = SandMaterial();
    pyramid_m.SurfacePattern(&pyramid_ptn);
    large_pyramid->SetMaterial(pyramid_m);
    world.Add(large_pyramid);

    Polyhedron medium_pyramid = factory.Pyramid();
    medium_pyramid->SetTransform(
        Transformation()
        .RotateY(M_PI/8)
        .Translate(-2.5, 0, 3.5)
        .Scale(scale)
    );
    medium_pyramid->SetMaterial(pyramid_m);
    world.Add(medium_pyramid);

    Polyhedron small_pyramid = factory.Pyramid();
    small_pyramid->SetTransform(
        Transformation()
        .Translate(0, 0, 7)
        .Scale(scale)
    );
    small_pyramid->SetMaterial(pyramid_m);
    world.Add(small_pyramid);

    Polyhedron flare_1 = factory.Pentagon();
    Transformation flare_1_transform { Transformation().RotateX(-k90DegreesRad) },
        flare_2_transform = flare_1_transform, flare_3_transform = flare_1_transform;
    flare_1_transform
        .Translate(-0.5, 0.85, -2)
        .Scale(scale/2)
    ;
    flare_1->SetTransform(flare_1_transform);
    flare_1->SetMaterial(
        Material()
        .Transparency(1)
        .Surface(kRed)
        .CastsShadow(false)
    );
    world.Add(flare_1);

    Polyhedron flare_2 = factory.Pentagon();
    flare_2_transform.Translate(-1, 0.95, -3).Scale(scale/2);
    Material p2_m = flare_1->ShapeMaterial();
    p2_m.Surface(kGreen);
    flare_2->SetTransform(flare_2_transform);
    flare_2->SetMaterial(p2_m);
    world.Add(flare_2);

    Polyhedron flare_3 = factory.Pentagon();
    flare_3_transform.Translate(-0.2, 0.75, -1).Scale(scale/2);
    Material p3_m = flare_1->ShapeMaterial();
    p3_m.Surface(kBlue);
    flare_3->SetTransform(flare_3_transform);
    flare_3->SetMaterial(p3_m);
    world.Add(flare_3);

    Camera camera = SceneCamera(scale, width, height, k60DegreesRad, CameraTransform(scale));
    Canvas canvas = camera.RenderConcurrent(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;

    return 0;
}