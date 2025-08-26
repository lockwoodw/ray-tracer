#ifndef RAY_TRACER_SCRIPTS_H
#define RAY_TRACER_SCRIPTS_H

#define _USE_MATH_DEFINES // for M_PI

#include <cmath>
#include <iostream>
#include <random>
#include <ctime>

#include "matrix.h"
#include "transformations.h"
#include "colour.h"
#include "material.h"
#include "world.h"
#include "camera.h"
#include "canvas.h"

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

Camera SceneCamera(double scale, int width, int height, double fov, const Matrix& view_transform) {
    int scale_int = static_cast<int>(scale);
    Camera camera { width * scale_int, height * scale_int, fov };
    camera.SetTransform(view_transform);
    return camera;
}

Material GlassMaterial(const Colour& colour = Colour::kWhite) {
    return Material()
        .Transparency(1.0)
        .RefractiveIndex(1.33333)
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

static std::mt19937_64 kMTEngine { static_cast<long unsigned int>(time(nullptr)) };

Colour GetNewColour(Colour c, double range = 0.5, double minimum = 0.2) {
    std::uniform_real_distribution<double> urd { -range, range };

    auto new_colour_pt = [minimum] (std::uniform_real_distribution<double> urd, double pt) {
        double offset = urd(kMTEngine),
               new_pt = pt + offset;
        return std::max(std::min(new_pt, 1.0), minimum);
    };

    std::set<int> remaining {
        Colour::kRed, Colour::kGreen, Colour::kBlue
    };

    double  red = c.Red(),
            green = c.Green(),
            blue = c.Blue(),
            max_colour = std::max(std::max(red, green), blue);

    int max_colour_index {};

    for (auto it = remaining.begin(); it != remaining.end(); it++) {
        if (simple_floating_point_compare(c.At(*it), max_colour)) {
            max_colour_index = *it;
            remaining.erase(it);
            break;
        }
    }
    double current_pt = c.At(max_colour_index),
           new_pt = new_colour_pt(urd, current_pt);
    double diff = std::fabs(current_pt - new_pt);
    c[max_colour_index] = new_pt;
    std::uniform_real_distribution<double> local_urd { -diff, diff };
    for (auto it = remaining.begin(); it != remaining.end(); it++) {
        c[*it] = new_colour_pt(urd, c[*it]);
    }
    return c;
}

#endif