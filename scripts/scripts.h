#ifndef RAY_TRACER_SCRIPTS_H
#define RAY_TRACER_SCRIPTS_H

#include <iostream>

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

#endif