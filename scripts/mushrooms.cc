#include <vector>

#include "scripts.h"
#include "hemisphere.h"
#include "plane.h"
#include "disc.h"
#include "pattern.h"
#include "cone.h"
#include "group.h"

Light WorldLight(double scale) {
    // Point origin { -15 * scale, 30*scale, -30 * scale };
    Point origin { -15 * scale, 2*scale, -30 * scale };
    Colour colour { 1, 1, 1 };
    return Light { origin, colour };
}

Matrix CameraTransform(double scale) {
    // Point from { 15*scale, 15*scale, -40*scale }, to { 15*scale, 12*scale, 0 };
    Point from { 10*scale, 0.5*scale, -20*scale }, to { 9*scale, 2*scale, 0 };
    Vector up { 0, 1, 0 };
    return ViewTransform { from, to, up };
}

static Colour
    kAlmond { 239.0 / 255, 222.0 / 255, 205.0 / 255 },
    kBlanchedAlmond { 1.0 ,235.0 / 255, 205.0 / 255 },
    kPaleBrown { 152.0 / 255, 118.0 / 255, 84.0 / 255 },
    kRed { 1.0, 0, 0 },
    kDarkRed { 139.0 / 255, 0, 0 },
    kFernGreen { 79.0 / 255, 121.0 / 255, 66.0 / 255 },
    kNeonGreen { 57.0 / 255, 1.0, 20.0 / 255 }
;

using Mushroom = ShapeGroup*;

class MushroomFactory {
    std::vector<Shape*> objects_;
    std::vector<Pattern*> patterns_;

    void AddCap(Mushroom mushroom, const Colour& c1, const Colour &c2,
            double scale, double head_offset) {
        Hemisphere* cap = new Hemisphere();
        objects_.push_back(cap);
        mushroom->Add(cap);

        cap->Closed(false);

        std::uniform_real_distribution<double> urd { 1.0, 5.0 };
        double head_scale { 2 * scale },
               vertical_scale { urd(kMTEngine) * scale };

        cap->SetTransform(
            Transformation()
            .RotateZ(-M_PI/2)
            .Scale(head_scale, vertical_scale, head_scale)
            .Translate(0, head_offset, 0)
        );

        SpeckledPattern* speckled_ptn_1 = new SpeckledPattern(c1);
        patterns_.push_back(speckled_ptn_1);
        speckled_ptn_1->SetDarkThreshold(0.8);
        speckled_ptn_1->SetLightThreshold(0.8);
        speckled_ptn_1->SetAttentuation(0.5);

        SpeckledPattern* speckled_ptn_2 = new SpeckledPattern(c2);
        patterns_.push_back(speckled_ptn_2);
        speckled_ptn_2->SetDarkThreshold(0.8);
        speckled_ptn_2->SetLightThreshold(0.8);
        speckled_ptn_2->SetAttentuation(0.8);

        GradientPattern* gradient_ptn = new GradientPattern(speckled_ptn_2, speckled_ptn_1);
        patterns_.push_back(gradient_ptn);

        PerturbedPattern* perturbed_ptn = new PerturbedPattern(gradient_ptn);
        patterns_.push_back(perturbed_ptn);

        cap->SetMaterial(
            Material()
            .SurfacePattern(perturbed_ptn)
            .Shininess(10)
            .Specular(0.2)
        );

        Disc* underside = new Disc();
        objects_.push_back(underside);
        mushroom->Add(underside);

        RingPattern* ring_ptn = new RingPattern(Colour::kWhite, kPaleBrown);
        patterns_.push_back(ring_ptn);
        ring_ptn->SetTransform(
            Transformation()
            .Scale(0.05, 1, 0.05)
        );

        PerturbedPattern* perturbed_ptn_2 = new PerturbedPattern(ring_ptn);
        patterns_.push_back(perturbed_ptn_2);

        underside->SetMaterial(Material().SurfacePattern(perturbed_ptn_2));

        underside->SetTransform(
            Transformation()
            .Scale(head_scale*0.99999)
            .Translate(0, head_offset + 1e-1*scale, 0)
        );
    }

    void AddStem(Mushroom mushroom, double scale, double head_offset) {
        Cone* stem = new Cone (-1, 0, true);
        objects_.push_back(stem);
        mushroom->Add(stem);

        SpeckledPattern *speckled_brown    = new SpeckledPattern(kPaleBrown),
                        *speckled_blanched = new SpeckledPattern(kBlanchedAlmond),
                        *speckled_almond   = new SpeckledPattern(kAlmond);
        patterns_.push_back(speckled_brown);
        patterns_.push_back(speckled_blanched);
        patterns_.push_back(speckled_almond);

        GradientPattern* gradient_ptn = new GradientPattern(speckled_brown, speckled_blanched);
        patterns_.push_back(gradient_ptn);

        StripePattern* stripe_ptn = new StripePattern(gradient_ptn, speckled_almond);
        patterns_.push_back(stripe_ptn);
        stripe_ptn->SetTransform(
            Transformation()
            .Scale(0.7, 1, 1)
        );

        PerturbedPattern* perturbed_ptn = new PerturbedPattern(stripe_ptn);
        patterns_.push_back(perturbed_ptn);

        stem->SetMaterial(
            Material()
            .SurfacePattern(perturbed_ptn)
            .Shininess(10)
            .Specular(0.2)
        );

        stem->SetTransform(
            Transformation()
            .Translate(0, 1, 0)
            .Scale(1.1*scale, 1.3*head_offset, 1.1*scale)
        );
    }

    public:
        MushroomFactory(): objects_ {}, patterns_ {} {}
        ~MushroomFactory() {
            for (auto o: objects_) {
                delete o;
            }
            for (auto p: patterns_) {
                delete p;
            }
        }

        Mushroom Generate(const Colour& c1, const Colour &c2, double scale) {
            Mushroom mushroom = new ShapeGroup();
            objects_.push_back(mushroom);

            std::uniform_real_distribution<double> urd { 2.5, 4.5 };
            double head_offset = urd(kMTEngine) * scale;
            AddCap(mushroom, c1, c2, scale, head_offset);
            AddStem(mushroom, scale, head_offset);
            return mushroom;
        }
};

Plane Sky(double scale) {
    Plane sky {};

    sky.SetTransform(
        Transformation()
        .RotateX(M_PI/2)
        .Translate(0, 0, scale * 1000)
    );

    sky.SetMaterial(
        Material()
        .Surface(Colour(0.2, 0.6, 0.8))
        .Specular(0)
        // .Shininess(300)
    );

    return sky;
}

int main(int argc, char** argv) {
    int scale = GetScale(argc, argv);

    World world {};

    Light light = WorldLight(scale);
    world.Add(&light);

    Plane ground {};
    world.Add(&ground);

    SpeckledPattern neon_ptn { kNeonGreen },
                    fern_ptn { kFernGreen };
    RadialGradientPattern gradient_ptn { &neon_ptn, &fern_ptn };
    gradient_ptn.SetTransform(Transformation().Scale(0.1, 1, 1));
    PerturbedPattern perturbed_ptn { &gradient_ptn };

    ground.SetMaterial(
        Material()
        .SurfacePattern(&perturbed_ptn)
        .Shininess(0)
        .Specular(0)
    );

    Plane sky = Sky(scale);
    world.Add(&sky);

    MushroomFactory factory {};
    ShapeGroup mushrooms {};
    world.Add(&mushrooms);

    Colour c1 = kRed,
           c2 = kDarkRed;

    std::uniform_real_distribution<double> rotation { -M_PI/12, M_PI/12 };
    std::uniform_real_distribution<double> mushroom_scale { 0.8, 2.0 };

    for (int i = 0; i < 15; i++) {
        Mushroom mushroom = factory.Generate(c1, c2, scale);
        mushrooms.Add(mushroom);
        double z_rot = rotation(kMTEngine),
               local_scale = mushroom_scale(kMTEngine);
        mushroom->SetTransform(
            Transformation()
            .Scale(local_scale)
            .RotateZ(z_rot)
            .Translate(i*scale*local_scale*2 + z_rot, -scale*std::fabs(z_rot), (i % 2 == 0 ? -i : i)*scale*local_scale*2)
        );
        c1 = GetNewColour(c1);
        c2 = GetNewColour(c2);
    }

    Camera camera = SceneCamera(scale, 108, 135, M_PI / 3, CameraTransform(scale));
    Canvas canvas = camera.RenderConcurrent(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;

    return 0;
}