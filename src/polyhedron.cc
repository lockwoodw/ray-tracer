#include <vector>
#include <algorithm>
#include <cmath>

#include "polyhedron.h"
#include "triangle.h"
#include "sheet.h"

PolyhedronFactory::~PolyhedronFactory() {
    std::for_each(objects_.begin(), objects_.end(),
        [] (Polyhedron polyhedron) {
            polyhedron->Clear();
            delete polyhedron;
        });
}

Polyhedron PolyhedronFactory::Tetrahedron() {
    Polyhedron tetrahedron = new ShapeGroup();
    objects_.push_back(tetrahedron);

    // base of equilateral tetrahedron with side length 2 lying in the x-z plane
    Triangle *base = new Triangle(Point(0, 0, 1), Point(-1, 0, 0), Point(1, 0, 0));

    // calculate location of apex: the point located directly above the base's centroid
    // the height of which (y co-ord) is given by (side_length ÷ √6)
    double side_length = (base->P3() - base->P2()).At(0);
    Point apex { 0, side_length / std::sqrt(6), base->Centroid().Z() };

    Triangle *side1 = new Triangle(apex, Point(-1, 0, 0), Point(1, 0, 0));
    Triangle *side2 = new Triangle(apex, Point(-1, 0, 0), Point(0, 0, 1));
    Triangle *side3 = new Triangle(apex, Point(1, 0, 0), Point(0, 0, 1));

    *tetrahedron << base << side1 << side2 << side3;

    return tetrahedron;
}

Polyhedron PolyhedronFactory::Pyramid() {
    Polyhedron pyramid = new ShapeGroup();
    objects_.push_back(pyramid);

    Sheet *base = new Sheet(2, 2);

    // height for a pyramid containing equilateral triangles on a square base
    double height = 2 / std::sqrt(2);
    Point apex { 0, height, 0 };

    Triangle *side1 = new Triangle(apex, Point(-1, 0, -1), Point(1, 0, -1));
    Triangle *side2 = new Triangle(apex, Point(1, 0, -1), Point(1, 0, 1));
    Triangle *side3 = new Triangle(apex, Point(1, 0, 1), Point(-1, 0, 1));
    Triangle *side4 = new Triangle(apex, Point(-1, 0, 1), Point(-1, 0, -1));

    *pyramid << base << side1 << side2 << side3 << side4;

    return pyramid;
}

Polyhedron PolyhedronFactory::Octahedron() {
    Polyhedron octahedron = new ShapeGroup();
    objects_.push_back(octahedron);

    // height for a pyramid containing equilateral triangles on a square base
    double height = 2 / std::sqrt(2);
    Point apexes[] {
        { 0, height, 0 },
        { 0, -height, 0 }
    };

    for (auto apex: apexes) {
        Triangle *side1 = new Triangle(apex, Point(-1, 0, -1), Point(1, 0, -1));
        Triangle *side2 = new Triangle(apex, Point(1, 0, -1), Point(1, 0, 1));
        Triangle *side3 = new Triangle(apex, Point(1, 0, 1), Point(-1, 0, 1));
        Triangle *side4 = new Triangle(apex, Point(-1, 0, 1), Point(-1, 0, -1));

        *octahedron << side1 << side2 << side3 << side4;
    }

    return octahedron;
}

Polyhedron PolyhedronFactory::Pentagon() {
    // Construct a regular pentagon oriented about the origin lying in the x-z plane
    Polyhedron pentagon = new ShapeGroup();
    objects_.push_back(pentagon);

    // Vertex calculation from
    // Weisstein, Eric W. "Regular Pentagon." From MathWorld--A Wolfram Resource.
    // https://mathworld.wolfram.com/RegularPentagon.html
    double
        sqrt_5 { std::sqrt(5) },
        z1 { 0.25 * (sqrt_5 - 1) },
        z2 { 0.25 * (sqrt_5 + 1) },
        x1 { 0.25 * std::sqrt(10 + 2 * sqrt_5) },
        x2 { 0.25 * std::sqrt(10 - 2 * sqrt_5) };

    Triangle *t1 = new Triangle(Point(0, 0, 1), Point(-x1, 0, z1), Point(0, 0, 0));
    Triangle *t2 = new Triangle(Point(-x1, 0, z1), Point(-x2, 0, -z2), Point(0, 0, 0));
    Triangle *t3 = new Triangle(Point(-x2, 0, -z2), Point(x2, 0, -z2), Point(0, 0, 0));
    Triangle *t4 = new Triangle(Point(x2, 0, -z2), Point(x1, 0, z1), Point(0, 0, 0));
    Triangle *t5 = new Triangle(Point(x1, 0, z1), Point(0, 0, 1), Point(0, 0, 0));

    *pentagon << t1 << t2 << t3 << t4 << t5;
    return pentagon;
}

Polyhedron PolyhedronFactory::Dodecahedron() {
    // Construction adapted from
    // https://en.wikipedia.org/wiki/Regular_dodecahedron#Relation_to_the_golden_ratio
    Polyhedron dodecahedron = new ShapeGroup();
    objects_.push_back(dodecahedron);

    // vertices given by golden ratio ϕ = (1 + √5) / 2
    // (±1, ±1, ±1), (0, ±ϕ, ±1/ϕ), (±1/ϕ, 0, ±ϕ), (±ϕ, ±1/ϕ, 0)
    double phi { (1 + std::sqrt(5)) / 2 },
           phi_inverse { 1 / phi };

    Point
        // the "orange" vertices lie at (±1, ±1, ±1)
        o1 { -1, -1, -1 }, o2 { -1, 1, -1 }, o3 { 1, 1, -1 }, o4 { 1, -1, -1 },
        o5 { -1, -1, 1 }, o6 { -1, 1, 1 }, o7 { 1, 1, 1 }, o8 { 1, -1, 1 },
        // the "green" vertices lie at (0, ±ϕ, ±⁠1/ϕ⁠) and form a rectangle on the yz-plane
        g1 { 0, -phi_inverse, -phi }, g2 { 0, phi_inverse, -phi },
            g3 { 0, phi_inverse, phi }, g4 { 0, -phi_inverse, phi },
        // the "blue" vertices lie at (±⁠1/ϕ⁠, 0, ±ϕ) and form a rectangle on the xy-plane
        b1 { -phi_inverse, -phi, 0 }, b2 { -phi_inverse, phi, 0 },
            b3 { phi_inverse, phi, 0 }, b4 { phi_inverse, -phi, 0 },
        // the "pink" vertices lie at (±ϕ, ±⁠1/ϕ⁠, 0) and form a rectangle on the xz-plane
        p1 { -phi, 0, -phi_inverse }, p2 { -phi, 0, phi_inverse },
            p3 { phi, 0, phi_inverse }, p4 { phi, 0, -phi_inverse };

    // Construct each face as three triangles that fan out from a common root
    Point roots[] = {
        // visible faces
        o3, p4, o1, p1, o2, g2,
        // hidden faces
        p1, o2, b2, g3, o5, p2 };

    // Define the remaining points for the 3 triangles (per face) that share a root
    size_t depth {  sizeof(roots) / sizeof(roots[0]) }, rows {3}, columns {2};
    Point vertices[depth][rows][columns] = {
        // Visible
        {
            { b3, o7 }, { o7, p3 }, { p3, p4 } // o3
        },
        {
            { p3, o8 }, { o8, b4 }, { b4, o4 } // p4
        },
        {
            { g1, o4 }, { o4, b4 }, { b4, b1 } // o1
        },
        {
            { o2, g2 }, { g2, g1 }, { g1, o1 } // p1
        },
        {
            { b2, b3 }, { b3, o3 }, { o3, g2 } // o2
        },
        {
            { o3, p4 }, { p4, o4 }, { o4, g1 } // g2
        },
        // Hidden
        {
            { p2, o5 }, { o5, b1 }, { b1, o1 } // p1
        },
        {
            { b2, o6 }, { o6, p2 }, { p2, p1 } // o2
        },
        {
            { b3, o7 }, { o7, g3 }, { g3, o6 } // b2
        },
        {
            { o7, p3 }, { p3, o8 }, { o8, g4 } // g3
        },
        {
            { g4, o8 }, { o8, b4 }, { b4, b1 } // o5
        },
        {
            { o6, g3 }, { g3, g4 }, { g4, o5 } // p2
        }
    };

    // For each root, combine the three triangles to form a pentagonal face
    for (size_t d = 0; d < depth; d++) {
        Point root = roots[d];
        for (size_t r = 0; r < rows; r++) {
            auto row = vertices[d][r];
            Triangle *t = new Triangle(root, row[0], row[1]);
            *dodecahedron << t;
        }
    }

    return dodecahedron;
}
