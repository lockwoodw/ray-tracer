#ifndef RAY_TRACER_MATERIAL_H
#define RAY_TRACER_MATERIAL_H

#include "space.h"
#include "colour.h"

class Light {
    Point position_;
    Colour intensity_;

    public:
        Light(const Point& p, const Colour& c): position_ { p }, intensity_ { c } {}
        Light(const Light& light): position_ { light.position_ }, intensity_ { light.intensity_} {}
        const Point Position() const { return position_; }
        const Colour Intensity() const { return intensity_; }
};

class Material {
    Colour surface_;
    double ambient_;
    double diffuse_;
    double specular_;
    double shininess_;

    public:
        Material():
            surface_ { Colour { 1, 1, 1 } },
            ambient_ { 0.1 },
            diffuse_ { 0.9 },
            specular_ { 0.9 },
            shininess_ { 200.0 } {}

        Material(Colour surface, double ambient, double diffuse, double specular,
            double shininess):
            surface_ { surface },
            ambient_ { ambient },
            diffuse_ { diffuse },
            specular_ { specular },
            shininess_ { shininess} {}

        Material(const Material& m):
            surface_ { m.surface_ },
            ambient_ { m.ambient_ },
            diffuse_ { m.diffuse_ },
            specular_ { m.specular_ },
            shininess_ { m.shininess_ } {}

        bool operator==(const Material& m) const;

        Colour Surface() const { return surface_; }
        double Ambient() const { return ambient_; }
        double Diffuse() const { return diffuse_; }
        double Specular() const { return specular_; }
        double Shininess() const { return shininess_; }

        Material& Surface(const Colour& c) { surface_ = c; return *this; }
        Material& Ambient(double a) { ambient_ = a; return *this; }
        Material& Diffuse(double d) { diffuse_ = d; return *this; }
        Material& Specular(double s) { specular_ = s; return *this; }
        Material& Shininess(double s) { shininess_ = s; return *this; }

        Colour ApplyLightAt(const Light& light, const Point& point,
            const Vector& eye_vector, const Vector& normal_vector) const;
};

#endif