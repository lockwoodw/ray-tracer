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
    Colour colour_;
    double ambient_;
    double diffuse_;
    double specular_;
    double shininess_;

    public:
        Material():
            colour_ { Colour { 1, 1, 1 } },
            ambient_ { 0.1 },
            diffuse_ { 0.9 },
            specular_ { 0.9 },
            shininess_ { 200.0 } {}

        Material(Colour colour, double ambient, double diffuse, double specular,
            double shininess):
            colour_ { colour },
            ambient_ { ambient },
            diffuse_ { diffuse },
            specular_ { specular },
            shininess_ { shininess} {}

        bool operator==(const Material& m) const;

        Colour MaterialColour() const { return colour_; }
        double Ambient() const { return ambient_; }
        double Diffuse() const { return diffuse_; }
        double Specular() const { return specular_; }
        double Shininess() const { return shininess_; }

        void SetColour(const Colour& c) { colour_ = c; }
        void SetAmbient(double a) { ambient_ = a; }
        void SetDiffuse(double d) { diffuse_ = d; }
        void SetSpecular(double s) { specular_ = s; }
        void SetShininess(double s) { shininess_ = s; }

        Colour ApplyLightAt(const Light& light, const Point& point,
            const Vector& eye_vector, const Vector& normal_vector) const;
};

#endif