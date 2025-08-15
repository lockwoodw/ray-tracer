#define _USE_MATH_DEFINES // for M_PI
#include <cmath>

#include "sphere.h"
#include "transformations.h"
#include "material.h"

Material FloorMaterial() {
    Material material {};
    Colour colour { 1, 0.9, 0.9 };
    material.Surface(colour);
    material.Specular(0);
    return material;
}

Sphere LargeSphere(double scale) {
    Sphere large_sphere {};
    Matrix transform = Transformation()
        .Scale(scale, scale, scale)
        .Translate(-0.5 * scale, scale, 0.5 * scale);
    large_sphere.SetTransform(transform);
    Material material {};
    Colour colour { 0.1, 1, 0.5 };
    material.Surface(colour);
    material.Diffuse(0.7);
    material.Specular(0.3);
    large_sphere.SetMaterial(material);
    return large_sphere;
}

Sphere SmallerSphere(double scale) {
    Sphere sphere;
    double halved = scale * 0.5;
    Matrix transform = Transformation()
        .Scale(halved, halved, halved)
        .Translate(1.5 * scale, halved, -halved);
    sphere.SetTransform(transform);
    Material material {};
    Colour colour { 0.5, 1, 0.1 };
    material.Surface(colour);
    material.Diffuse(0.7);
    material.Specular(0.3);
    sphere.SetMaterial(material);
    return sphere;
}

Sphere SmallestSphere(double scale) {
    Sphere sphere;
    double third = scale / 3;
    Matrix transform = Transformation()
        .Scale(third, third, third)
        .Translate(-1.5 * scale, third, -0.75 * scale);
    sphere.SetTransform(transform);
    Material material {};
    Colour colour { 1, 0.8, 0.1 };
    material.Surface(colour);
    material.Diffuse(0.7);
    material.Specular(0.3);
    sphere.SetMaterial(material);
    return sphere;
}

Light WorldLight(double scale) {
    double scaled = 10 * scale;
    Point origin { -scaled, scaled, -scaled };
    Colour colour { 1, 1, 1 };
    return Light { origin, colour };
}

Matrix CameraTransform(double scale) {
    Point from { 0, 1.5 * scale, -5 * scale }, to { 0, scale, 0 };
    Vector up { 0, 1, 0 };
    return ViewTransform { from, to, up };
}