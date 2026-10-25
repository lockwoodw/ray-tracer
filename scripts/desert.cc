#include "scripts.h"
#include "plane.h"
#include "porous-sheet.h"
#include "polyhedron.h"
#include "pattern.h"

static Colour
    kAlmond { 239.0 / 255, 222.0 / 255, 205.0 / 255 },
    kBlanchedAlmond { 1.0 ,235.0 / 255, 205.0 / 255 },
    kPaleBrown { 152.0 / 255, 118.0 / 255, 84.0 / 255 },
    kRed { 1.0, 0, 0 },
    kDarkRed { 139.0 / 255, 0, 0 },
    kFernGreen { 79.0 / 255, 121.0 / 255, 66.0 / 255 },
    kNeonGreen { 57.0 / 255, 1.0, 20.0 / 255 }
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

    Light moon { Point(10*scale, 4*scale, scale * (kHorizonDepth-5e-1)), Colour(0.3, 0.3, 0.3) };
    world.Add(&moon);

    Plane sand = Sand();
    world.Add(&sand);

    Plane horizon = Horizon(scale);
    world.Add(&horizon);

    PassThroughFunction sigmoid_fn = [] (const Point& p)->bool {
        return floating_point_compare(p.X(), 1 / (1 + std::exp(-p.Z())), 1e-1);
    };

    PorousSheet sigmoid = PorousSheet(sigmoid_fn, 5, 150);
    sigmoid.SetTransform(
        Transformation()
        .Translate(0, 1e-6, 0)
        .Scale(5*scale, 1, scale)
    );
    sigmoid.SetMaterial(WaterMaterial());
    world.Add(&sigmoid);

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

    Polyhedron pentagon = factory.Pentagon();
    Transformation pentagon_transform { Transformation().RotateX(-k90DegreesRad) },
        p2_transform = pentagon_transform, p3_transform = pentagon_transform;
    pentagon_transform
        .Translate(-0.5, 0.85, -2)
        .Scale(scale/2)
    ;
    pentagon->SetTransform(pentagon_transform);
    pentagon->SetMaterial(
        Material()
        .Transparency(1)
        .Surface(Colour(1, 0, 0))
        .CastsShadow(false)
    );
    world.Add(pentagon);

    Polyhedron p2 = factory.Pentagon();
    p2_transform.Translate(-1, 0.95, -3).Scale(scale/2);
    Material p2_m = pentagon->ShapeMaterial();
    p2_m.Surface(Colour(0, 1, 0));
    p2->SetTransform(p2_transform);
    p2->SetMaterial(p2_m);
    world.Add(p2);

    Polyhedron p3 = factory.Pentagon();
    p3_transform.Translate(-0.2, 0.75, -1).Scale(scale/2);
    Material p3_m = pentagon->ShapeMaterial();
    p3_m.Surface(Colour(0, 0, 1));
    p3->SetTransform(p3_transform);
    p3->SetMaterial(p3_m);
    world.Add(p3);

    Camera camera = SceneCamera(scale, width, height, k60DegreesRad, CameraTransform(scale));
    Canvas canvas = camera.RenderConcurrent(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;

    return 0;
}