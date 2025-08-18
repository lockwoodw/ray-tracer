#include "scripts.h"
#include "plane.h"
#include "sphere.h"
#include "pattern.h"
#include "group.h"

// Light WorldLight(double scale) {
//     Point origin { -5*scale, 5 * scale, -5*scale };
//     Colour colour { 1, 1, 1 };
//     return Light { origin, colour };
// }

Light WorldLight(double scale) {
    Point origin { -25 * scale, 30 * scale, -20 * scale };
    Colour colour { 1, 1, 1 };
    return Light { origin, colour };
}

Matrix CameraTransform(double scale) {
    // Point from { -0.5*scale, 3*scale, -6.5* scale }, to { -0.5*scale, 2*scale, 0 };
    // Point from { 0, 3*scale, -3* scale }, to { 0, 0, -2*scale };
    // Point from { 0.25 * scale, 2.5 * scale, -5 * scale }, to { 0, 0, 0 };
    Point from { 2.1 * scale, 0.5 * scale, -2 * scale }, to { 0, scale, -0.5*scale };
    Vector up { 0, 1, 0 };
    return ViewTransform { from, to, up };
}

class PatternFactory {
    std::vector<Pattern*> patterns_;

    public:
        PatternFactory(): patterns_ {} {}

        ~PatternFactory() {
            for (auto p: patterns_) {
                delete p;
            }
        }

        Pattern* SandPattern(const Colour& colour, /*const Colour& light,*/
                const Matrix* transform = nullptr) {
            SpeckledPattern* speckled_ptn = new SpeckledPattern(colour);
            patterns_.push_back(speckled_ptn);
            speckled_ptn->SetDarkThreshold(0.8);
            speckled_ptn->SetAttentuation(0.3);
            speckled_ptn->SetLightThreshold(0.8);
            // SpeckledPattern* light_ptn = new SpeckledPattern(light);
            // patterns_.push_back(light_ptn);
            // RadialGradientPattern* gradient_ptn = new RadialGradientPattern(dark_ptn, light_ptn);
            // patterns_.push_back(gradient_ptn);
            // if (transform) {
            //     gradient_ptn->SetTransform(*transform);
            // }
            // PerturbedPattern* perturbed_ptn = new PerturbedPattern(gradient_ptn);
            return speckled_ptn;
        }

        Pattern* RockPattern(const Colour& dark, const Colour& light,
                const Matrix* transform = nullptr) {
            SpeckledPattern* dark_ptn = new SpeckledPattern(dark);
            patterns_.push_back(dark_ptn);
            dark_ptn->SetDarkThreshold(0.8);
            SpeckledPattern* light_ptn = new SpeckledPattern(light);
            patterns_.push_back(light_ptn);
            light_ptn->SetDarkThreshold(0.8);
            light_ptn->SetLightThreshold(0.8);
            light_ptn->SetAttentuation(0.8);
            RadialGradientPattern* gradient_ptn = new RadialGradientPattern(dark_ptn, light_ptn);
            patterns_.push_back(gradient_ptn);
            if (transform) {
                gradient_ptn->SetTransform(*transform);
            }
            PerturbedPattern* perturbed_ptn = new PerturbedPattern(gradient_ptn);
            return perturbed_ptn;
        }
};

Material RockMaterial(Pattern* ptn) {
    return Material()
        .SurfacePattern(ptn)
        .Shininess(10)
        .Specular(0.1)
        .Diffuse(1);
}

Plane Sky(double scale) {
    Plane sky {};

    sky.SetTransform(
        Transformation()
        .RotateX(M_PI/2)
        .Translate(0, 0, scale * 70)
    );

    sky.SetMaterial(
        Material()
        .Surface(Colour(0.2, 0.6, 0.8))
        .Specular(0)
    );

    return sky;
}

Plane Water(double scale) {
    Plane water {};
    
    water.SetTransform(
        Transformation()
        .RotateZ(-M_PI*179/180)
        .Translate(0, 0.75 * scale, 0)
    );
    
    water.SetMaterial(
        Material()
        .Transparency(1.0)
        .RefractiveIndex(1.0 + 1.0/3)
        .Diffuse(0.1)
        .Ambient(0.1)
        .Reflectivity(0.9)
        .Specular(1)
        .Surface(Colour { 0.0, 0.5, 1 })
        // .CastsShadow(false)
    );

    return water;
}

using Bubble = ShapeGroup*;

class BubbleFactory {
    std::vector<Shape*> shapes_;

    public:
        BubbleFactory(): shapes_ {} {}

        ~BubbleFactory() {
            for (auto s: shapes_) {
                delete s;
            }
        }

        Bubble Generate(const Colour& colour, double scale, double bubble_ratio=0.98) {
            Bubble bubble = new ShapeGroup();
            shapes_.push_back(bubble);

            Sphere* outside = new Sphere();
            shapes_.push_back(outside);
            outside->SetTransform(Transformation().Scale(scale));
            outside->SetMaterial(GlassMarbleMaterial(colour));
            *bubble << outside;
            
            if (bubble_ratio > 0.0 && bubble_ratio < 1.0) {
                Sphere* inside = new Sphere();
                shapes_.push_back(inside);
                inside->SetTransform(Transformation().Scale(scale * bubble_ratio));
                inside->SetMaterial(AirBubbleMaterial());
                *bubble << inside;
            }

            return bubble;
        }

};

int main(int argc, char** argv) {
    double scale = GetScale(argc, argv);

    World world {};

    Light light = WorldLight(scale);
    world.Add(&light);
    
    ShapeGroup rocks {};
    world.Add(&rocks);
    
    PatternFactory pattern_factory {};
    
    Colour
    pale_brown      { 152.0 / 255, 118.0 / 255,  84.0 / 255 },
    wood_brown      { 193.0 / 255, 154.0 / 255, 107.0 / 255 },
    spanish_bistre  { 128.0 / 255, 117.0 / 255,  90.0 / 255 },
    bistre          {  61.0 / 255,  43.0 / 255,  31.0 / 255 },
    raw_umber       { 130.0 / 255, 102.0 / 255,  68.0 / 255 },
    almond          { 239.0 / 255, 222.0 / 255,  205.0 / 255 },
    blanched_almond { 1.0,         235.0 / 255,  almond.Blue() };
    
    Sphere rock_1 {};
    rocks.Add(&rock_1);
    
    rock_1.SetTransform(
        Transformation()
        // .Scale(scale)
        .Scale(scale, 0.8*scale, 1.3*scale)
        // .RotateY(M_PI/8)
        .RotateX(M_PI/2)
        .Translate(-scale, -0.1*scale, -scale)
    );

    rock_1.SetMaterial(RockMaterial(pattern_factory.RockPattern(
        spanish_bistre, raw_umber, &rock_1.InverseTransform()
    )));
        
    Sphere rock_2 {};
    rocks.Add(&rock_2);
    
    rock_2.SetTransform(
        Transformation()
        .Scale(scale, scale, 2*scale)
        .RotateY(-M_PI/3)
        .RotateZ(M_PI/12)
        .Translate(0.15*scale, 0, 1.25 * scale)
        // .Shear(0, 0, 0, 0, 0, 2)
    );
    
    rock_2.SetMaterial(RockMaterial(pattern_factory.RockPattern(
        pale_brown, wood_brown, &rock_2.InverseTransform()
    )));
    
    Sphere rock_3 {};
    rocks.Add(&rock_3);

    rock_3.SetTransform(
        Transformation()
        .Scale(0.6*scale, 0.4*scale, 0.4*scale)
        .Translate(0.5*scale, 0.15*scale, -2*scale)
    );

    rock_3.SetMaterial(RockMaterial(pattern_factory.RockPattern(
        bistre, bistre, &rock_3.InverseTransform()
    )));

    Plane sand {};
    world.Add(&sand);

    sand.SetMaterial(Material().SurfacePattern(pattern_factory.SandPattern(
        blanched_almond/*, almond*/
    )));
    
    Plane sky = Sky(scale);
    world.Add(&sky);

    Plane water = Water(scale);
    world.Add(&water);

    BubbleFactory bubble_factory {};

    for (int i = 0; i < 6; i++) {
        Bubble bubble = bubble_factory.Generate(Colour { 1, 1, 1 }, 0.05*scale, 0.999999);
        rocks.Add(bubble);
        bubble->SetTransform(
            Transformation()
            // .Scale(0.1*scale)
            .Shear(0, 0, 0.5, 0, 0, 0)
            .Translate(scale + i*(i%2-1)*0.1*scale, 0.05*scale + i*0.1*scale, -scale + i*0.1*scale)
        );
    }

    Camera camera = SceneCamera(scale, 108, 135, M_PI / 3, CameraTransform(scale));
    Canvas canvas = camera.Render(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;
    return 0;
}
