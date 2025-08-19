/*
g++ -I include/ -o build/chapter-14-groups src/utils.cc src/tuple.cc src/colour.cc \
src/space.cc src/matrix.cc src/transformations.cc src/bounds.cc src/shape.cc \
src/pattern.cc src/material.cc src/world.cc src/camera.cc src/canvas.cc \
src/sheet.cc src/sphere.cc src/group.cc src/disc.cc scripts/challenges/chapter-14-groups.cc
*/

#define _USE_MATH_DEFINES // for M_PI

#include <cmath>
#include <iostream>
#include <vector>

#include "space.h"
#include "colour.h"
#include "material.h"
#include "shape.h"
#include "world.h"
// #include "plane.h"
#include "camera.h"
#include "canvas.h"
#include "sphere.h"
#include "pattern.h"
#include "sphere.h"
#include "group.h"
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
            Material m = Material()
                .Transparency(1.0)
                .RefractiveIndex(1.52)
                .Diffuse(0.9)
                .Ambient(0.1)
                .Reflectivity(1.0)
                .Shininess(300)
                .Specular(1)
                .Surface(colour);
            if (pattern_ != nullptr) {
                m.SurfacePattern(pattern_);
            }
            return m;
        }
        
        const Material AirBubbleMaterial() const {
            Material m = Material()
                .Transparency(1.0)
                .RefractiveIndex(1.00029)
                .Diffuse(0)
                .Ambient(0)
                .Specular(0)
                .Reflectivity(0)
                .Shininess(10)
                .CastsShadow(false);
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
    light.CastsShadow(false);
    world.Add(&light);

    // Plane floor {};
    // Sheet floor {};
    Disc earth {};
    world.Add(&earth);
    
    // StripePattern p1 { Colour { 1, 1, 1 }, Colour { 1, 0, 0 } },
    // p2 { p1.A(), p1.B() };
    // p1.SetTransform(Transformation().Scale(0.05*scale, scale, scale).RotateY(M_PI / 4));
    // p2.SetTransform(Transformation().Scale(0.05*scale, scale, scale).RotateY(-M_PI / 4));
    // AveragePatternBlender blender {};
    // BlendedPattern bp { &blender };
    // bp.Add(&p1).Add(&p2);
    Material earth_m {};
    earth_m.Specular(0);
    earth_m.CastsShadow(false);
    SpeckledPattern sp { Colour { 93.0/255, 54.0/255, 7.0/255 } };
    sp.SetDarkThreshold(0.5);
    sp.SetLightThreshold(0.9);
    sp.SetAttentuation(0.5);
    // PerturbedPattern pp { &sp };
    earth_m.SurfacePattern(&sp);
    earth.SetMaterial(earth_m);
    earth.SetTransform(Transformation().Scale(30*scale, 1, 30*scale).Translate(-15*scale, 0, 0));//.Translate(0, 0, 0*scale)); //.RotateY(-M_PI/6)


    Sheet sky {};
    sky.SetTransform(
        Transformation()
        .Scale(50*scale)
        .RotateX(M_PI/2)
        .Translate(0, 0, 20*scale)
    );
    Material sky_m = Material()
        .Surface(Colour { 0, 0.8, 1.0 })
        .Specular(2);
        // .Transparency(1.0)
        // .RefractiveIndex(1)
        // .Diffuse(0.5);
        // .Reflectivity(1)
        // .Shininess(300);
    sky.SetMaterial(sky_m);
    world.Add(&sky);

    Light sun { Point { 8*scale, 8*scale, 19*scale }, Colour { 0.9, 0.5, 0.5 } };
    world.Add(&sun);

    RadialGradientPattern pattern4 { Colour { 0.2, 1, 0 }, Colour { 0.2, 0, 1 } };
    pattern4.SetTransform(Transformation().Scale(0.25, 1, 1));
    PerturbedPattern p4 { &pattern4 };


    MarbleFactory marble_factory { &p4 };
    
    double marble_1_scale { 3 * scale };
    Marble marble_1 = marble_factory.Generate(Colour { 1.0, 0.0, 0.0 }, marble_1_scale);
    marble_1->SetTransform(Transformation().Translate(-2*scale, marble_1_scale, 3*scale));
    world.Add(marble_1);
    
    double marble_2_scale { scale };
    Marble marble_2 = marble_factory.Generate(Colour { 1.0, 0.0, 1.0 }, marble_2_scale);
    marble_2->SetTransform(Transformation().Translate(2*scale, marble_2_scale, 0));
    world.Add(marble_2);

    Camera camera = SceneCamera(scale, 108, 135, M_PI / 3);

    Canvas canvas = camera.Render(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;
 
    return 0;
}
