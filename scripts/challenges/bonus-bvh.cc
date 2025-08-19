/*
The Ray Tracer Challenge: Bonus Chapter, "Bounding boxes and hierarchies"

http://raytracerchallenge.com/bonus/bounding-boxes.html

Render a scene containing a lot of objects, to confirm that a bounding volume
hierarchy (BVH) facilitates rendering in a reasonable amount of time.

Supply a scaling factor at the command line to increase the image dimensions.
*/

#include <vector>

#include "challenges.h"
#include "sphere.h"
#include "group.h"
#include "plane.h"

Light WorldLight(double scale) {
    Point origin { 60*scale, 60 * scale, -20*scale };
    Colour colour { 1, 1, 1 };
    return Light { origin, colour };
}

Matrix CameraTransform(double scale) {
    Point from { 50*scale, 50*scale, -70 * scale }, to { 30*scale, 40*scale, 0 };
    Vector up { 0, 1, 0 };
    return ViewTransform { from, to, up };
}

int main(int argc, char** argv) {
    double scale = GetScale(argc, argv);

    World world {};

    Light light = WorldLight(scale);
    world.Add(&light);

    std::vector<Colour> colours = {
        Colour { 1, 0, 0 },
        Colour { 0, 1, 0 },
        Colour { 0, 0, 1 },
        Colour { 1, 1, 0 },
        Colour { 0, 1, 1 },
        Colour { 1, 0, 1 }
    };

    ShapeGroup shapes {};
    world.Add(&shapes);

    std::vector<Shape*> objects {};
    int dim { 20 };
    for (int y = 0; y < dim; y++) {
        for (int z = 0; z < dim; z++) {
            for (int x = 0; x < dim; x++) {
                Sphere* s = new Sphere();
                objects.push_back(s);
                shapes << s;
                s->SetTransform(
                    Transformation()
                    .Scale(scale)
                    .Translate(2*x*scale, 2*y*scale, 2*z*scale)
                );
                Material m {};
                m.Surface(colours[(x + y + z) % colours.size()]);
                s->SetMaterial(m);
            }
        }
    }

    shapes.Divide(10000);

    Camera camera = SceneCamera(scale, 108, 135, M_PI / 3, CameraTransform(scale));
    Canvas canvas = camera.Render(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;
 
    for (auto o: objects) {
        delete o;
    }

    return 0;
}

