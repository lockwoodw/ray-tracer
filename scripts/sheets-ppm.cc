/*
g++ -I include/ -o build/sheets-ppm src/utils.cc src/tuple.cc src/colour.cc \
src/space.cc src/matrix.cc src/transformations.cc src/shape.cc src/pattern.cc \
src/material.cc src/world.cc src/camera.cc src/canvas.cc src/plane.cc \
src/sheet.cc scripts/sheets-ppm.cc
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
#include "sheet.h"

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
    Point origin { 0, 2*scale, -scale};
    Colour colour { 1, 1, 1 };
    return Light { origin, colour };
}

Matrix CameraTransform(double scale) {
    Point from { -scale, 5*scale, -8 * scale }, to { 0, 4*scale, 0 };
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
    m.CastsShadow(false);
    return m;
}

int main(int argc, char** argv) {
    double scale = GetScale(argc, argv);
    int scale_int = static_cast<int>(scale);

    double sheet_scale { 3 * scale },
           sheet_y { sheet_scale / 2 },
           _90_degree_rad = M_PI / 2;

    World world {};

    Light light = WorldLight(scale);
    world.Add(&light);

    Plane horizon {};
    horizon.SetTransform(
        Transformation()
        .RotateX(_90_degree_rad)
        .Translate(0, 0, 40*scale)
    );
    SpeckledPattern sp { Colour { 1.0, 174.0/255, 66.0/255 }};
    sp.SetDarkThreshold(0.8);
    sp.SetLightThreshold(1.0);
    sp.SetAttentuation(0.2);
    Material horizon_material {};
    horizon_material.SurfacePattern(&sp);
    // horizon_material.Reflectivity(1.0);
    horizon.SetMaterial(horizon_material);
    world.Add(&horizon);

    Plane boundary {};
    boundary.SetTransform(
        Transformation()
        .Translate(0, 5.5*scale, 0)
        .RotateZ(M_PI/12)
    );
    boundary.SetMaterial(GlassMaterial());
    world.Add(&boundary);

    std::vector<Sheet*> sheets {};

    Material sheet_material {};
    sheet_material.Surface(Colour { 1.0, 0, 0 });
    sheet_material.Specular(0);
    sheet_material.CastsShadow(false);

    for (int z_offset = 6; z_offset >= -1; z_offset--)
    {
        for (double i = -1; i <= 1; i += 0.5) {
            Sheet* sheet = new Sheet();
            sheet->SetMaterial(sheet_material);
            sheet->SetTransform(
                Transformation()
                .Scale(sheet_scale)
                .RotateX(_90_degree_rad)
                .RotateY(i*_90_degree_rad)
                .RotateZ((1 + i)*M_PI/12)
                .Translate(i*(sheet_scale + scale), sheet_y, (z_offset*5 -i)*scale)
            );
            sheets.push_back(sheet);
            world.Add(sheet);
        }
    }

    Camera camera { 108 * scale_int, 135 * scale_int, M_PI / 3 };
    camera.SetTransform(CameraTransform(scale));

    Canvas canvas = camera.Render(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;

    for (auto sheet: sheets) {
        delete sheet;
    }

    return 0;
}