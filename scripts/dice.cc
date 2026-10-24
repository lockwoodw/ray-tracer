/*
Render the same scene as The Ray Tracer Challenge: Chapter 15, but change the
shape material to glass and replace the world lights with lights inside the shapes.

Supply a scaling factor at the command line to increase the image dimensions.
*/

#include "scripts.h"
#include "triangle.h"
#include "plane.h"
#include "polyhedron.h"

Matrix CameraTransform(double scale) {
    Point from { 5 * scale, 4.5 * scale, -10 * scale }, to { 0, scale, 0 };
    Vector up { 0, 1, 0 };
    return ViewTransform { from, to, up };
}

int main(int argc, char** argv) {
    double scale = GetScale(argc, argv);

    World world {};

    PolyhedronFactory factory {};

    Polyhedron tetrahedron = factory.Tetrahedron();
    // Move tetrahedron so the centroid of its base is at (0, 0, 0), then Rotate etc.
    const Triangle *base = dynamic_cast<const Triangle*>((*tetrahedron)[0]);
    double tetrahedron_scale = 1.5 * scale;
    double x_offset { -3 }, z_offset { -1 };
    tetrahedron->SetTransform(
        Transformation()
        .Translate(0, 0, -base->Centroid().Z())
        .RotateY(M_PI/9)
        .Translate(x_offset, 0, z_offset)
        .Scale(tetrahedron_scale)
    );
    tetrahedron->SetMaterial(GlassMaterial(Colour(0, 0.8, 1)));
    world.Add(tetrahedron);

    // Add light at center of tetrahedron
    BoundingBox bbox = tetrahedron->BoundsOf();
    Point tetrahedron_center { x_offset * tetrahedron_scale,
        (bbox.Max().Y() / 2) * tetrahedron_scale, -z_offset * tetrahedron_scale };
    Light tetrahedron_light { tetrahedron_center, tetrahedron->ShapeMaterial().Surface() };
    world.Add(&tetrahedron_light);

    double pyramid_height = 2 / std::sqrt(2);
    z_offset = -2.75;
    Polyhedron pyramid = factory.Pyramid();
    pyramid->SetTransform(
        Transformation()
        .RotateY(-M_PI/6)
        .Translate(1, 0, z_offset)
        .Scale(scale)
    );
    pyramid->SetMaterial(GlassMaterial(Colour(1, 0.7, 0.2)));
    world.Add(pyramid);

    Point pyramid_center { scale, pyramid_height*scale/2, z_offset * scale };
    Light pyramid_light { pyramid_center, pyramid->ShapeMaterial().Surface() };
    world.Add(&pyramid_light);

    Polyhedron octahedron = factory.Octahedron();
    x_offset = -2;
    octahedron->SetTransform(
        Transformation()
        .RotateY(k45DegreesRad)
        .Translate(x_offset, pyramid_height, 0)
        .Scale(scale)
    );
    octahedron->SetMaterial(GlassMaterial(Colour(0.7, 0.1, 1)));
    world.Add(octahedron);

    Point octahedron_center { x_offset * scale,  pyramid_height*scale, 0 };
    Light octahedron_light { octahedron_center, octahedron->ShapeMaterial().Surface() };
    world.Add(&octahedron_light);

    Polyhedron dodecahedron = factory.Dodecahedron();
    double x_rot_offset { (180 - 116.56505) / 2 };
    double in_radius { std::sqrt(25 + 10 * std::sqrt(5)) / 10 };
    x_offset = z_offset = 1.5;
    double y_offset { 2 * in_radius };
    dodecahedron->SetTransform(
        Transformation()
        .RotateX(x_rot_offset*M_PI/180)
        .Translate(x_offset, y_offset, z_offset)
        .Scale(scale)
    );
    dodecahedron->SetMaterial(GlassMaterial(Colour(1, 0, 0)));
    world.Add(dodecahedron);

    Point dodecahedron_center { x_offset * scale, y_offset * scale, z_offset * scale };
    Light dodecahedron_light { dodecahedron_center, dodecahedron->ShapeMaterial().Surface() };
    world.Add(&dodecahedron_light);

    Plane ceiling {};
    ceiling.SetTransform(
        Transformation()
        .RotateZ(M_PI/24)
        .Translate(0, 5*scale, 0)
    );
    ceiling.SetMaterial(
        Material()
        .Surface(Colour(0.3, 0.3, 1))
    );
    world.Add(&ceiling);

    Polyhedron pentagon = factory.Pentagon();
    pentagon->SetTransform(
        Transformation()
        .Scale(20*scale)
        .Translate(-7*scale, -5*scale, 15*scale)
    );
    pentagon->SetMaterial(
        Material()
        .Reflectivity(0)
        .Shininess(0)
        .Specular(0)
        .Surface(Colour(0.3, 0.3, 0.3))
    );
    world.Add(pentagon);

    Camera camera = SceneCamera(scale, 108, 135, k60DegreesRad, CameraTransform(scale));
    Canvas canvas = camera.RenderConcurrent(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;

    return 0;
}
