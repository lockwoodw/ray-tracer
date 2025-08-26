/*
The Ray Tracer Challenge: Chapter 13

Render a scene using cones.

Supply a scaling factor at the command line to increase the image dimensions.
*/

#include <vector>
#include <random>
#include <ctime>
#include <set>

#include "challenges.h"
#include "plane.h"
#include "cone.h"
#include "sheet.h"
#include "cylinder.h"
#include "group.h"
#include "sphere.h"
#include "hemisphere.h"
#include "pattern.h"

static std::mt19937_64 mt_engine { static_cast<long unsigned int>(time(nullptr)) };

using Pencil = ShapeGroup*;

class PencilFactory {
    std::vector<Shape *> objects_;
    static SpeckledPattern kWoodPattern;

    double Circumradius(double scale) {
        return 2.2 * scale;
    }

    void AddTip(Pencil pencil, double scale, const Colour& colour) {
        ShapeGroup* tip = new ShapeGroup();
        objects_.push_back(tip);
        pencil->Add(tip);

        Cone* lead  = new Cone(0, 1, true);
        objects_.push_back(lead);
        tip->Add(lead);

        lead->SetTransform(
            Transformation()
            .Scale(scale*1.01, 3*scale, scale*1.01)
        );

        lead->SetMaterial(Material().Surface(colour));

        Cone* wood = new Cone(0, 2, true);
        objects_.push_back(wood);
        tip->Add(wood);

        wood->SetTransform(
            Transformation()
            .Scale(scale, 3*scale, scale)
        );

        wood->SetMaterial(
            Material()
            .SurfacePattern(&kWoodPattern)
            .Shininess(10)
            .Specular(0)
        );
    }

    void AddBody(Pencil pencil, double scale, const Colour& colour) {
        // Properties for hexagon:
        int n_sides { 6 };
        double base_y_radians = M_PI / 3,          // 60 degrees
               circumradius = Circumradius(scale), // radius of circle that runs through each vertex
               // calculate apothem (aka, inradius) using Pythagorean theorem
               half_circumradius = circumradius / 2,
               apothem = std::sqrt(circumradius * circumradius - half_circumradius * half_circumradius);

        // Rotate xz plane in x to create side
        Transformation base_transform = Transformation().RotateX(M_PI / 2);

        // Create a side, rotate it in y in increasing increments of 60 degrees,
        // and shift it in z
        ShapeGroup* body = new ShapeGroup();
        objects_.push_back(body);
        pencil->Add(body);

        for (int i = 0; i < 6; i++) {
            Sheet* side = new Sheet();
            objects_.push_back(side);
            body->Add(side);

            side->SetTransform(
                Transformation(base_transform)
                .Scale(circumradius, 16*circumradius, 1)
                .Translate(0, 10.8*circumradius, apothem)
                .RotateY(i * base_y_radians)
            );

            side->SetMaterial(
                Material()
                .Surface(colour)
            );
        }
    }

    void AddEnd(Pencil pencil, double scale) {
        ShapeGroup* end = new ShapeGroup();
        objects_.push_back(end);
        pencil->Add(end);

        Colour dark_goldenrod { 184.0 / 255, 134.0 / 255, 11.0 / 255 },
               gold  { 1.0, 215.0 / 255, 0};

        int n_bands { 9 };
        double circumradius = Circumradius(scale),
               band_height { circumradius / 5 },
               band_scale { circumradius * 1.05 },
               band_y_offset { 18.7*circumradius };

        for (int i = 0; i < n_bands; i++) {
            Cylinder* band = new Cylinder(0, 1, false);
            objects_.push_back(band);
            end->Add(band);

            Colour colour = i % 2 == 0 ? gold : dark_goldenrod;
            band->SetMaterial(
                Material()
                .Surface(colour)
                .Reflectivity(1)
                .Shininess(300)
                .Specular(1)
            );

            band->SetTransform(
                Transformation()
                .Scale(band_scale, band_height, band_scale)
                .Translate(0, band_y_offset + i*(band_height), 0)
            );
        }

        Hemisphere* eraser = new Hemisphere();
        objects_.push_back(eraser);
        end->Add(eraser);

        Colour new_york_pink { 215.0 / 255, 131.0 / 255, 127.0 / 255 };
        eraser->SetMaterial(
            Material()
            .Surface(new_york_pink)
            .Shininess(10)
            .Specular(0)
        );

        eraser->SetTransform(
            Transformation()
            .Scale(0.95*circumradius, band_scale, band_scale)
            .RotateZ(-M_PI/2)
            .Translate(0, band_y_offset + n_bands*band_height, 0)
        );
    }

    public:
        PencilFactory(): objects_ {} {}

        ~PencilFactory() {
            for (auto o: objects_) {
                delete o;
            }
        }

        Pencil Generate(double scale, const Colour& colour) {
            Pencil pencil = new ShapeGroup();
            objects_.push_back(pencil);

            AddTip(pencil, scale, colour);
            AddBody(pencil, scale, colour);
            AddEnd(pencil, scale);
            return pencil;
        }

};

SpeckledPattern PencilFactory::kWoodPattern = [] () -> SpeckledPattern {
    Colour bisque { 1.0, 228.0 / 255, 196.0 / 255 };
    SpeckledPattern ptn { bisque };
    ptn.SetDarkThreshold(0.8);
    ptn.SetLightThreshold(0.8);
    ptn.SetAttentuation(0.2);
    return ptn;
} ();

Light WorldLight(double scale) {
    Point origin { -0 * scale, 250 * scale, -20 * scale };
    Colour colour { 1, 1, 1 };
    return Light { origin, colour };
}

Matrix CameraTransform(double scale) {
    Point from { 20*scale, 220*scale, -70 * scale }, to { 10*scale, 170*scale, 0 };
    Vector up { 0, 1, 0 };
    return ViewTransform { from, to, up };
}

Colour GetNewColour(Colour c, double range = 0.5, double minimum = 0.2) {
    std::uniform_real_distribution<double> urd { -range, range };

    auto new_colour_pt = [minimum] (std::uniform_real_distribution<double> urd, double pt) {
        double offset = urd(mt_engine),
               new_pt = pt + offset;
        return std::max(std::min(new_pt, 1.0), minimum);
    };

    std::set<int> remaining {
        Colour::kRed, Colour::kGreen, Colour::kBlue
    };

    double  red = c.Red(),
            green = c.Green(),
            blue = c.Blue(),
            max_colour = std::max(std::max(red, green), blue);

    int max_colour_index {};

    for (auto it = remaining.begin(); it != remaining.end(); it++) {
        if (simple_floating_point_compare(c.At(*it), max_colour)) {
            max_colour_index = *it;
            remaining.erase(it);
            break;
        }
    }
    double current_pt = c.At(max_colour_index),
           new_pt = new_colour_pt(urd, current_pt);
    double diff = std::fabs(current_pt - new_pt);
    c[max_colour_index] = new_pt;
    std::uniform_real_distribution<double> local_urd { -diff, diff };
    for (auto it = remaining.begin(); it != remaining.end(); it++) {
        c[*it] = new_colour_pt(local_urd, c[*it]);
    }
    return c;
};

int main(int argc, char** argv) {
    double scale = GetScale(argc, argv);

    World world {};

    Light light = WorldLight(scale);
    world.Add(&light);

    Plane p {};
    world.Add(&p);

    Plane horizon {};
    horizon.SetTransform(
        Transformation()
        .RotateX(M_PI/2)
        .Translate(0, 0, 80*scale)
    );

    ShapeGroup pencils {};
    world.Add(&pencils);
    PencilFactory pencil_factory {};

    Transformation base_transform = Transformation().RotateZ(M_PI/2).RotateX(M_PI/6);

    int n_pencils { 20 };
    double y_offset = { 2.1 * scale };
    Colour colour { 1, 0.5, 0.5 };

    std::vector<Shape*> layers {};
    for (int y = 0; y < 50; y++) {
        ShapeGroup* layer = new ShapeGroup();
        layers.push_back(layer);
        pencils.Add(layer);

        for (int x = 0; x < 6; x++) {
            std::uniform_real_distribution<double> urd { -M_PI/30, M_PI/30 };
            double radians { M_PI/10 };
            double rotation { x * radians + urd(mt_engine) },
            x_rot { std::cos(rotation) },
            z_rot { std::sin(rotation) };

            Pencil pencil = pencil_factory.Generate(scale, colour);
            layer->Add(pencil);

            pencil->SetTransform(
                Transformation(base_transform)
                .Translate(10*x_rot*(x+urd(mt_engine))*scale, 0, 10*z_rot*x*scale)
                .RotateY(rotation)
            );

            colour = GetNewColour(colour);
        }
        layer->SetTransform(
            Transformation()
            .Translate(0, y_offset + 2*y*y_offset, 0)
        );
    }

    Camera camera = SceneCamera(scale, 108, 135, M_PI / 3, CameraTransform(scale));
    Canvas canvas = camera.RenderConcurrent(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;

    for (auto s: layers) {
        delete s;
    }

    return 0;
}