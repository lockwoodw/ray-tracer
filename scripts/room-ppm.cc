/*
g++ -I include/ -o build/room-ppm src/utils.cc src/tuple.cc src/colour.cc \
src/space.cc src/matrix.cc src/transformations.cc src/shape.cc src/pattern.cc \
src/material.cc src/world.cc src/camera.cc src/canvas.cc src/sphere.cc \
src/cube.cc scripts/room-ppm.cc
*/

#define _USE_MATH_DEFINES // for M_PI

#include <cmath>
#include <iostream>
#include <vector>
#include <array>

#include "space.h"
#include "colour.h"
#include "material.h"
#include "world.h"
#include "camera.h"
#include "canvas.h"
#include "pattern.h"
#include "cube.h"
#include "sphere.h"
#include "plane.h"

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

// Light WorldLight(double scale) {
//     double scaled = 10 * scale;
//     Point origin { 2*scaled, 4.5 * scaled, -2*scaled };
//     Colour colour { 1, 1, 1 };
//     return Light { origin, colour };
// }

Matrix CameraTransform(double scale) {
    Point from { 7*scale, 6*scale, -7*scale }, to { scale, 3*scale, 0 };
    // Point from { 3*scale, 4.5*scale, -3*scale }, to { 3*scale, 4.5*scale, 0 };
    Vector up { 0, 1, 0 };
    return ViewTransform { from, to, up };
}

Material GlassMarbleMaterial(const Colour& colour) {
    Material m {};
    m.Transparency(1.0);
    m.RefractiveIndex(1.52);
    m.Diffuse(0.1);
    m.Ambient(0.1);
    m.Reflectivity(1.0);
    m.Shininess(300);
    m.Specular(1);
    m.Surface(colour);
    return m;
}

Material AirBubbleMaterial() {
    Material m {};
    m.Transparency(1.0);
    m.RefractiveIndex(1.00029);
    m.Diffuse(0);
    m.Ambient(0);
    m.Specular(0);
    m.Reflectivity(0);
    m.Shininess(10);
    m.CastsShadow(false);
    return m;
}

int main(int argc, char** argv) {
    double scale = GetScale(argc, argv);
    int scale_int = static_cast<int>(scale);

    World world {};

    // Sphere s {};
    // world.Add(&s);

    // Sphere s2 {};
    // s2.SetTransform(Transformation().Translate(2*scale, 3.5*scale, -3.5*scale));
    // world.Add(&s2);

    // Plane plane {};
    // world.Add(&plane);
    // Light light = WorldLight(scale);
    // world.Add(&light);

    // Plane floor {};
    // world.Add(&floor);

    // Sphere ball {};
    // ball.SetTransform(Transformation().Scale(scale, scale, scale));
    // world.Add(&ball);

    double room_width = 15 * scale,
           room_height = 12 * scale;

    Cube walls {};
    walls.SetTransform(
        Transformation()
        .Scale(room_width, room_height + 1, room_width)
        .Translate(0, room_height, 0)
        // .RotateY(M_PI/3)
    );
    Material wall_material {};
    Colour powder_blue { 176.0/255, 224.0/255, 230.0/255 };
    Colour light_cyan { 224.0/255, 1.0, 1.0 };
    wall_material.Surface(light_cyan);
    // StripePattern wall_pattern { powder_blue, Colour::kWhite };
    // wall_pattern.SetTransform(Transformation().Scale(0.2));
    // wall_material.SurfacePattern(&wall_pattern);
    walls.SetMaterial(wall_material);
    world.Add(&walls);

    Cube window {};
    double window_elevation { 5 * scale },
           window_height { 1.5 * scale },
           window_width { 3 * scale },
           window_z { 5 * scale };
    window.SetMaterial(GlassMarbleMaterial(Colour::kWhite));
    window.SetTransform(
        Transformation()
        .Scale(window_width, window_height, window_width)
        .Translate(-(window_width + room_width) + 1, window_height + window_elevation, window_z)
    );
    world.Add(&window);

    Light bulb_light { Point { -room_width+0.25, window_height + window_elevation + window_height/2, window_z + window_width/2}, Colour { 0, 0.5, 1.0 } };
    world.Add(&bulb_light);
    Light bulb_light_2 { Point { -room_width+0.5, window_height + window_elevation -0.5*scale, window_z-window_width/2 }, Colour { 1.0, 0, 0 } };
    world.Add(&bulb_light_2);



    Cube horizontal_partitions {};
    horizontal_partitions.SetTransform(
        Transformation()
        .Scale(room_width + scale, room_height, room_width + scale)
        .Translate(0, room_height, 0)
        // .RotateY(M_PI/3)
    );
    Material floor_material {};
    CheckerPattern floor_pattern { Colour::kBlack, Colour::kWhite };
    floor_pattern.SetTransform(Transformation()
        .Scale(0.1)
        // .RotateY(M_PI / 3)
        .Translate(scale, 0, 0)
    );
    floor_material.SurfacePattern(&floor_pattern);
    // floor_material.Reflectivity(0.2);
    horizontal_partitions.SetMaterial(floor_material);
    world.Add(&horizontal_partitions);

    Light light { Point { 6*scale, room_height - 3*scale, -8*scale }, Colour::kWhite };
    world.Add(&light);

    double table_width { 2.4 * scale },
           table_length { 4 * scale },
           leg_width { 0.125 * scale },
           leg_height { 2 * scale },
           double_leg_width { leg_width * 2 },
           leg_center_x { table_length - double_leg_width },
           leg_center_z { table_width - double_leg_width };

    Cube table_top {};
    Colour pale_brown { 152.0/255, 118.0/255, 84.0/255 },
           wood { 193.0/255, 154.0/255, 107.0/255 };
    StripePattern stripe_pattern { pale_brown, wood };
    stripe_pattern.SetTransform(Transformation().RotateY(-M_PI / 6).Scale(0.09));
    PerturbedPattern pptn = PerturbedPattern { &stripe_pattern };
    Material table_top_m {};
    // table_top_m.Surface(Colour { 1.0, 1.0, 0 });
    // table_top_m.CastsShadow(true);
    table_top_m.SurfacePattern(&pptn);
    table_top.SetMaterial(table_top_m);
    table_top.SetTransform(
        Transformation()
        .Scale(table_length, leg_width, table_width)
        .Translate(0, leg_width + 2*leg_height, 0)
    );
    world.Add(&table_top);

    std::vector<std::array<double, 2>> corners {
        {{ -1, -1 }}, {{ 1, -1 }}, {{ 1, 1 }}, {{ -1, 1 }}
    };
    std::vector<Cube*> legs {};
    Material leg_material {};
    Colour blanched_almond { 1.0, 235.0/255, 205.0/255 };
    leg_material.Surface(blanched_almond);
    // leg_material.CastsShadow(true);
    Transformation leg_scale = Transformation().Scale(leg_width, leg_height, leg_width);
    for (auto corner: corners) {
        Cube* leg = new Cube();
        auto [ x, z ] = corner;
        leg->SetTransform(Transformation(leg_scale).Translate(x * leg_center_x, leg_height, z * leg_center_z));
        leg->SetMaterial(leg_material);
        legs.push_back(leg);
        world.Add(leg);
    }

    Sphere bulb {};
    double bulb_scale { 0.5 * scale },
           bulb_x { leg_center_x + 1.5*bulb_scale },
           bulb_y { 2*leg_height + leg_width },
           bulb_z { -leg_center_z/2 };
    Transformation bulb_translation = Transformation().Translate(bulb_x, bulb_y, bulb_z);
    bulb.SetTransform(Transformation().Scale(bulb_scale).Apply(bulb_translation));
    Colour bulb_colour { 0.9, 0.05, 0.05 };
    bulb.SetMaterial(GlassMarbleMaterial(bulb_colour));
    world.Add(&bulb);

    Sphere air_bubble {};
    double bubble_scale_factor { 0.98 };
    air_bubble.SetTransform(Transformation().Scale(bubble_scale_factor * bulb_scale).Apply(bulb_translation));
    air_bubble.SetMaterial(AirBubbleMaterial());
    world.Add(&air_bubble);

    Sphere bulb2 {};
    double bulb_2_x { leg_center_x - bulb_scale },
           bulb_2_y { 2*leg_height + 2*leg_width + bulb_scale },
           bulb_2_z { -leg_center_z/2 };
    Transformation bulb_2_translation = Transformation().Translate(bulb_2_x, bulb_2_y, bulb_2_z);
    bulb2.SetTransform(Transformation().Scale(bulb_scale).Apply(bulb_2_translation));
    Colour bulb_2_colour { 0.05, 0.05, 0.9 };
    bulb2.SetMaterial(GlassMarbleMaterial(bulb_2_colour));
    world.Add(&bulb2);

    Sphere air_bubble_2 {};
    air_bubble_2.SetTransform(Transformation().Scale(bubble_scale_factor * bulb_scale).Apply(bulb_2_translation));
    air_bubble_2.SetMaterial(AirBubbleMaterial());
    world.Add(&air_bubble_2);


    Sphere bulb3 {};
    double bulb_3_x { leg_center_x + 2.5*bulb_scale },
           bulb_3_y { leg_height + bulb_scale },
           bulb_3_z { -leg_center_z/2 };
    Transformation bulb_3_translation = Transformation().Translate(bulb_3_x, bulb_3_y, bulb_3_z);
    bulb3.SetTransform(Transformation().Scale(bulb_scale).Apply(bulb_3_translation));
    Colour bulb_3_colour { 0.8, 0.6, 0.05 };
    bulb3.SetMaterial(GlassMarbleMaterial(bulb_3_colour));
    world.Add(&bulb3);

    Sphere air_bubble_3 {};
    air_bubble_3.SetTransform(Transformation().Scale(bubble_scale_factor * bulb_scale).Apply(bulb_3_translation));
    air_bubble_3.SetMaterial(AirBubbleMaterial());
    world.Add(&air_bubble_3);

    // Light bulb_light { Point { bulb_2_x, bulb_2_y, bulb_2_z }, Colour::kWhite };
    // world.Add(&bulb_light);
    // std::vector<Light*> extra_lights {};
    // for (int i = 0; i < 3; i++) {
    //     Light* light = new Light(bulb_light);
    //     extra_lights.push_back(light);
    //     world.Add(light);
    // }

    Camera camera { 108 * scale_int, 135 * scale_int, M_PI / 3 };
    camera.SetTransform(CameraTransform(scale));

    Canvas canvas = camera.Render(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;

    for (auto leg: legs) {
        delete leg;
    }

    // for (auto light: extra_lights) {
    //     delete light;
    // }

    return 0;
}
