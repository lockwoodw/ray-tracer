/*
The Ray Tracer Challenge: Chapter 15

Render a tetrahedron, four-sided pyramid, an octahedron, and a dodecahedron.

Supply a scaling factor at the command line to increase the image dimensions.
*/

#include <vector>
#include <algorithm>
#include <cmath>

#include "challenges.h"
#include "triangle.h"
#include "pattern.h"
#include "plane.h"
#include "polyhedron.h"

const double k90DegreesRad { M_PI / 2 };
const double k60DegreesRad { M_PI / 3 };
const double k45DegreesRad { M_PI / 4 } ;

Plane Floor(const Material &material) {
    Plane floor {};
    floor.SetMaterial(material);
    return floor;
}

Plane LeftWall(double scale, const Material &material) {
    Plane left_wall {};
    Matrix transform = Transformation()
        .RotateX(k90DegreesRad)
        .RotateY(-k45DegreesRad)
        .Translate(0, 0, 12 * scale);
    left_wall.SetTransform(transform);
    left_wall.SetMaterial(material);
    return left_wall;
}

Plane RightWall(double scale, const Material &material) {
    Plane right_wall {};
    Matrix transform = Transformation()
        .RotateX(k90DegreesRad)
        .RotateY(k45DegreesRad)
        .Translate(0, 0, 12 * scale);
    right_wall.SetTransform(transform);
    right_wall.SetMaterial(material);
    return right_wall;
}

Light WorldLight(double scale) {
    double scaled = 10 * scale;
    Point origin { -scaled, 2 * scaled, -scaled };
    Colour colour { 1, 1, 1 };
    return Light { origin, colour };
}

Matrix CameraTransform(double scale) {
    Point from { 5 * scale, 4.5 * scale, -10 * scale }, to { 0, scale, 0 };
    Vector up { 0, 1, 0 };
    return ViewTransform { from, to, up };
}

int main(int argc, char** argv) {
    double scale = GetScale(argc, argv);

    World world {};

    Light light = WorldLight(scale);
    world.Add(&light);

    // Add a second light for more definition of the faces
    Light light_2 { Point(-7 * scale, 4 * scale, -7 * scale), Colour(0.3, 0.3, 0.3) };
    world.Add(&light_2);

    Material floor_material {};
    CheckerPattern floor_pattern { Colour(0.95, 0.95, 0.95), Colour(0.75, 0.85, 0.95) };
    floor_pattern.SetTransform(Transformation()
        .Scale(scale*1.2)
    );
    floor_material.SurfacePattern(&floor_pattern);

    Plane floor = Floor(floor_material),
          left_wall = LeftWall(scale, floor_material),
          right_wall = RightWall(scale, floor_material);
    world.Add(&floor);
    world.Add(&left_wall);
    world.Add(&right_wall);

    PolyhedronFactory factory {};

    Polyhedron tetrahedron = factory.Tetrahedron();
    tetrahedron->SetTransform(Transformation().RotateY(M_PI/9).Translate(-3, 0, -1).Scale(1.5*scale));
    tetrahedron->SetMaterial(Material().Surface(Colour(0, 0.8, 1)));
    world.Add(tetrahedron);

    Polyhedron pyramid = factory.Pyramid();
    pyramid->SetTransform(Transformation().RotateY(-M_PI/6).Translate(1, 0, -2.75).Scale(scale));
    pyramid->SetMaterial(Material().Surface(Colour(0.9, 0.7, 0.2)));
    world.Add(pyramid);

    Polyhedron octahedron = factory.Octahedron();
    double height = 2 / std::sqrt(2);
    octahedron->SetTransform(
        Transformation()
            .RotateY(k45DegreesRad)
            .Translate(-2, height, 0)
            .Scale(scale)
    );
    octahedron->SetMaterial(Material().Surface(Colour(0.7, 0.1, 0.9)));
    world.Add(octahedron);

    Polyhedron dodecahedron = factory.Dodecahedron();
    // rotation required to sit the dodecahedron on its bottom face
    // (the difference between a straight line [180] and the internal angle, halved)
    double x_rot_offset { (180 - 116.56505) / 2 };
    double y_offset { 1.376381 }; // determined empirically
    dodecahedron->SetTransform(
        Transformation()
            .RotateX(x_rot_offset*M_PI/180)
            .Translate(1.5, y_offset, 1.5)
            .Scale(scale)
    );
    dodecahedron->SetMaterial(Material().Surface(Colour(1, 0, 0)));
    world.Add(dodecahedron);

    Camera camera = SceneCamera(scale, 200, 100, k60DegreesRad, CameraTransform(scale));
    Canvas canvas = camera.RenderConcurrent(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;

    return 0;
}
