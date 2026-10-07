/*
The Ray Tracer Challenge: Chapter 14

Render a hexagon using spheres and cylinders.

Supply a scaling factor at the command line to increase the image dimensions.
*/

#include <vector>
#include <algorithm>

#include "challenges.h"
#include "sphere.h"
#include "cylinder.h"
#include "pattern.h"
#include "group.h"
#include "plane.h"

Light WorldLight(double scale) {
    Point origin { -4 * scale, 10 * scale, -4 * scale };
    Colour colour { 1, 1, 1 };
    return Light { origin, colour };
}

Matrix CameraTransform(double scale) {
    Point from { 0, 8 * scale, -20 * scale }, to { 0, scale, 0 };
    Vector up { 0, 1, 0 };
    return ViewTransform { from, to, up };
}

using Arm = ShapeGroup*;

class ArmFactory {
    std::vector<Arm> objects_;

    public:
        ArmFactory(): objects_ {} {}

        ~ArmFactory() {
            std::for_each(objects_.begin(), objects_.end(),
                [] (Arm arm) {
                    arm->Clear();
                    delete arm;
                });
        }

        Arm Generate() {
            Arm arm = new ShapeGroup();
            objects_.push_back(arm);

            double object_scale = 0.25;

            Sphere* knob = new Sphere();
            knob->SetTransform(
                Transformation()
                .Scale(object_scale)
                .Translate(0, 0, -1)
            );
            arm->Add(knob);

            Cylinder* bar = new Cylinder(0, 1, false);
            bar->SetTransform(
                Transformation()
                .Scale(object_scale, 1, object_scale)
                .RotateZ(-M_PI/2)
                .RotateY(-M_PI/6)
                .Translate(0, 0, -1)
            );
            arm->Add(bar);

            return arm;
        }
};

int main(int argc, char** argv) {
    double scale = GetScale(argc, argv);

    World world {};

    Light light = WorldLight(scale);
    world.Add(&light);

    ArmFactory factory {};

    // Properties for hexagon:
    int n_sides { 6 };
    double base_y_radians { M_PI / 3 };  // 60 degrees

    ShapeGroup hexagon {};
    world.Add(&hexagon);

    for (int i = 0; i < n_sides; i++) {
        Arm arm = factory.Generate();
        arm->SetTransform(
            Transformation()
            .RotateY(i * base_y_radians)
        );
        hexagon.Add(arm);
    }

    hexagon.SetTransform(
        Transformation()
        .Scale(scale*scale)
    );

    hexagon.SetMaterial(
        Material()
        .Surface(Colour { 1, 0, 0 })
    );

    Camera camera = SceneCamera(scale, 200, 100, M_PI / 3, CameraTransform(scale));
    Canvas canvas = camera.RenderConcurrent(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;

    return 0;
}
