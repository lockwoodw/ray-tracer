/*
Render an abstract scene of triangular "leaves" constructed and placed using a
version of the drunken ant algorithm.

Supply a scaling factor at the command line to increase the image dimensions.
*/

#include <vector>
#include <random>
#include <ctime>
#include <cmath>
#include <algorithm>

#include "scripts.h"
#include "triangle.h"
#include "plane.h"
#include "group.h"

Light WorldLight(double scale) {
    Point origin {  -scale, 10 * scale,  scale };
    Colour colour { 1, 1, 1 };
    return Light { origin, colour };
}

Matrix CameraTransform(double scale) {
    Point from { -scale, 6 * scale, -10 * scale }, to { 0, scale, 0 };
    Vector up { 0, 1, 0 };
    return ViewTransform { from, to, up };
}

bool sort_triangle_by_area(Triangle *a, Triangle *b)
{
    return a->Area() > b->Area();
}

int main(int argc, char** argv) {
    double scale = GetScale(argc, argv);
    int width { 108 },
        height { 135 };

    // max. width/height from (0, 0); adjust these values to constrain the area
    // of the field along which the leaves are placed
    double width_magnitude { width * scale /2 },
           height_magnitude { height * scale /3 };

    World world {};

    Light light = WorldLight(scale);
    world.Add(&light);

    std::vector<Triangle *> leaves {};
    std::vector<Point> vertices {};

    // Initialization for drunken ant algorithm
    int n_leaves { 300 };
    int n_drinks { 1 };
    Point current_pt { 0, 0, 0 };
    double current_length {};
    double init_velocity { std::sqrt(0.5) };
    double velocity[] { init_velocity, init_velocity };
    double acceleration {};
    std::mt19937_64 mt_engine_ { static_cast<long unsigned int>(time(nullptr)) };
    std::uniform_real_distribution<double> urd_ { -1.0, 1.0 },
        accel_urd { -0.1, 0.1 };
    double angular { urd_(mt_engine_) };
    double level {};
    Colour colour { 1, 0, 0 };

    // Use drunken ant algorithm to generate points; after generating every group
    // of 3, create a triangle.
    while (leaves.size() < n_leaves)
    {
        while (vertices.size() < 3) {
            // create next point by adding velocity to current point
            double pt_x { current_pt.X() + velocity[0] },
                pt_z { current_pt.Z() + velocity[1] };
            // adjust point so it lies within the field boundaries
            if (pt_x < -width_magnitude) {
                pt_x = -width_magnitude + 1e-6;
            }
            else if (pt_x > width_magnitude) {
                pt_x = width_magnitude - 1e-6;
            }
            if (pt_z < -height_magnitude) {
                pt_z = -height_magnitude + 1e-6;
            }
            else if (pt_z > height_magnitude) {
                pt_z = height_magnitude - 1e-6;
            }
            Point next_pt { pt_x, 0, pt_z };
            vertices.push_back(next_pt);
            double magnitude { std::sqrt(velocity[0]*velocity[0] + velocity[1]*velocity[1]) };
            // adjust overall length by magnitude of velocity
            current_length += magnitude;
            current_pt = next_pt;
            // rotate velocity by angular acceleration
            double x { velocity[0] };
            double z { velocity[1] };
            double cos_angular { std::cos(angular) };
            double sin_angular { std::sin(angular) };
            // scale velocity so its magnitude is current magnitude + acceleration
            double velocity_scale { (magnitude + acceleration) / magnitude };
            velocity[0] = (x * cos_angular - z * sin_angular) * velocity_scale;
            velocity[1] = (z * cos_angular + x * sin_angular) * velocity_scale;
            acceleration += accel_urd(mt_engine_) * n_drinks;
            angular += urd_(mt_engine_);
        }
        Triangle *leaf {};
        try {
            // instantiation could fail if the vertices don't actually describe a triangle
            leaf = new Triangle(vertices[0], vertices[1], vertices[2]);
            leaves.push_back(leaf);
        }
        catch (std::invalid_argument e) {
            if (leaf != nullptr) {
                delete leaf;
            }
        }
        vertices.clear();
    }

    // arrange leaves so biggest ones sink to the bottom
    std::sort(leaves.begin(), leaves.end(), sort_triangle_by_area);
    ShapeGroup group {};
    for (Triangle *leaf: leaves) {
        leaf->SetTransform(
            Transformation()
            .Translate(0, -leaf->Area() * 1e-3, 0)
            .RotateX(-M_PI/12)
        );
        leaf->SetMaterial(
            Material()
            .Surface(colour)
            .CastsShadow(false)
            .Specular(0.5)
        );
        colour = GetNewColour(colour, 0.08);
        group.Add(leaf);
    }
    group.Divide(50);
    world.Add(&group);

    // Add a plane as lowest object in the scene
    double bottom { leaves[0]->Area() };
    Plane floor {};
    floor.SetTransform(
        Transformation()
        .Translate(0, -bottom -1, 0)
    );
    floor.SetMaterial(
        Material()
        .Surface(Colour { 0.3, 1, 0.3 })
        .CastsShadow(false)
    );
    world.Add(&floor);

    // Add a second light within the mass of leaves for extra definition
    Light submerged { Point(-10*scale, -scale, -10*scale), Colour(1, 1, 1) };
    world.Add(&submerged);

    Camera camera = SceneCamera(scale, width, height, k60DegreesRad, CameraTransform(scale));
    Canvas canvas = camera.Render(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;

    // clean up heap
    while (!leaves.empty()) {
        Triangle *leaf = leaves.back();
        leaves.pop_back();
        delete leaf;
    }

    return 0;
}