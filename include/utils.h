#ifndef RAY_TRACER_UTILS_H
#define RAY_TRACER_UTILS_H

static const double kAbsEpsilon = 1e-12;
static const double kRelEpsilon = 1e-8;

bool floating_point_relative_compare(double a, double b, double rel_epsilon = kRelEpsilon);
bool floating_point_compare(double a, double b, double abs_epsilon = kAbsEpsilon, double rel_epsilon = kRelEpsilon);

#endif