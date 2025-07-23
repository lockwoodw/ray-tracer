#include "material.h"

#include <cmath>

#include "utils.h"
#include "pattern.h"

bool Material::operator==(const Material& m) const {
    bool pattern_exists = PatternExists(),
         m_pattern_exists = m.PatternExists();
    return surface_ == m.surface_
        && (
            (pattern_exists && m_pattern_exists && *pattern_ == *m.pattern_)
            || (!pattern_exists && !m_pattern_exists)
        )
        && floating_point_compare(ambient_, m.ambient_)
        && floating_point_compare(diffuse_, m.diffuse_)
        && floating_point_compare(specular_, m.specular_)
        && floating_point_compare(shininess_, m.shininess_)
        && floating_point_compare(reflectivity_, m.reflectivity_)
        && floating_point_compare(transparency_, m.transparency_)
        && floating_point_compare(refractive_index_, m.refractive_index_)
        && (casts_shadow_ == m.casts_shadow_);
}

Colour Material::ApplyLightAt(const Shape* object, const Light& light, const Point& point,
        const Vector& eye_vector, const Vector& normal_vector, bool in_shadow) const
{
    // Apply colour from pattern, if any
    Colour colour = PatternExists() ? pattern_->ObjectColourAt(object, point) : surface_;

    // Combine the surface colour with the light's colour/intensity
    Colour effective = colour * light.Intensity();

    // Effective ambient colour
    Colour ambient = effective * ambient_;

    // Ignore diffuse and specular contributions if the point is in shadow
    if (in_shadow) {
        return ambient;
    }

    // Find the direction of the light
    Vector light_vector = (Vector { light.Position() - point } ).Normalize();

    // The dot-product of the light and normal vectors represents the cosine of the
    // angle between them.
    double light_normal = Vector::DotProduct(light_vector, normal_vector);

    // If the dot-product is negative, the light is on the other side of the surface
    Colour black { 0, 0, 0 };
    Colour diffuse = black;
    Colour specular = black;

    // Otherwise, compute the diffuse value
    if (light_normal >= 0) {
        diffuse = effective * diffuse_ * light_normal;

        Vector reflection = Vector::Reflect(-light_vector, normal_vector);

        // The dot-product of the reflection and eye vectors represents the
        // cosine of the angle between them.
        double reflection_eye = Vector::DotProduct(reflection, eye_vector);

        // If the dot-product is negative, the light reflects away from the eye,
        // so there is no specular value
        if (reflection_eye > 0) {
            double factor = std::pow(reflection_eye, shininess_);
            specular = light.Intensity() * specular_ * factor;
        }
    }

    return ambient + diffuse + specular;
}