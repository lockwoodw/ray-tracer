#include "scripts.h"

#include <vector>
#include <random>
#include <ctime>

#include "porous-sheet.h"
#include "plane.h"
#include "group.h"
#include "pattern.h"

std::mt19937_64 mt_engine_ { static_cast<long unsigned int>(time(nullptr)) };
std::uniform_real_distribution<double> urd_ { 0.0, 1.0 };

Matrix CameraTransform(double scale) {
    Point from { 7 * scale, 2 * scale, 6 * scale }, to { 0, 0.5 * scale, -6 * scale };
    Vector up { 0, 1, 0 };
    return ViewTransform { from, to, up };
}

int main(int argc, char** argv) {
    double scale = GetScale(argc, argv);

    World world {};

    Light light { Point { 0, 0, 1000 * scale }, Colour::kWhite };
    world.Add(&light);

    Transformation vertical = Transformation().RotateX(M_PI/ 2);

    Plane horizon {};
    world.Add(&horizon);
    horizon.SetTransform(Transformation().RotateZ(-M_PI/3).Translate(0, -scale, 0));
    horizon.SetMaterial(Material().Surface(Colour::kWhite).Reflectivity(0.5));

    std::vector<Colour> colours = {
        Colour { 1, 0, 0 },
        Colour { 1, 0.5, 0 },
        Colour { 1, 1, 0 },
        Colour { 0.5, 1, 0 },
        Colour { 0, 1, 0 },
        Colour { 0, 1, 0.5 },
        Colour { 0, 1, 1 },
        Colour { 0, 0, 1 },
        Colour { 0, 0, 1 },
        Colour { 0.5, 0, 1 },
        Colour { 1, 0, 0.5 }
    };

    std::vector<Shape*> shapes {};
    std::vector<Pattern*> patterns {};
    ShapeGroup sheets {};
    world.Add(&sheets);

    PassThroughFunction pass_fn = [] (const Point& p)->bool {
        return floating_point_compare(std::fabs(p.X()), p.Z() * p.Z() * 3, 1e-2)
            || urd_(mt_engine_) < 0.005;
    };

    size_t n_sheets { colours.size() };
    double sheet_scale { (n_sheets - 1) * scale },
           sheet_rotation { M_PI / 12 },
           pattern_rotation { M_PI / 8 };

    for (size_t i = 0; i < n_sheets; i++) {
        PorousSheet* sheet = new PorousSheet(pass_fn);
        shapes.push_back(sheet);
        sheets.Add(sheet);

        sheet->SetTransform(
            Transformation(vertical)
            .RotateZ(i*sheet_rotation)
            .Scale(1.5*sheet_scale)
            .Translate(0, 0, -(1.0 / n_sheets) * i * sheet_scale));

        GradientPattern* ptn = new GradientPattern(colours[i % n_sheets],
            Colour { 0.1, 0.1, 0.1 });
        ptn->SetTransform(Transformation().RotateY(pattern_rotation));
        patterns.push_back(ptn);

        sheet->SetMaterial(
            Material()
            .SurfacePattern(ptn)
            .CastsShadow(false)
            .Shininess(300)
            .Specular(2)
        );
    }

    Camera camera = SceneCamera(scale, 108, 135, M_PI / 2, CameraTransform(scale));
    Canvas canvas = camera.RenderConcurrent(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;

    for (auto s: shapes) {
        delete s;
    }

    for (auto p: patterns) {
        delete p;
    }

    return 0;
}