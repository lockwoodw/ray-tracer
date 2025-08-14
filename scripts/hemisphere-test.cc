/*
g++ -I include/ -o build/hemisphere-test src/utils.cc src/tuple.cc src/colour.cc \
src/space.cc src/matrix.cc src/transformations.cc src/bounds.cc src/shape.cc \
src/pattern.cc src/material.cc src/world.cc src/camera.cc src/canvas.cc \
src/plane.cc src/group.cc src/cube.cc src/sphere.cc src/hemisphere.cc scripts/hemisphere-test.cc
*/

#define _USE_MATH_DEFINES // for M_PI

#include <cmath>
#include <iostream>
#include <vector>

#include "space.h"
#include "colour.h"
#include "material.h"
// #include "shape.h"
#include "world.h"
#include "plane.h"
#include "camera.h"
#include "canvas.h"
#include "sphere.h"
#include "pattern.h"
// #include "sphere.h"
#include "group.h"
#include "hemisphere.h"
#include "cube.h"

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
    Point origin { -5*scale, 5 * scale, -5*scale };
    Colour colour { 0.3, 0.3, 0.3 };
    return Light { origin, colour };
}

Matrix CameraTransform(double scale) {
    Point from { -0.5*scale, scale, -7* scale }, to { -0.5*scale, 0.5*scale, 0 };
    Vector up { 0, 1, 0 };
    return ViewTransform { from, to, up };
}

Camera SceneCamera(double scale, int width, int height, double fov) {
    int scale_int = static_cast<int>(scale);
    Camera camera { width * scale_int, height * scale_int, fov };
    camera.SetTransform(CameraTransform(scale));
    return camera;
}


class CupFactory {
    std::vector<Shape*> objects_;

    public:
        CupFactory() {}
        ~CupFactory() {
            for (auto o: objects_) {
                delete o;
            }
        }
        ShapeGroup* GetCup(const Transformation& t) {
            Hemisphere* cup = new Hemisphere();
            objects_.push_back(cup);
            cup->SetTransform(t);
            cup->SetMaterial(CupMaterial());
            cup->Closed(false);

            Hemisphere* bob = new Hemisphere();
            objects_.push_back(bob);
            bob->SetTransform(
                t *
                Transformation()
                .Scale(0.25)
                .Translate(-0.25, 0, 0)
                .RotateZ(-M_PI)
            );
            bob->SetMaterial(BobMaterial());

            ShapeGroup* collection = new ShapeGroup();
            objects_.push_back(collection);
            *collection << cup << bob;
            return collection;
        }

        const Material CupMaterial(const Colour& colour = Colour::kWhite) const {
            Material m = Material()
                .Transparency(1.0)
                .RefractiveIndex(2.52)
                .Diffuse(0.9)
                .Ambient(0.1)
                .Reflectivity(1.0)
                .Shininess(300)
                .Specular(1)
                .Surface(colour);
            return m;
        }

        const Material BobMaterial(const Colour& colour = Colour::kWhite) const {
            Material m = Material()
                .Reflectivity(1.0);
            return m;
        }
};

int main(int argc, char** argv) {
    double scale = GetScale(argc, argv);

    World world {};

    Light light = WorldLight(scale);
    // world.Add(&light);

    CupFactory cf {};

    Transformation cup_1_tm = Transformation()
        .Scale(scale)
        .RotateY(-M_PI/12)
        .RotateZ(-M_PI/12)
        .Translate(0, scale, 0);
    ShapeGroup* cup_1 = cf.GetCup(cup_1_tm);
    world.Add(cup_1);

    Point light_point { 0, 0, 0 };
    Transformation light_tm = Transformation().Translate(-0.5, 0, 0);

    Light cup_1_lt { light_tm * cup_1_tm * light_point, Colour { 0.3, 0.2, 0.2 } };
    world.Add(&cup_1_lt);

    // std::cerr << cup_1_tm * light_tm * light_point << '\n';

    Transformation cup_2_tm = Transformation()
        .Scale(scale)
        .RotateY(M_PI/12)
        .Translate(-2*scale, scale, -scale);
    ShapeGroup* cup_2 = cf.GetCup(cup_2_tm);
    world.Add(cup_2);

    Light cup_2_lt { light_tm * cup_2_tm * light_point, Colour { 0.1, 0.2, 0.1 } };
    world.Add(&cup_2_lt);

    Transformation cup_3_tm = Transformation()
        .Scale(scale)
        .RotateY(M_PI*165/180)
        .RotateZ(M_PI/4)
        .Translate(2*scale, scale, 1.5*scale);
    ShapeGroup* cup_3 = cf.GetCup(cup_3_tm);
    world.Add(cup_3);

    Light cup_3_lt { light_tm * cup_3_tm * light_point, Colour { 0.1, 0.1, 0.2 } };
    world.Add(&cup_3_lt);

    Transformation cup_4_tm = Transformation()
        .Scale(scale)
        .RotateY(M_PI/12)
        .RotateZ(-M_PI/3)
        .Translate(-3*scale, 5*scale, 7*scale);
    ShapeGroup* cup_4 = cf.GetCup(cup_4_tm);
    world.Add(cup_4);

    Light cup_4_lt { light_tm * cup_4_tm * light_point, Colour { 0.0, 0.2, 0.3 } };
    world.Add(&cup_4_lt);


    Plane p {};
    p.SetTransform(
        Transformation()
        // .RotateX(M_PI/2)
        .Translate(0, -10*scale, 0)
    );
    Material pm = Material()
        .Surface(Colour {0.5, 0.2, 0.6});
        // .Reflectivity(1);
    p.SetMaterial(pm);
    world.Add(&p);

    // Material cup_m = CupMaterial();
    // Hemisphere h {};
    // h.SetMaterial(cup_m);
    // h.Closed(false);
    // h.SetTransform(
    //     Transformation()
    //     .Scale(scale)
    //     // .RotateZ(M_PI/2)
    //     .Translate(0, scale, 0)
    //     // .Shear(0.5*scale, 0, 0, 0.5*scale, 0, 0)
    //     // .RotateY(M_PI/12)
    // );
    // // world.Add(&h);

    // Matrix cup_transform = h.Transform();
    // Point light_point { -0.75, 0, 0 };
    // // Transformation light_transform = Transformation().Translate(0, -scale/2, 0);

    
    // Hemisphere s {};
    // s.SetTransform(
    //     cup_transform *
    //     Transformation()
    //     .Scale(0.25)
    //     .Translate(-0.25, 0, 0)
    //     .RotateZ(-M_PI)
    // );
    // // world.Add(&s);

    // ShapeGroup collection {};
    // world.Add(&collection);

    // // collection.SetTransform(Transformation().Scale(scale).Translate(0, scale, 0));
    // collection << &h << &s;

    // Light inside { /*Point { 0, 0.25*scale, 0 }*/ h.Transform() * light_point, Colour { 0.5, 0, 0.9 } };
    // world.Add(&inside);

    // // Hemisphere h2 {};
    // // h2.SetMaterial(cup_m);
    // // h2.Closed(false);
    // // h2.SetTransform(
    // //     Transformation()
    // //     .Scale(scale)
    // //     .RotateY(M_PI)
    // //     .Translate(2*scale, scale, 0)
    // //     // .Shear(0.5*scale, 0, 0, 0.5*scale, 0, 0)
    // //     // .RotateY(M_PI/12)
    // // );
    // // world.Add(&h2);

    // // Light inside2 { /*Point { 2.25*scale, 0.5*scale, 0 }*/ h2.Transform() * light_point, Colour { 0, 0.5, 0.9 } };
    // // world.Add(&inside2);

    
    
    // Hemisphere h3 {};
    // h3.SetMaterial(cup_m);
    // h3.Closed(false);
    // h3.SetTransform(
    //     Transformation()
    //     .Scale(scale)
    //     .Translate(-2*scale, scale, -scale)
    //     // .RotateZ(M_PI/12)
    //     // .Shear(0.5*scale, 0, 0, 0.5*scale, 0, 0)
    //     .RotateY(M_PI/12)
    // );
    // world.Add(&h3);
    
    // Light inside3 { /*Point { 2.25*scale, 0.5*scale, 0 }*/ h3.Transform() * light_point, Colour { 0.6, 0.5, 0 } };
    // world.Add(&inside3);
    
    // Hemisphere s2 {};
    // s2.SetTransform(
    //     h3.Transform() *
    //     Transformation()
    //     .Scale(0.25)
    //     .Translate(-0.25, 0, 0)
    //     .RotateZ(-M_PI)
    //     // .RotateY(M_PI/2)
    // );
    // Material s2_m {};
    // s2_m.Reflectivity(1.0);
    // s2.SetMaterial(s2_m);
    // world.Add(&s2);

    // Cube c {};
    // c.SetTransform(Transformation().Scale(scale).Translate(1.1*scale, 0, 0));
    // world.Add(&c);

    // Sphere s {};
    // s.SetTransform(Transformation().Scale(5).Translate(0, 5, 0));
    // world.Add(&s);

    Camera camera = SceneCamera(scale, 108, 135, M_PI / 3);
    Canvas canvas = camera.Render(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;
 
    // for (auto o: objects) {
    //     delete o;
    // }

    return 0;
}