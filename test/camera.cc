#define _USE_MATH_DEFINES // for M_PI
#include <gtest/gtest.h>
#include <cmath>
#include "camera.h"
#include "matrix.h"
#include "ray.h"
#include "space.h"
#include "utils.h"
#include "transformations.h"
#include "sphere.h"
#include "material.h"
#include "canvas.h"

/*
Scenario: Constructing a camera
  Given hsize ← 160
    And vsize ← 120
    And field_of_view ← π/2
  When c ← camera(hsize, vsize, field_of_view)
  Then c.hsize = 160
    And c.vsize = 120
    And c.field_of_view = π/2
    And c.transform = identity_matrix
*/

TEST(CameraTest, ConstructingACamera) {
    int hsize { 160 }, vsize { 120 };
    double field_of_view { M_PI / 2 };
    Camera c { hsize, vsize, field_of_view };
    ASSERT_EQ(c.Horizontal(), hsize);
    ASSERT_EQ(c.Vertical(), vsize);
    ASSERT_DOUBLE_EQ(c.FieldOfView(), field_of_view);
    ASSERT_EQ(c.Transform(), Matrix::Identity(4));
}

/*
Scenario: The pixel size for a horizontal canvas
  Given c ← camera(200, 125, π/2)
  Then c.pixel_size = 0.01
*/

TEST(CameraTest, ConfirmingPixelSizeForAHorizontalCanvas) {
    Camera c { 200, 125, M_PI / 2 };
    ASSERT_DOUBLE_EQ(c.PixelSize(), 0.01);
}

/*
Scenario: The pixel size for a vertical canvas
  Given c ← camera(125, 200, π/2)
  Then c.pixel_size = 0.01
*/

TEST(CameraTest, ConfirmingPixelSizeForAVerticalCanvas) {
    Camera c { 125, 200, M_PI / 2 };
    ASSERT_DOUBLE_EQ(c.PixelSize(), 0.01);
}

/*
Scenario: Constructing a ray through the center of the canvas
  Given c ← camera(201, 101, π/2)
  When r ← ray_for_pixel(c, 100, 50)
  Then r.origin = point(0, 0, 0)
    And r.direction = vector(0, 0, -1)
*/

TEST(CameraTest, ConstructingARayThroughTheCenterOfTheCanvas) {
    Camera c { 201, 101, M_PI / 2 };
    Ray r = c.RayAt(100, 50);
    Point expected_origin { 0, 0, 0 };
    Vector expected_direction { 0, 0, -1 };
    ASSERT_EQ(r.Origin(), expected_origin);
    ASSERT_EQ(r.Direction(), expected_direction);
}

/*
Scenario: Constructing a ray through a corner of the canvas
  Given c ← camera(201, 101, π/2)
  When r ← ray_for_pixel(c, 0, 0)
  Then r.origin = point(0, 0, 0)
    And r.direction = vector(0.66519, 0.33259, -0.66851)
*/

TEST(CameraTest, ConstructingARayThroughACornerOfTheCanvas) {
    Camera c { 201, 101, M_PI / 2 };
    Ray r = c.RayAt(0, 0);
    Point expected_origin { 0, 0, 0 };
    Vector expected_direction { 0.66519, 0.33259, -0.66851 };
    ASSERT_EQ(r.Origin(), expected_origin);
    ASSERT_TRUE(simple_floating_point_compare(r.Direction().X(), expected_direction.X()));
    ASSERT_TRUE(simple_floating_point_compare(r.Direction().Y(), expected_direction.Y()));
    ASSERT_TRUE(simple_floating_point_compare(r.Direction().Z(), expected_direction.Z()));
}

/*
Scenario: Constructing a ray when the camera is transformed
  Given c ← camera(201, 101, π/2)
  When c.transform ← rotation_y(π/4) * translation(0, -2, 5)
    And r ← ray_for_pixel(c, 100, 50)
  Then r.origin = point(0, 2, -5)
    And r.direction = vector(√2/2, 0, -√2/2)
*/

TEST(CameraTest, ConstructingARayWhenTheCameraIsTransformed) {
    Camera c { 201, 101, M_PI / 2 };
    Matrix transform = Transformation().Translate(0, -2, 5).RotateY(M_PI / 4);
    c.SetTransform(transform);
    Ray r = c.RayAt(100, 50);
    Point expected_origin { 0, 2, -5 };
    double value = std::sqrt(2) / 2;
    Vector expected_direction { value, 0, -value };
    ASSERT_EQ(r.Origin(), expected_origin);
    ASSERT_EQ(r.Direction(), expected_direction);
}

/*
Scenario: Rendering a world with a camera
  Given w ← default_world()
    And c ← camera(11, 11, π/2)
    And from ← point(0, 0, -5)
    And to ← point(0, 0, 0)
    And up ← vector(0, 1, 0)
    And c.transform ← view_transform(from, to, up)
  When image ← render(c, w)
  Then pixel_at(image, 5, 5) = color(0.38066, 0.47583, 0.2855)
*/

TEST(CameraTest, RenderingAWorldWithACamera) {
    // Set up world, see p. 92
    World default_world {};

    Point position { -10, 10, -10 };
    Colour intensity { 1, 1, 1 };
    Light light { position, intensity };
    default_world.Add(&light);

    Sphere sphere1 {};
    Colour c1 { 0.8, 1.0, 0.6 };
    Material material1 { c1, 0.1, 0.7, 0.2, 200.0 };
    sphere1.SetMaterial(material1);
    default_world.Add(&sphere1);

    Sphere sphere2 {};
    Matrix t2 = Transformation().Scale(0.5, 0.5, 0.5);
    sphere2.SetTransform(t2);
    default_world.Add(&sphere2);

    Camera c { 11, 11, M_PI / 2 };

    Point from { 0, 0, -5 },
          to { 0, 0, 0 };
    Vector up { 0, 1, 0 };
    ViewTransform vt { from, to, up };
    c.SetTransform(vt);

    Canvas image = c.Render(default_world);
    Colour expected { 0.38066, 0.47583, 0.2855 }, actual = image.At(5, 5);
    ASSERT_TRUE(simple_floating_point_compare(actual.Red(), expected.Red()));
    ASSERT_TRUE(simple_floating_point_compare(actual.Green(), expected.Green()));
    ASSERT_TRUE(simple_floating_point_compare(actual.Blue(), expected.Blue()));
}