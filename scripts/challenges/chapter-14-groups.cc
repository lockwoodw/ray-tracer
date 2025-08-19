/*
The Ray Tracer Challenge: Chapter 14

Use shape groups to define and render a scene.

Supply a scaling factor at the command line to increase the image dimensions.
*/

#include <vector>

#include "challenges.h"
#include "sphere.h"
#include "pattern.h"
#include "group.h"
#include "sheet.h"
#include "disc.h"

Light WorldLight(double scale) {
    Point origin { 4*scale, 10 * scale, -4*scale };
    Colour colour { 0.9, 0.3, 0.3 };
    return Light { origin, colour };
}

using Marble = ShapeGroup*;
class MarbleFactory {
    std::vector<Shape *> shapes_;
    Pattern* pattern_;

    public:
        MarbleFactory(): shapes_ {}, pattern_ { nullptr } {}
        MarbleFactory(Pattern* ptn): shapes_ {}, pattern_ { ptn } {}

        ~MarbleFactory() {
            for (auto m: shapes_) {
                delete m;
            }
        }

        const Material GlassMarbleMaterial(const Colour& colour) const {
            Material m = GlassMaterial(colour);
            m.Diffuse(0.9);
            if (pattern_ != nullptr) {
                m.SurfacePattern(pattern_);
            }
            return m;
        }

        Marble Generate(const Colour& colour, double scale, double bubble_ratio=0.98) {
            Marble marble = new ShapeGroup();
            shapes_.push_back(marble);

            Sphere* outside = new Sphere();
            shapes_.push_back(outside);
            outside->SetTransform(Transformation().Scale(scale));
            outside->SetMaterial(GlassMarbleMaterial(colour));
            *marble << outside;

            if (bubble_ratio > 0.0 && bubble_ratio < 1.0) {
                Sphere* inside = new Sphere();
                shapes_.push_back(inside);
                inside->SetTransform(Transformation().Scale(scale * bubble_ratio));
                inside->SetMaterial(AirBubbleMaterial());
                *marble << inside;
            }

            return marble;
        }
};

Matrix CameraTransform(double scale) {
    Point from { scale, 4*scale, -9 * scale }, to { 0, 4*scale, 0 };
    Vector up { 0, 1, 0 };
    return ViewTransform { from, to, up };
}

Material GroundMaterial() {
    static SpeckledPattern speckled_ptn { Colour { 93.0/255, 54.0/255, 7.0/255 } };
    speckled_ptn.SetDarkThreshold(0.5);
    speckled_ptn.SetLightThreshold(0.9);
    speckled_ptn.SetAttentuation(0.5);
    return Material()
        .Specular(0)
        .CastsShadow(false)
        .SurfacePattern(&speckled_ptn);
}

Pattern* MarblePattern() {
    static RadialGradientPattern gradient_ptn { Colour { 0.2, 1, 0 }, Colour { 0.2, 0, 1 } };
    gradient_ptn.SetTransform(Transformation().Scale(0.25, 1, 1));
    static PerturbedPattern perturbed_ptn { &gradient_ptn };
    return &perturbed_ptn;
}

int main(int argc, char** argv) {
    double scale = GetScale(argc, argv);

    World world {};

    Light light = WorldLight(scale);
    light.CastsShadow(false);
    world.Add(&light);

    Disc earth {};
    world.Add(&earth);

    earth.SetTransform(
        Transformation()
        .Scale(30*scale, 1, 30*scale)
        .Translate(-15*scale, 0, 0)
    );

    earth.SetMaterial(GroundMaterial());

    Sheet sky {};
    world.Add(&sky);

    sky.SetTransform(
        Transformation()
        .Scale(50*scale)
        .RotateX(M_PI/2)
        .Translate(0, 0, 20*scale)
    );

    sky.SetMaterial(
        Material()
        .Surface(Colour { 0, 0.8, 1.0 })
        .Specular(2)
    );

    Light star { Point { 8*scale, 8*scale, 19*scale }, Colour { 0.9, 0.5, 0.5 } };
    world.Add(&star);

    MarbleFactory marble_factory { MarblePattern() };

    double marble_1_scale { 3 * scale };
    Marble marble_1 = marble_factory.Generate(Colour { 1.0, 0.0, 0.0 }, marble_1_scale);
    marble_1->SetTransform(Transformation().Translate(-2*scale, marble_1_scale, 3*scale));
    world.Add(marble_1);

    double marble_2_scale { scale };
    Marble marble_2 = marble_factory.Generate(Colour { 1.0, 0.0, 1.0 }, marble_2_scale);
    marble_2->SetTransform(Transformation().Translate(2*scale, marble_2_scale, 0));
    world.Add(marble_2);

    Camera camera = SceneCamera(scale, 108, 135, M_PI / 3, CameraTransform(scale));
    Canvas canvas = camera.Render(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;

    return 0;
}
