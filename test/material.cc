#include "material.h"

#include <gtest/gtest.h>
#include <cmath>

#include "utils.h"
#include "colour.h"
#include "shape.h"
#include "space.h"
#include "pattern.h"

/*
Scenario: A point light has a position and intensity
  Given intensity ← color(1, 1, 1)
    And position ← point(0, 0, 0)
  When light ← point_light(position, intensity)
  Then light.position = position
    And light.intensity = intensity
*/

TEST(MaterialTest, ConfirmingThePropertiesOfALight) {
    Colour intensity { 1, 1, 1 };
    Point position { 0, 0, 0 };
    Light light { position, intensity };
    ASSERT_EQ(light.Position(), position);
    ASSERT_EQ(light.Intensity(), intensity);
}

/*
Scenario: The default material
  Given m ← material()
  Then m.color = color(1, 1, 1)
    And m.ambient = 0.1
    And m.diffuse = 0.9
    And m.specular = 0.9
    And m.shininess = 200.0
*/

TEST(MaterialTest, ConfirmingTheDefaultMaterial) {
    Material m {};
    Colour expected { 1, 1, 1 };
    ASSERT_EQ(m.Surface(), expected);
    ASSERT_DOUBLE_EQ(m.Ambient(), 0.1);
    ASSERT_DOUBLE_EQ(m.Diffuse(), 0.9);
    ASSERT_DOUBLE_EQ(m.Specular(), 0.9);
    ASSERT_DOUBLE_EQ(m.Shininess(), 200.0);
}

/*
Scenario: Reflectivity for the default material
  Given m ← material()
  Then m.reflective = 0.0
*/

/*
Scenario: Transparency and Refractive Index for the default material
  Given m ← material()
  Then m.transparency = 0.0
    And m.refractive_index = 1.0
*/

/*
Scenario: Lighting with the eye between the light and the surface
  Given eyev ← vector(0, 0, -1)
    And normalv ← vector(0, 0, -1)
    And light ← point_light(point(0, 0, -10), color(1, 1, 1))
  When result ← lighting(m, light, position, eyev, normalv)
  Then result = color(1.9, 1.9, 1.9)
*/

TEST(MaterialTest, LightingWithTheEyeBetweenTheLightAndTheSurface) {
    Material m {};
    Point position { 0, 0, 0 };
    Vector eye_vector { 0, 0, -1 },
           normal_vector { 0, 0, -1 };
    Light light { Point { 0, 0, -10 }, Colour { 1, 1, 1 }};
    Colour expected { 1.9, 1.9, 1.9 };
    ASSERT_EQ(m.ApplyLightAt(light, position, eye_vector, normal_vector), expected);
}

/*
Scenario: Lighting with the eye between light and surface, eye offset 45°
  Given eyev ← vector(0, √2/2, -√2/2)
    And normalv ← vector(0, 0, -1)
    And light ← point_light(point(0, 0, -10), color(1, 1, 1))
  When result ← lighting(m, light, position, eyev, normalv)
  Then result = color(1.0, 1.0, 1.0)
*/

TEST(MaterialTest, LightingWithTheEyeOffset45Degrees) {
    Material m {};
    Point position { 0, 0, 0 };
    double d { std::sqrt(2) / 2 };
    Vector eye_vector { 0, d, -d },
           normal_vector { 0, 0, -1 };
    Light light { Point { 0, 0, -10 }, Colour { 1, 1, 1 }};
    Colour expected { 1.0, 1.0, 1.0 };
    ASSERT_EQ(m.ApplyLightAt(light, position, eye_vector, normal_vector), expected);
}

/*
Scenario: Lighting with eye opposite surface, light offset 45°
  Given eyev ← vector(0, 0, -1)
    And normalv ← vector(0, 0, -1)
    And light ← point_light(point(0, 10, -10), color(1, 1, 1))
  When result ← lighting(m, light, position, eyev, normalv)
  Then result = color(0.7364, 0.7364, 0.7364)
*/

TEST(MaterialTest, LightingWithTheEyeOppositeSurface) {
    Material m {};
    Point position { 0, 0, 0 };
    Vector eye_vector { 0, 0, -1 },
           normal_vector { 0, 0, -1 };
    Light light { Point { 0, 10, -10 }, Colour { 1, 1, 1 }};
    Colour expected { 0.7364, 0.7364, 0.7364 };
    Colour result = m.ApplyLightAt(light, position, eye_vector, normal_vector);
    // Compare colour constituents individually using comparator with smaller Epsilon
    ASSERT_TRUE(simple_floating_point_compare(expected.Red(), result.Red()));
    ASSERT_TRUE(simple_floating_point_compare(expected.Green(), result.Green()));
    ASSERT_TRUE(simple_floating_point_compare(expected.Blue(), result.Blue()));
}

/*
Scenario: Lighting with eye in the path of the reflection vector
  Given eyev ← vector(0, -√2/2, -√2/2)
    And normalv ← vector(0, 0, -1)
    And light ← point_light(point(0, 10, -10), color(1, 1, 1))
  When result ← lighting(m, light, position, eyev, normalv)
  Then result = color(1.6364, 1.6364, 1.6364)
*/

TEST(MaterialTest, LightingWithTheEyeInThePathOfTheReflectionVector) {
    Material m {};
    Point position { 0, 0, 0 };
    double d { std::sqrt(2) / 2 };
    Vector eye_vector { 0, -d, -d },
           normal_vector { 0, 0, -1 };
    Light light { Point { 0, 10, -10 }, Colour { 1, 1, 1 }};
    Colour expected { 1.6364, 1.6364, 1.6364 };
    Colour result = m.ApplyLightAt(light, position, eye_vector, normal_vector);
    // Compare colour constituents individually using comparator with smaller Epsilon
    ASSERT_TRUE(simple_floating_point_compare(expected.Red(), result.Red()));
    ASSERT_TRUE(simple_floating_point_compare(expected.Green(), result.Green()));
    ASSERT_TRUE(simple_floating_point_compare(expected.Blue(), result.Blue()));
}

/*
Scenario: Lighting with the light behind the surface
  Given eyev ← vector(0, 0, -1)
    And normalv ← vector(0, 0, -1)
    And light ← point_light(point(0, 0, 10), color(1, 1, 1))
  When result ← lighting(m, light, position, eyev, normalv)
  Then result = color(0.1, 0.1, 0.1)
*/

TEST(MaterialTest, LightingBehindTheSurface) {
    Material m {};
    Point position { 0, 0, 0 };
    Vector eye_vector { 0, 0, -1 },
           normal_vector { 0, 0, -1 };
    Light light { Point { 0, 0, 10 }, Colour { 1, 1, 1 }};
    Colour expected { 0.1, 0.1, 0.1 };
    ASSERT_EQ(m.ApplyLightAt(light, position, eye_vector, normal_vector), expected);
}

/*
Scenario: Lighting with the surface in shadow
  Given eyev ← vector(0, 0, -1)
    And normalv ← vector(0, 0, -1)
    And light ← point_light(point(0, 0, -10), color(1, 1, 1))
    And in_shadow ← true
  When result ← lighting(m, light, position, eyev, normalv, in_shadow)
  Then result = color(0.1, 0.1, 0.1)
*/

TEST(MaterialTest, LightingWithTheSurfaceInShadow) {
    Material m {};
    Point position { 0, 0, 0 };
    Vector eye_vector { 0, 0, -1 },
           normal_vector { 0, 0, -1 };
    Light light { Point { 0, 0, -10 }, Colour { 1, 1, 1 }};
    bool in_shadow { true };
    Colour expected { 0.1, 0.1, 0.1 };
    ASSERT_EQ(m.ApplyLightAt(light, position, eye_vector, normal_vector, in_shadow), expected);
}

/*
Scenario: Lighting with a pattern applied
  Given m.pattern ← stripe_pattern(color(1, 1, 1), color(0, 0, 0))
    And m.ambient ← 1
    And m.diffuse ← 0
    And m.specular ← 0
    And eyev ← vector(0, 0, -1)
    And normalv ← vector(0, 0, -1)
    And light ← point_light(point(0, 0, -10), color(1, 1, 1))
  When c1 ← lighting(m, light, point(0.9, 0, 0), eyev, normalv, false)
    And c2 ← lighting(m, light, point(1.1, 0, 0), eyev, normalv, false)
  Then c1 = color(1, 1, 1)
    And c2 = color(0, 0, 0)
*/

TEST(MaterialTest, LightingWithAPatternApplied) {
    Material m {};
    Colour white { 1, 1, 1 },
           black { 0, 0, 0 };
    StripePattern sp { white, black };
    m.SurfacePattern(&sp).Ambient(1).Diffuse(0).Specular(0);
    Vector eye { 0, 0, -1 },
           normal { 0, 0, -1 };
    Light light { Point { 0, 0, -10 }, white };
    Colour c1 = m.ApplyLightAt(light, Point { 0.9, 0, 0 }, eye, normal, false),
           c2 = m.ApplyLightAt(light, Point { 1.1, 0, 0 }, eye, normal, false);
    ASSERT_EQ(c1, white);
    ASSERT_EQ(c2, black);
}