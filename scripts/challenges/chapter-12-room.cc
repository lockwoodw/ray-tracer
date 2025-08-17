/*
The Ray Tracer Challenge: Chapter 12

Render a scene of a room with a table and other objects (pp. 175-6).

Supply a scaling factor at the command line to increase the image dimensions.
*/

#define _USE_MATH_DEFINES // for M_PI

#include <cmath>
#include <vector>
#include <array>

#include "challenges.h"
#include "world.h"
#include "camera.h"
#include "canvas.h"
#include "pattern.h"
#include "cube.h"
#include "sphere.h"
#include "plane.h"

Matrix CameraTransform(double scale) {
    Point from { 7*scale, 6*scale, -7*scale }, to { scale, 3*scale, 0 };
    Vector up { 0, 1, 0 };
    return ViewTransform { from, to, up };
}

int main(int argc, char** argv) {
    double scale = GetScale(argc, argv);
    int scale_int = static_cast<int>(scale);

    World world {};

    double room_width = 15 * scale,
    room_height = 12 * scale;

    // The walls, floor/ceiling are two overlapping cubes; the walls cube is
    // taller than the floor/ceiling cube, so its top and bottom sides are not
    // visible; similarly, the floor/ceiling cubes vertical sides are wider than
    // the walls cube. This is so we can style the walls, floor, etc. independently.

    Cube walls {};
    world.Add(&walls);

    walls.SetTransform(
        Transformation()
        .Scale(room_width, room_height + 1, room_width)
        .Translate(0, room_height, 0)
    );

    Material wall_material {};
    Colour light_cyan { 224.0 / 255, 1.0, 1.0 };
    wall_material.Surface(light_cyan);
    walls.SetMaterial(wall_material);

    Cube horizontal_partitions {};
    world.Add(&horizontal_partitions);

    horizontal_partitions.SetTransform(
        Transformation()
        .Scale(room_width + scale, room_height, room_width + scale)
        .Translate(0, room_height, 0)
    );

    Material floor_material {};
    CheckerPattern floor_pattern { Colour::kBlack, Colour::kWhite };
    floor_pattern.SetTransform(Transformation()
        .Scale(0.1)
        .Translate(scale, 0, 0)
    );
    floor_material.SurfacePattern(&floor_pattern);
    horizontal_partitions.SetMaterial(floor_material);

    Cube tv {};
    world.Add(&tv);

    double tv_elevation { 5 * scale },
           tv_height { 1.5 * scale },
           tv_width { 3 * scale },
           tv_z { 5 * scale };

    tv.SetTransform(
        Transformation()
        .Scale(tv_width, tv_height, tv_width)
        .Translate(-(tv_width + room_width) + 1.5, tv_height + tv_elevation, tv_z)
    );

    tv.SetMaterial(GlassMarbleMaterial(Colour::kWhite));

    Light led_light_1 {
        Point { -room_width+0.5, tv_height + tv_elevation + tv_height/2, tv_z + tv_width/2},
        Colour { 0, 0.5, 1.0 }
    };
    world.Add(&led_light_1);

    Light led_light_2 {
        Point { -room_width+0.5, tv_height + tv_elevation -0.5*scale, tv_z-tv_width/2 },
        Colour { 1.0, 0, 0 }
    };
    world.Add(&led_light_2);

    Light overhead_light {
        Point { 6*scale, room_height - 3*scale, -8*scale },
        Colour::kWhite
    };
    world.Add(&overhead_light);

    double table_width { 2.4 * scale },
           table_length { 4 * scale },
           leg_width { 0.125 * scale },
           leg_height { 2 * scale },
           double_leg_width { leg_width * 2 },
           leg_center_x { table_length - double_leg_width },
           leg_center_z { table_width - double_leg_width };

    Cube table_top {};
    world.Add(&table_top);

    Colour pale_brown { 152.0 / 255, 118.0 / 255, 84.0 / 255 },
           wood { 193.0 / 255, 154.0 / 255, 107.0 / 255 };
    StripePattern stripe_pattern { pale_brown, wood };
    stripe_pattern.SetTransform(Transformation().RotateY(-M_PI / 6).Scale(0.09));
    PerturbedPattern perturbed_pattern = PerturbedPattern { &stripe_pattern };
    Material table_top_m {};
    table_top_m.SurfacePattern(&perturbed_pattern);
    table_top.SetMaterial(table_top_m);

    table_top.SetTransform(
        Transformation()
        .Scale(table_length, leg_width, table_width)
        .Translate(0, leg_width + 2*leg_height, 0)
    );

    std::vector<std::array<double, 2>> corners {
        {{ -1, -1 }}, {{ 1, -1 }}, {{ 1, 1 }}, {{ -1, 1 }}
    };
    std::vector<Cube*> legs {};
    Material leg_material {};
    Colour blanched_almond { 1.0, 235.0 / 255, 205.0 / 255 };
    leg_material.Surface(blanched_almond);
    Transformation leg_scale = Transformation().Scale(leg_width, leg_height, leg_width);
    for (auto corner: corners) {
        Cube* leg = new Cube();
        auto [ x, z ] = corner;
        leg->SetTransform(Transformation(leg_scale)
            .Translate(x * leg_center_x, leg_height, z * leg_center_z));
        leg->SetMaterial(leg_material);
        legs.push_back(leg);
        world.Add(leg);
    }

    double bulb_scale { 0.5 * scale },
    bulb_1_x { leg_center_x + 1.5*bulb_scale },
    bulb_1_y { 2*leg_height + leg_width },
    bulb_1_z { -leg_center_z/2 };

    Sphere bulb_1 {};
    world.Add(&bulb_1);

    Transformation bulb_1_translation = Transformation().Translate(bulb_1_x, bulb_1_y, bulb_1_z);
    bulb_1.SetTransform(Transformation().Scale(bulb_scale).Apply(bulb_1_translation));
    Colour bulb_1_colour { 0.9, 0.05, 0.05 };
    bulb_1.SetMaterial(GlassMarbleMaterial(bulb_1_colour));

    double bubble_scale { 0.98 * bulb_scale };

    Sphere air_bubble_1 {};
    world.Add(&air_bubble_1);

    air_bubble_1.SetTransform(Transformation().Scale(bubble_scale).Apply(bulb_1_translation));
    air_bubble_1.SetMaterial(AirBubbleMaterial());

    double bulb_2_x { leg_center_x - bulb_scale },
    bulb_2_y { 2*leg_height + 2*leg_width + bulb_scale },
    bulb_2_z { -leg_center_z/2 };

    Sphere bulb_2 {};
    world.Add(&bulb_2);

    Transformation bulb_2_translation = Transformation().Translate(bulb_2_x, bulb_2_y, bulb_2_z);
    bulb_2.SetTransform(Transformation().Scale(bulb_scale).Apply(bulb_2_translation));
    Colour bulb_2_colour { 0.05, 0.05, 0.9 };
    bulb_2.SetMaterial(GlassMarbleMaterial(bulb_2_colour));

    Sphere air_bubble_2 {};
    world.Add(&air_bubble_2);

    air_bubble_2.SetTransform(Transformation().Scale(bubble_scale).Apply(bulb_2_translation));
    air_bubble_2.SetMaterial(AirBubbleMaterial());

    double bulb_3_x { leg_center_x + 2.5*bulb_scale },
           bulb_3_y { leg_height + bulb_scale },
           bulb_3_z { -leg_center_z/2 };

    Sphere bulb3 {};
    world.Add(&bulb3);

    Transformation bulb_3_translation = Transformation().Translate(bulb_3_x, bulb_3_y, bulb_3_z);
    bulb3.SetTransform(Transformation().Scale(bulb_scale).Apply(bulb_3_translation));
    Colour bulb_3_colour { 0.8, 0.6, 0.05 };
    bulb3.SetMaterial(GlassMarbleMaterial(bulb_3_colour));

    Sphere air_bubble_3 {};
    air_bubble_3.SetTransform(Transformation().Scale(bubble_scale).Apply(bulb_3_translation));
    air_bubble_3.SetMaterial(AirBubbleMaterial());
    world.Add(&air_bubble_3);

    Camera camera { 108 * scale_int, 135 * scale_int, M_PI / 3 };
    camera.SetTransform(CameraTransform(scale));

    Canvas canvas = camera.Render(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;

    for (auto leg: legs) {
        delete leg;
    }

    return 0;
}
