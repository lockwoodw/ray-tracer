#include <vector>

#include "scripts.h"
#include "plane.h"
#include "sphere.h"
#include "pattern.h"
#include "group.h"
#include "hemisphere.h"

Matrix CameraTransform(double scale) {
    Point from { -0.5*scale, scale, -7* scale }, to { -0.5*scale, 0.5*scale, 0 };
    Vector up { 0, 1, 0 };
    return ViewTransform { from, to, up };
}

using Cup = ShapeGroup*;

class CupFactory {
    std::vector<Shape*> objects_;

    public:
        CupFactory() {}
        ~CupFactory() {
            for (auto o: objects_) {
                delete o;
            }
        }
        Cup GetCup(const Transformation& t) {
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

            Cup collection = new ShapeGroup();
            objects_.push_back(collection);
            *collection << cup << bob;
            return collection;
        }

        const Material CupMaterial(const Colour& colour = Colour::kWhite) const {
            return Material()
                .Transparency(1.0)
                .RefractiveIndex(2.52)
                .Diffuse(0.9)
                .Ambient(0.1)
                .Reflectivity(1.0)
                .Shininess(300)
                .Specular(1)
                .Surface(colour);
        }

        const Material BobMaterial(const Colour& colour = Colour::kWhite) const {
            return Material()
                .Reflectivity(1.0);
        }
};

int main(int argc, char** argv) {
    double scale = GetScale(argc, argv);

    World world {};

    CupFactory cf {};

    Transformation cup_1_tm = Transformation()
        .Scale(scale)
        .RotateY(-M_PI/12)
        .RotateZ(-M_PI/12)
        .Translate(0, 0.5*scale, 0);

    Cup cup_1 = cf.GetCup(cup_1_tm);
    world.Add(cup_1);

    Point light_point { 0, 0, 0 };
    Transformation light_tm = Transformation().Translate(-0.5, 0, 0);

    Light cup_1_lt { light_tm * cup_1_tm * light_point, Colour { 0.3, 0.2, 0.2 } };
    world.Add(&cup_1_lt);

    Transformation cup_2_tm = Transformation()
        .Scale(scale)
        .RotateY(M_PI/12)
        .Translate(-2*scale, scale, -scale);

    Cup cup_2 = cf.GetCup(cup_2_tm);
    world.Add(cup_2);

    Light cup_2_lt { light_tm * cup_2_tm * light_point, Colour { 0.1, 0.2, 0.1 } };
    world.Add(&cup_2_lt);

    Transformation cup_3_tm = Transformation()
        .Scale(scale)
        .RotateY(M_PI*165/180)
        .RotateZ(M_PI/4)
        .Translate(2*scale, scale, 1.5*scale);

    Cup cup_3 = cf.GetCup(cup_3_tm);
    world.Add(cup_3);

    Light cup_3_lt { light_tm * cup_3_tm * light_point, Colour { 0.1, 0.1, 0.2 } };
    world.Add(&cup_3_lt);

    Transformation cup_4_tm = Transformation()
        .Scale(scale)
        .RotateY(M_PI/12)
        .RotateZ(-M_PI/3)
        .Translate(-3*scale, 5*scale, 7*scale);
    Cup cup_4 = cf.GetCup(cup_4_tm);
    world.Add(cup_4);

    Light cup_4_lt { light_tm * cup_4_tm * light_point, Colour { 0.0, 0.2, 0.3 } };
    world.Add(&cup_4_lt);

    Plane p {};
    world.Add(&p);

    p.SetTransform(
        Transformation()
        .Translate(0, -10*scale, 0)
    );

    p.SetMaterial(Material()
        .Surface(Colour {0.5, 0.2, 0.6}));

    Camera camera = SceneCamera(scale, 108, 135, M_PI / 3, CameraTransform(scale));
    Canvas canvas = camera.Render(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;
 
    return 0;
}