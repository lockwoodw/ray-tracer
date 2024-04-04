#ifndef RAY_TRACER_MATERIAL_H
#define RAY_TRACER_MATERIAL_H

#include "space.h"
#include "colour.h"

class Light {
    Point position_;
    Colour intensity_;

    public:
        Light(const Point& p, const Colour& c): position_ { p }, intensity_ { c } {}
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

        bool operator==(const Material& m) const;

        Colour Surface() const { return surface_; }
        double Ambient() const { return ambient_; }
        double Diffuse() const { return diffuse_; }
        double Specular() const { return specular_; }
        double Shininess() const { return shininess_; }

        void Surface(const Colour& c) { surface_ = c; }
        void Ambient(double a) { ambient_ = a; }
        void Diffuse(double d) { diffuse_ = d; }
        void Specular(double s) { specular_ = s; }
        void Shininess(double s) { shininess_ = s; }

        Colour ApplyLightAt(const Light& light, const Point& point,
            const Vector& eye_vector, const Vector& normal_vector) const;
};

#endif