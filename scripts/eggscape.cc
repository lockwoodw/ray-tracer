/*
Render a scene composed of randomly positioned submerged spheres of different sizes and colours.

Supply a scaling factor at the command line to increase the resolution.
*/

#include <vector>
#include <ctime>
#include <cstdlib>
#include <cstring> // memset

#include "scripts.h"
#include "plane.h"
#include "sphere.h"
#include "pattern.h"
#include "group.h"

class PatternManager {
    std::vector<Pattern*> patterns_;

    public:
        PatternManager(): patterns_ {} {}
        ~PatternManager();
        Pattern* GroundPattern(double scale);
        Pattern* HorizonPattern(double scale);
        Pattern* BlobPattern(double scale, const Colour& base_colour);
};

PatternManager::~PatternManager() {
    for (Pattern* pattern: patterns_) {
        delete pattern;
    }
}

Pattern* PatternManager::GroundPattern(double scale) {
    RadialGradientPattern* rgp = new RadialGradientPattern(
        Colour { 184.0 / 255, 134.0 / 255, 11.0 / 255 }, // dark golden rod
        Colour { 245.0 / 255, 222.0 / 255, 179.0 / 255 } // wheat
    );
    rgp->SetTransform(Transformation().Scale(0.125 * scale, 1, 1));
    PerturbedPattern* ground_pattern = new PerturbedPattern(rgp);
    patterns_.push_back(rgp);
    patterns_.push_back(ground_pattern);
    return ground_pattern;
}

Pattern* PatternManager::HorizonPattern(double scale) {
    GradientPattern* gp = new GradientPattern(
        Colour { 140.0 / 255, 190.0 / 255, 214.0 / 255 }, // dark sky blue
        Colour { 135.0 / 255, 206.0 / 255, 250.0 / 255 }  // light sky blue
    );
    gp->SetTransform(Transformation().Scale(scale * 20, 1, 1).RotateY(-M_PI / 2));
    PerturbedPattern* horizon_pattern = new PerturbedPattern(gp);
    patterns_.push_back(gp);
    patterns_.push_back(horizon_pattern);
    return horizon_pattern;
}

Pattern* PatternManager::BlobPattern(double scale, const Colour& base_colour) {
    RadialGradientPattern* rgp = new RadialGradientPattern(
        base_colour,
        Colour { 152.0 / 255, 118.0 / 255, 84.0 / 255 } // pale brown
    );
    rgp->SetTransform(Transformation().Scale(0.25 * scale, 1, 1));
    PerturbedPattern* blob_pattern = new PerturbedPattern(rgp);
    patterns_.push_back(rgp);
    patterns_.push_back(blob_pattern);
    return blob_pattern;
}

Plane Ground(Pattern* ground_pattern) {
    Plane ground {};
    Material material {};
    material.Specular(0);
    material.SurfacePattern(ground_pattern);
    ground.SetMaterial(material);
    // ground.SetTransform(Transformation().RotateZ(-M_PI / 48));
    return ground;
}

Plane Horizon(double scale, Pattern* horizon_pattern) {
    Plane horizon {};
    Material material {};
    material.Specular(0);
    material.SurfacePattern(horizon_pattern);
    horizon.SetMaterial(material);
    Matrix transform = Transformation().RotateX(M_PI/2).Translate(0, 0, 200 * scale);
    horizon.SetTransform(transform);
    return horizon;
}

class SimpleRandomNumberGenerator {
    public:
        SimpleRandomNumberGenerator() {
            std::srand(std::time(nullptr));
        }
        int Number() const {
            return std::rand();
        }
};

Colour RandomBlobColour(const SimpleRandomNumberGenerator& srng) {
    auto Number = [srng] () -> double {
        int number = srng.Number();
        return static_cast<double>(number % 7 + 1) / 14;
    };
    Colour base { 152.0 / 255, 118.0 / 255, 84.0 / 255 };
    return Colour { base.Red() + Number(), base.Green() + Number(), base.Blue() + Number() };
}

Sphere Blob(double scale, double x, double z, const SimpleRandomNumberGenerator& srng,
        PatternManager& pm) {
    Material material {};
    material.Specular(0.1);
    material.Shininess(10);
    material.Diffuse(1);
    material.SurfacePattern(pm.BlobPattern(scale, RandomBlobColour(srng)));
    Sphere blob {};
    blob.SetMaterial(material);
    double size = scale * 0.25 * (srng.Number() % 3 + 1),
           x_size = size,
           y_size = size,
           z_size = size,
           extra = size + 0.125 * (srng.Number() % 3 + 1);
    // Turn sphere into ellipsoid in one dimension
    switch (srng.Number() % 3) {
        case 0:
            x_size += extra;
            break;

        case 1:
            y_size += extra / 3; // reduce blob height
            break;

        default:
            z_size += extra;
    }
    double rotation = M_PI / (srng.Number() % 3 + 1);
    Matrix transform = Transformation()
        .Scale(x_size, y_size, z_size)
        .Translate(x * scale, 0, z * scale)
        .RotateY(rotation);
    blob.SetTransform(transform);
    return blob;
}

Light WorldLight(double scale) {
    double scaled = 10 * scale;
    Point origin { -scaled, scaled, -scaled };
    Colour colour { 1, 1, 1 };
    return Light { origin, colour };
}

Matrix CameraTransform(double scale) {
    Point from { 10*scale, scale, -10 * scale }, to { 0, scale, 0 };
    Vector up { 0, 1, 0 };
    return ViewTransform { from, to, up };
}

int main(int argc, char** argv) {
    double scale { 1 }, max_scale { 20 };

    if (argc > 1) {
        scale = atof(argv[1]);
    }

    if (scale <= 0 || scale > max_scale) {
        std::cerr << "Given scale invalid (1-" << max_scale << ")" << std::endl;
        return 1;
    }

    int scale_int = static_cast<int>(scale);

    World world {};
    PatternManager pattern_mgr {};

    Plane ground = Ground(pattern_mgr.GroundPattern(scale));
    world.Add(&ground);

    Plane horizon = Horizon(scale, pattern_mgr.HorizonPattern(scale));
    world.Add(&horizon);

    // Create a map to track where the spheres are positioned in the x-z plane
    // to ensure none have the same origin.
    SimpleRandomNumberGenerator srng {};
    struct MapPosition {
        int x_;
        int z_;
    };    
    int n_blobs { 2 };
    int map_dimension = scale_int * n_blobs;
    n_blobs += 10 * scale_int; // ensure larger scaled scenes have additional blobs
    int** map = new int*[map_dimension];

    for (int i = 0; i < map_dimension; i++) {
        map[i] = new int[map_dimension];
        memset(map[i], 0, map_dimension * sizeof(int));
    }

    auto XZ = [srng, map_dimension] (int** m) -> MapPosition {
        MapPosition position {};
        while (true) {
            position.x_ = srng.Number() % map_dimension;
            position.z_ = srng.Number() % map_dimension;
            if (m[position.x_][position.z_] == 0) {
                m[position.x_][position.z_] = 1;
                break;
            }
        }
        // Ensure spheres are approx. evenly distributed in -x and +x space
        position.x_ -= map_dimension / 2;
        return position;
    };

    // Generate the spheres
    ShapeGroup collection {};
    world.Add(&collection);

    Sphere blobs[n_blobs] {};
    for (int i = 0; i < n_blobs; i++) {
        MapPosition position = XZ(map);
        blobs[i] = Blob(scale, position.x_, position.z_, srng, pattern_mgr);
        collection.Add(&blobs[i]);
    }

    Light light = WorldLight(scale);
    world.Add(&light);

    Camera camera { 191 * scale_int, 100 * scale_int, M_PI / 3 };
    camera.SetTransform(CameraTransform(scale));

    Canvas canvas = camera.RenderConcurrent(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;

    // Clean up heap
    for (int i = 0; i < map_dimension; i++) {
        delete[] map[i];
    }
    delete[] map;

    return 0;
}