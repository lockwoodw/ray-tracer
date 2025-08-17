#ifndef RAY_TRACER_CHALLENGES_H
#define RAY_TRACER_CHALLENGES_H

#include <iostream>

#include "colour.h"
#include "material.h"

static double kMaxScale { 20.0 };

double GetScale(int argc, char** argv) {
    double scale { 1.0 };

    if (argc > 1) {
        scale = atof(argv[1]);
    }

    if (scale <= 0 || scale > kMaxScale) {
        std::cerr << "Given scale invalid (1-" << kMaxScale << ")" << std::endl;
        exit(-1);
    }
    return scale;
}

Material GlassMarbleMaterial(const Colour& colour) {
    return Material()
        .Transparency(1.0)
        .RefractiveIndex(1.52)
        .Diffuse(0.1)
        .Ambient(0.1)
        .Reflectivity(1.0)
        .Shininess(300)
        .Specular(1)
        .Surface(colour);
}

Material AirBubbleMaterial() {
    return Material()
        .Transparency(1.0)
        .RefractiveIndex(1.00029)
        .Diffuse(0)
        .Ambient(0)
        .Specular(0)
        .Reflectivity(0)
        .Shininess(10)
        .CastsShadow(false);
}

#endif