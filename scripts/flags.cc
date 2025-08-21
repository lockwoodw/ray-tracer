#include <vector>

#include "scripts.h"
#include "plane.h"
#include "pattern.h"
#include "sheet.h"
#include "group.h"

Light WorldLight(double scale) {
    Point origin { 0, 2*scale, -3*scale};
    Colour colour { 1, 1, 1 };
    return Light { origin, colour };
}

Matrix CameraTransform(double scale) {
    Point from { -scale, 5*scale, -8 * scale }, to { 0, 4*scale, 0 };
    Vector up { 0, 1, 0 };
    return ViewTransform { from, to, up };
}

int main(int argc, char** argv) {
    double scale = GetScale(argc, argv);

    double sheet_scale { 3 * scale },
           sheet_y { sheet_scale / 2 },
           _90_degree_rad = M_PI / 2;

    World world {};
    ShapeGroup shapes {};
    world.Add(&shapes);

    Light light = WorldLight(scale);
    world.Add(&light);

    Sheet horizon {};
    shapes.Add(&horizon);

    horizon.SetTransform(
        Transformation()
        .Scale(scale * 60, 1, scale * 60)
        .RotateX(_90_degree_rad)
        .Translate(scale * 5, 0, 40 * scale)
    );

    SpeckledPattern speckled_ptn { Colour { 1.0, 174.0 / 255, 66.0 / 255 }};
    speckled_ptn.SetDarkThreshold(0.8);
    speckled_ptn.SetLightThreshold(1.0);
    speckled_ptn.SetAttentuation(0.5);

    horizon.SetMaterial(Material().SurfacePattern(&speckled_ptn));

    Sheet sky {};
    shapes.Add(&sky);

    sky.SetTransform(
        Transformation()
        .Scale(scale * 50, 1, scale * 50)
        .Translate(0, 5.1 * scale, 0)
        .RotateZ(M_PI / 12)
    );

    sky.SetMaterial(GlassMaterial());

    std::vector<Shape*> objects {};

    Material flag_mat = Material()
        .Surface(Colour { 1.0, 0, 0 })
        .Specular(0)
        .CastsShadow(false);

    for (int z_offset = 8; z_offset >= -1; z_offset--)
    {
        ShapeGroup* row = new ShapeGroup();
        objects.push_back(row);
        shapes.Add(row);

        for (double i = -1; i <= 1; i += 0.5) {
            Sheet* flag = new Sheet();
            flag->SetMaterial(flag_mat);
            flag->SetTransform(
                Transformation()
                .Scale(sheet_scale)
                .RotateX(_90_degree_rad)
                .RotateY(i * _90_degree_rad)
                .RotateZ((1 + i) * M_PI / 12)
                .Translate(i * (sheet_scale + scale), sheet_y, (z_offset * 5 - i) * scale)
            );
            objects.push_back(flag);
            row->Add(flag);
        }
    }
    shapes.Divide(100);

    Camera camera = SceneCamera(scale, 108, 135, M_PI / 3, CameraTransform(scale));
    Canvas canvas = camera.RenderConcurrent(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;

    for (auto o: objects) {
        delete o;
    }

    return 0;
}