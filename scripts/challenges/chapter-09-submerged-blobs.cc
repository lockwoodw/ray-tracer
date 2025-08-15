/*
The Ray Tracer Challenge: Chapter 9

Render a scene composed of randomly positioned submerged spheres of different
sizes and colours.

Supply a scaling factor at the command line to increase the image dimensions.
*/

#include <ctime>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <cstring> // memset

#include "sphere.h"
#include "plane.h"
#include "matrix.h"
#include "transformations.h"
#include "material.h"
#include "colour.h"
#include "world.h"
#include "space.h"
#include "camera.h"
#include "canvas.h"

class SimpleRandomNumberGenerator {
    public:
        SimpleRandomNumberGenerator() {
            std::srand(std::time(nullptr));
        }
        int Number() const {
            return std::rand();
        }
};

Plane Floor() {
    Plane floor {};
    Material material {};
    Colour colour { 1, 0.9, 0.9 };
    material.Surface(colour);
    material.Specular(0);
    floor.SetMaterial(material);
    return floor;
}

Colour RandomColour(const SimpleRandomNumberGenerator& srng) {
    auto Number = [srng] () -> double {
        int number = srng.Number();
        return static_cast<double>(number % 100 + 1) / 100;
    };
    return Colour { Number(), Number(), Number() };
}

Sphere Blob(double scale, double x, double z, const SimpleRandomNumberGenerator& srng) {
    Material material {};
    material.Surface(RandomColour(srng));
    material.Specular(0.1);
    Sphere blob {};
    blob.SetMaterial(material);
    double size = scale * (srng.Number() % 5 + 1),
           x_size = size,
           y_size = size,
           z_size = size,
           extra = size + (srng.Number() % 5 + 1);
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
    Matrix transform = Transformation().Scale(x_size, y_size, z_size).Translate(x * scale, 0, z * scale);
    blob.SetTransform(transform);
    return blob;
}

Light WorldLight(double scale) {
    double scaled = 5 * scale;
    return Light { Point { 0, 5 * scaled, scaled }, Colour { 1, 1, 1 } };
}

Matrix CameraTransform(double scale) {
    Point from { 0, 15 * scale, -20 * scale }, to { 0, 8 * scale, 0 };
    Vector up { 0, 1, 0 };
    return ViewTransform { from, to, up };
}

struct MapPosition {
    int x_;
    int z_;
};

int main(int argc, char** argv) {
    double scale { 1 }, max_scale { 20 };

    if (argc > 1) {
        scale = atof(argv[1]);
    }

    if (scale <= 0 || scale > max_scale) {
        std::cerr << "Given scale invalid (1-" << max_scale << ")" << std::endl;
        return 1;
    }

    SimpleRandomNumberGenerator srng {};

    World world {};
    Plane floor = Floor();
    world.Add(&floor);

    // Create a map to track where the spheres are positioned in the x-z plane
    // to ensure none have the same origin.
    int n_blobs { 20 }, scale_int = static_cast<int>(std::nearbyint(scale));
    int map_dimension = scale_int * n_blobs;
    n_blobs += 2 * scale_int; // ensure larger scaled scenes have additional blobs
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
    Sphere blobs[n_blobs] {};
    for (int i = 0; i < n_blobs; i++) {
        MapPosition position = XZ(map);
        blobs[i] = Blob(scale, position.x_, position.z_, srng);
        world.Add(&blobs[i]);
    }

    Light light = WorldLight(scale);
    world.Add(&light);

    Camera camera { 200 * scale_int, 100 * scale_int, M_PI / 3 };
    camera.SetTransform(CameraTransform(scale));

    Canvas canvas = camera.Render(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;

    // Clean up heap
    for (int i = 0; i < map_dimension; i++) {
        delete[] map[i];
    }
    delete[] map;

    return 0;
}