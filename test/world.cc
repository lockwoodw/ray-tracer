#include <gtest/gtest.h>
#include <cmath>
#include "world.h"
#include "sphere.h"
#include "material.h"
#include "colour.h"
#include "transformations.h"
#include "ray.h"
#include "plane.h"
#include "pattern.h"

/*
Scenario: Creating a world
  Given w ← world()
  Then w contains no objects
    And w has no light source
*/

TEST(WorldTest, CreatingAWorld) {
    World w {};
    ASSERT_EQ(w.NObjects(), 0);
    ASSERT_EQ(w.NLights(), 0);
}

class DefaultWorldTest: public testing::Test {
    protected:
        World default_world_ {};
        Light* light_;
        Shape* sphere1_;
        Shape* sphere2_;
        Material material1_;
        Material material2_;

        void SetUp() override {
            Point position { -10, 10, -10 };
            Colour intensity { 1, 1, 1 };
            light_ = new Light(position, intensity);
            default_world_.Add(light_);

            sphere1_ = new Sphere();
            Colour c1 { 0.8, 1.0, 0.6 };
            material1_.Surface(c1).Ambient(0.1).Diffuse(0.7).Specular(0.2).Shininess(200.0);
            sphere1_->SetMaterial(material1_);
            default_world_.Add(sphere1_);

            sphere2_ = new Sphere();
            sphere2_->SetMaterial(material2_);
            Matrix t2 = Transformation().Scale(0.5, 0.5, 0.5);
            sphere2_->SetTransform(t2);
            default_world_.Add(sphere2_);
        }

        void TearDown() override {
            delete light_;
            delete sphere1_;
            delete sphere2_;
        }

    public:
        static const double kSqrt2;
        static const double kHalfSqrt2;
};

const double DefaultWorldTest::kSqrt2 = std::sqrt(2);
const double DefaultWorldTest::kHalfSqrt2 = DefaultWorldTest::kSqrt2 / 2;


const bool ColoursAreEqual(Colour& c1, Colour& c2, double epsilon=kEpsilon) {
    return simple_floating_point_compare(c1.Red(), c2.Red(), epsilon)
        && simple_floating_point_compare(c1.Green(), c2.Green(), epsilon)
        && simple_floating_point_compare(c1.Blue(), c2.Blue(), epsilon);
}

/*
Scenario: The default world
  Given light ← point_light(point(-10, 10, -10), color(1, 1, 1))
    And s1 ← sphere() with:
      | material.color     | (0.8, 1.0, 0.6)        |
      | material.diffuse   | 0.7                    |
      | material.specular  | 0.2                    |
    And s2 ← sphere() with:
      | transform | scaling(0.5, 0.5, 0.5) |
  When w ← default_world()
  Then w.light = light
    And w contains s1
    And w contains s2
*/

TEST_F(DefaultWorldTest, ConfirmingTheDefaultWorld) {
    ASSERT_TRUE(default_world_.Contains(light_));
    ASSERT_EQ(default_world_.NLights(), 1);
    ASSERT_TRUE(default_world_.Contains(sphere1_));
    ASSERT_TRUE(default_world_.Contains(sphere2_));
    ASSERT_EQ(default_world_.NObjects(), 2);
}

/*
Scenario: Intersect a world with a ray
  Given w ← default_world()
    And r ← ray(point(0, 0, -5), vector(0, 0, 1))
  When xs ← intersect_world(w, r)
  Then xs.count = 4
    And xs[0].t = 4
    And xs[1].t = 4.5
    And xs[2].t = 5.5
    And xs[3].t = 6
*/

TEST_F(DefaultWorldTest, IntersectingAWorldWithARay) {
    Point origin { 0, 0, -5 };
    Vector direction { 0, 0, 1 };
    Ray ray { origin, direction };
    IntersectionList xs = default_world_.Intersect(ray);
    double expected[] { 4, 4.5, 5.5, 6 };
    int size = sizeof(expected) / sizeof(double);
    ASSERT_EQ(xs.Size(), size);
    for (int i = 0; i < size; i++) {
        ASSERT_EQ(xs[i]->Distance(), expected[i]);
    }
}

/*
Scenario: Shading an intersection
  Given w ← default_world()
    And r ← ray(point(0, 0, -5), vector(0, 0, 1))
    And shape ← the first object in w
    And i ← intersection(4, shape)
  When comps ← prepare_computations(i, r)
    And c ← shade_hit(w, comps)
  Then c = color(0.38066, 0.47583, 0.2855)
*/

TEST_F(DefaultWorldTest, ShadingAnIntersection) {
    Point origin { 0, 0, -5 };
    Vector direction { 0, 0, 1 };
    Ray ray { origin, direction };
    Intersection i { 4, sphere1_ };
    IntersectionComputation ic { i, ray };
    Colour expected { 0.38066, 0.47583, 0.2855 },
           shade = default_world_.ColourAt(ic);
    ASSERT_TRUE(ColoursAreEqual(expected, shade));
}

/*
Scenario: Shading an intersection from the inside
  Given w ← default_world()
    And w.light ← point_light(point(0, 0.25, 0), color(1, 1, 1))
    And r ← ray(point(0, 0, 0), vector(0, 0, 1))
    And shape ← the second object in w
    And i ← intersection(0.5, shape)
  When comps ← prepare_computations(i, r)
    And c ← shade_hit(w, comps)
  Then c = color(0.90498, 0.90498, 0.90498)
*/

TEST_F(DefaultWorldTest, ShadingAnIntersectionFromTheInside) {
    Point origin { 0, 0, 0 }, light_origin { 0, 0.25, 0 };
    Vector direction { 0, 0, 1 };
    Ray ray { origin, direction };
    // Replace light
    ASSERT_EQ(default_world_.Remove(light_), 1);
    Colour light_colour { 1, 1, 1 };
    Light light { light_origin, light_colour };
    default_world_.Add(&light);
    Intersection i { 0.5, sphere2_ };
    IntersectionComputation ic { i, ray };
    Colour expected { 0.90498, 0.90498, 0.90498 },
           shade = default_world_.ColourAt(ic);
	ASSERT_TRUE(ColoursAreEqual(expected, shade));
}

/*
Scenario: The color when a ray misses
  Given w ← default_world()
    And r ← ray(point(0, 0, -5), vector(0, 1, 0))
  When c ← color_at(w, r)
  Then c = color(0, 0, 0)
*/

TEST_F(DefaultWorldTest, FindingTheColourWhenTheRayMisses) {
    Point origin { 0, 0, -5 };
    Vector direction { 0, 1, 0 };
    Ray ray { origin, direction };
    Colour expected  { 0, 0, 0 }, actual = default_world_.ColourAt(ray);
    ASSERT_EQ(expected, actual);
}

/*
Scenario: The color when a ray hits
  Given w ← default_world()
    And r ← ray(point(0, 0, -5), vector(0, 0, 1))
  When c ← color_at(w, r)
  Then c = color(0.38066, 0.47583, 0.2855)
*/

TEST_F(DefaultWorldTest, FindingTheColourWhenTheRayHits) {
    Point origin { 0, 0, -5 };
    Vector direction { 0, 0, 1 };
    Ray ray { origin, direction };
    Colour expected  { 0.38066, 0.47583, 0.2855 }, actual = default_world_.ColourAt(ray);
	ASSERT_TRUE(ColoursAreEqual(expected, actual));
}

/*
Scenario: The color with an intersection behind the ray
  Given w ← default_world()
    And outer ← the first object in w
    And outer.material.ambient ← 1
    And inner ← the second object in w
    And inner.material.ambient ← 1
    And r ← ray(point(0, 0, 0.75), vector(0, 0, -1))
  When c ← color_at(w, r)
  Then c = inner.material.color
*/

TEST_F(DefaultWorldTest, FindingTheColourForAnIntersectionBehindTheRay) {
    Point origin { 0, 0, 0.75 };
    Vector direction { 0, 0, -1 };
    Ray ray { origin, direction };
    // Adjust objects in world
    material1_.Ambient(1.0);
    sphere1_->SetMaterial(material1_);
    material2_.Ambient(1.0);
    sphere2_->SetMaterial(material2_);
    Colour actual = default_world_.ColourAt(ray), expected = sphere2_->ShapeMaterial().Surface();
	ASSERT_TRUE(ColoursAreEqual(expected, actual));
}

/*
Scenario: There is no shadow when nothing is collinear with point and light
  Given w ← default_world()
    And p ← point(0, 10, 0)
   Then is_shadowed(w, p) is false
*/

TEST_F(DefaultWorldTest, FindingNoShadowWhenNothingIsCollinearWithPointAndLight) {
    Point point { 0, 10, 0 };
    ASSERT_FALSE(default_world_.InShadow(point));
}

/*
Scenario: The shadow when an object is between the point and the light
  Given w ← default_world()
    And p ← point(10, -10, 10)
   Then is_shadowed(w, p) is true
*/

TEST_F(DefaultWorldTest, FindingAShadowWhenAnObjectIsBetweenThePointAndLight) {
    Point point { 10, -10, 10 };
    ASSERT_TRUE(default_world_.InShadow(point));
}

/*
Scenario: There is no shadow when an object is behind the light
  Given w ← default_world()
    And p ← point(-20, 20, -20)
   Then is_shadowed(w, p) is false
*/

TEST_F(DefaultWorldTest, FindingNoShadowWhenAnObjectIsBehindTheLight) {
    Point point { -20, 20, -20 };
    ASSERT_FALSE(default_world_.InShadow(point));
}

/*
Scenario: There is no shadow when an object is behind the point
  Given w ← default_world()
    And p ← point(-2, 2, -2)
   Then is_shadowed(w, p) is false
*/

TEST_F(DefaultWorldTest, FindingNoShadowWhenAnObjectIsBehindThePoint) {
    Point point { -2, 2, -2 };
    ASSERT_FALSE(default_world_.InShadow(point));
}

/*
Scenario: shade_hit() is given an intersection in shadow
  Given w ← world()
    And w.light ← point_light(point(0, 0, -10), color(1, 1, 1))
    And s1 ← sphere()
    And s1 is added to w
    And s2 ← sphere() with:
      | transform | translation(0, 0, 10) |
    And s2 is added to w
    And r ← ray(point(0, 0, 5), vector(0, 0, 1))
    And i ← intersection(4, s2)
  When comps ← prepare_computations(i, r)
    And c ← shade_hit(w, comps)
  Then c = color(0.1, 0.1, 0.1)
*/

TEST(WorldTest, FindingAnIntersectionInShadow) {
    World w {};
    Point light_origin { 0, 0, -10 };
    Colour light_colour { 1, 1, 1 };
    Light light { light_origin, light_colour };
    w.Add(&light);
    Sphere s1 {};
    w.Add(&s1);
    Sphere s2 {};
    Matrix transform = Transformation().Translate(0, 0, 10);
    s2.SetTransform(transform);
    w.Add(&s2);
    Point origin { 0, 0, 5 };
    Vector direction { 0, 0, 1 };
    Ray r { origin, direction };
    Intersection i { 4, &s2 };
    IntersectionComputation ic { i, r };
    Colour expected { 0.1, 0.1, 0.1 }, actual = w.ColourAt(ic);
    ASSERT_EQ(expected, actual);
}

/*
Scenario: The reflected color for a nonreflective material
  Given w ← default_world()
    And r ← ray(point(0, 0, 0), vector(0, 0, 1))
    And shape ← the second object in w
    And shape.material.ambient ← 1
    And i ← intersection(1, shape)
  When comps ← prepare_computations(i, r)
    And color ← reflected_color(w, comps)
  Then color = color(0, 0, 0)
*/

TEST_F(DefaultWorldTest, ConfirmingTheReflectedColourForNonreflectiveMaterial) {
    Ray r { Point(0, 0, 0), Vector(0, 0, 1) };
    material2_.Ambient(1.0);
    sphere2_->SetMaterial(material2_);
    Intersection i { 1, sphere2_ };
    IntersectionComputation comps { i, r };
    Colour colour = default_world_.ReflectedColour(comps);
    ASSERT_EQ(colour, Colour(0, 0, 0));
}

/*
Scenario: The reflected color for a reflective material
  Given w ← default_world()
    And shape ← plane() with:
      | material.reflective | 0.5                   |
      | transform           | translation(0, -1, 0) |
    And shape is added to w
    And r ← ray(point(0, 0, -3), vector(0, -√2/2, √2/2))
    And i ← intersection(√2, shape)
  When comps ← prepare_computations(i, r)
    And color ← reflected_color(w, comps)
  Then color = color(0.19032, 0.2379, 0.14274)
*/

TEST_F(DefaultWorldTest, ConfirmingTheReflectedColourForReflectiveMaterial) {
    Plane plane {};
    Material material {};
    material.Reflectivity(0.5);
    plane.SetMaterial(material);
    Transformation transform {};
    transform.Translate(0, -1, 0);
    plane.SetTransform(transform);
    default_world_.Add(&plane);

    Ray r { Point(0, 0, -3), Vector(0, -DefaultWorldTest::kHalfSqrt2,
        DefaultWorldTest::kHalfSqrt2) };

    Intersection i { DefaultWorldTest::kSqrt2, &plane };
    IntersectionComputation comps { i, r };
    Colour colour = default_world_.ReflectedColour(comps),
           expected { 0.19032, 0.2379, 0.14274 };
    // Note: smaller epsilon seems to be required for this test
    ASSERT_TRUE(ColoursAreEqual(expected, colour, 1e-4));
}

/*
Scenario: shade_hit() with a reflective material
  Given w ← default_world()
    And shape ← plane() with:
      | material.reflective | 0.5                   |
      | transform           | translation(0, -1, 0) |
    And shape is added to w
    And r ← ray(point(0, 0, -3), vector(0, -√2/2, √2/2))
    And i ← intersection(√2, shape)
  When comps ← prepare_computations(i, r)
    And color ← shade_hit(w, comps)
  Then color = color(0.87677, 0.92436, 0.82918)
*/

TEST_F(DefaultWorldTest, ConfirmingReflectedColourInWorld) {
    Plane plane {};
    Material material {};
    material.Reflectivity(0.5);
    plane.SetMaterial(material);
    Transformation transform {};
    transform.Translate(0, -1, 0);
    plane.SetTransform(transform);
    default_world_.Add(&plane);

    Ray r { Point(0, 0, -3), Vector(0, -DefaultWorldTest::kHalfSqrt2,
        DefaultWorldTest::kHalfSqrt2) };

    Intersection i { DefaultWorldTest::kSqrt2, &plane };
    IntersectionComputation comps { i, r };
    Colour colour = default_world_.ColourAt(comps),
           expected { 0.87677, 0.92436, 0.82918 };
    // Note: smaller epsilon seems to be required for this test
    ASSERT_TRUE(ColoursAreEqual(expected, colour, 1e-4));
}

/*
Scenario: color_at() with mutually reflective surfaces
  Given w ← world()
    And w.light ← point_light(point(0, 0, 0), color(1, 1, 1))
    And lower ← plane() with:
      | material.reflective | 1                     |
      | transform           | translation(0, -1, 0) |
    And lower is added to w
    And upper ← plane() with:
      | material.reflective | 1                    |
      | transform           | translation(0, 1, 0) |
    And upper is added to w
    And r ← ray(point(0, 0, 0), vector(0, 1, 0))
  Then color_at(w, r) should terminate successfully
*/

TEST(WorldTest, ConfirmingColourForMutuallyReflectiveSurfaces) {
    World w {};
    Light point_light { Point(0, 0, 0), Colour(1, 1, 1) };
    w.Add(&point_light);

    Plane lower {};
    Material lower_material {};
    lower_material.Reflectivity(1);
    Transformation lower_transform {};
    lower_transform.Translate(0, -1, 0);
    lower.SetMaterial(lower_material);
    lower.SetTransform(lower_transform);
    w.Add(&lower);

    Plane upper {};
    Material upper_material {};
    upper_material.Reflectivity(1);
    Transformation upper_transform {};
    upper_transform.Translate(0, 1, 0);
    upper.SetMaterial(upper_material);
    upper.SetTransform(upper_transform);
    w.Add(&upper);

    Ray r { Point(0, 0, 0), Vector(0, 1, 0) };
    // Hack to assert no exit due to seg fault (from endless recursion)
    EXPECT_EXIT((w.ColourAt(r), exit(0)), ::testing::ExitedWithCode(0),".*");
    // This is the assertion to use when there is (expected) endless recursion:
    // EXPECT_EXIT((w.ColourAt(r), exit(0)), ::testing::KilledBySignal(SIGSEGV), ".*");
}

/*
Scenario: The reflected color at the maximum recursive depth
  Given w ← default_world()
    And shape ← plane() with:
      | material.reflective | 0.5                   |
      | transform           | translation(0, -1, 0) |
    And shape is added to w
    And r ← ray(point(0, 0, -3), vector(0, -√2/2, √2/2))
    And i ← intersection(√2, shape)
  When comps ← prepare_computations(i, r)
    And color ← reflected_color(w, comps, 0)
  Then color = color(0, 0, 0)
*/

TEST_F(DefaultWorldTest, ConfirmingReflectedColourAtMaximumRecursiveDepth) {
    Plane plane {};
    Material material {};
    material.Reflectivity(0.5);
    plane.SetMaterial(material);
    Transformation transform {};
    transform.Translate(0, -1, 0);
    plane.SetTransform(transform);
    default_world_.Add(&plane);

    Ray r { Point(0, 0, -3), Vector(0, -DefaultWorldTest::kHalfSqrt2,
        DefaultWorldTest::kHalfSqrt2) };

    Intersection i { DefaultWorldTest::kSqrt2, &plane };
    IntersectionComputation comps { i, r };
    Colour colour = default_world_.ReflectedColour(comps, 0),
           expected { 0, 0, 0 };
    ASSERT_TRUE(ColoursAreEqual(expected, colour));
}

/*
Scenario: The refracted color with an opaque surface
  Given w ← default_world()
    And shape ← the first object in w
    And r ← ray(point(0, 0, -5), vector(0, 0, 1))
    And xs ← intersections(4:shape, 6:shape)
  When comps ← prepare_computations(xs[0], r, xs)
    And c ← refracted_color(w, comps, 5)
  Then c = color(0, 0, 0)
*/

TEST_F(DefaultWorldTest, ConfirmingRefractedColourOfOpaqueSurface) {
    Ray r { Point { 0, 0, -5 }, Vector { 0, 0, 1 } };
    Intersection x1 { 4, sphere1_ };
    Intersection x2 { 6, sphere1_ };
    IntersectionList intersections {};
    intersections.Add(x1);
    intersections.Add(x2);
    IntersectionComputation comps { x1, r, &intersections };
    Colour c = default_world_.RefractedColour(comps, 5),
           expected = Colour::kBlack;
    ASSERT_TRUE(ColoursAreEqual(c, expected));
}

/*
Scenario: The refracted color at the maximum recursive depth
  Given w ← default_world()
    And shape ← the first object in w
    And shape has:
      | material.transparency     | 1.0 |
      | material.refractive_index | 1.5 |
    And r ← ray(point(0, 0, -5), vector(0, 0, 1))
    And xs ← intersections(4:shape, 6:shape)
  When comps ← prepare_computations(xs[0], r, xs)
    And c ← refracted_color(w, comps, 0)
  Then c = color(0, 0, 0)
*/

TEST_F(DefaultWorldTest, ConfirmingRefractedColourAtTheMaximumRecursiveDepth) {
    Material m {};
    m.Transparency(1.0).RefractiveIndex(1.5);
    sphere1_->SetMaterial(m);
    Ray r { Point { 0, 0, -5 }, Vector { 0, 0, 1 } };
    Intersection x1 { 4, sphere1_ };
    Intersection x2 { 6, sphere1_ };
    IntersectionList intersections {};
    intersections.Add(x1);
    intersections.Add(x2);
    IntersectionComputation comps { x1, r, &intersections };
    Colour c = default_world_.RefractedColour(comps, 0),
           expected = Colour::kBlack;
    ASSERT_TRUE(ColoursAreEqual(c, expected));
}

/*
Scenario: The refracted color under total internal reflection
  Given w ← default_world()
    And shape ← the first object in w
    And shape has:
      | material.transparency     | 1.0 |
      | material.refractive_index | 1.5 |
    And r ← ray(point(0, 0, √2/2), vector(0, 1, 0))
    And xs ← intersections(-√2/2:shape, √2/2:shape)
  # NOTE: this time you're inside the sphere, so you need
  # to look at the second intersection, xs[1], not xs[0]
  When comps ← prepare_computations(xs[1], r, xs)
    And c ← refracted_color(w, comps, 5)
  Then c = color(0, 0, 0)
*/

TEST_F(DefaultWorldTest, ConfirmingRefractedColourUnderTotalInternalRefraction) {
    Material m {};
    m.Transparency(1.0).RefractiveIndex(1.5);
    sphere1_->SetMaterial(m);
    Ray r { Point { 0, 0, DefaultWorldTest::kHalfSqrt2 }, Vector { 0, 1, 0 } };
    Intersection x1 { -DefaultWorldTest::kHalfSqrt2, sphere1_ },
                 x2 { DefaultWorldTest::kHalfSqrt2, sphere1_ };
    IntersectionList intersections {};
    intersections.Add(x1);
    intersections.Add(x2);
    IntersectionComputation comps { x2, r, &intersections };
    Colour c = default_world_.RefractedColour(comps, 5),
           expected = Colour::kBlack;
    ASSERT_TRUE(ColoursAreEqual(c, expected));
}

/*
Scenario: The refracted color with a refracted ray
  Given w ← default_world()
    And A ← the first object in w
    And A has:
      | material.ambient | 1.0            |
      | material.pattern | test_pattern() |
    And B ← the second object in w
    And B has:
      | material.transparency     | 1.0 |
      | material.refractive_index | 1.5 |
    And r ← ray(point(0, 0, 0.1), vector(0, 1, 0))
    And xs ← intersections(-0.9899:A, -0.4899:B, 0.4899:B, 0.9899:A)
  When comps ← prepare_computations(xs[2], r, xs)
    And c ← refracted_color(w, comps, 5)
  Then c = color(0, 0.99888, 0.04725)
*/

TEST_F(DefaultWorldTest, ConfirmingRefractedColourWithARefractedRay) {
    Material m1 {};
    TestPattern p1 {};
    m1.Ambient(1.0).SurfacePattern(&p1);
    sphere1_->SetMaterial(m1);

    Material m2 {};
    m2.Transparency(1.0).RefractiveIndex(1.5);
    sphere2_->SetMaterial(m2);

    Ray r { Point { 0, 0, 0.1 }, Vector { 0, 1, 0 } };
    Intersection x1 { -0.9899, sphere1_ },
                 x2 { -0.4899, sphere2_ },
                 x3 { 0.4899, sphere2_ },
                 x4 { 0.9899, sphere1_ };
    IntersectionList intersections {};
    intersections.Add(x1);
    intersections.Add(x2);
    intersections.Add(x3);
    intersections.Add(x4);
    IntersectionComputation comps { x3, r, &intersections };
    Colour c = default_world_.RefractedColour(comps, 5),
           expected = Colour { 0, 0.99888, 0.04725 };
    ASSERT_TRUE(ColoursAreEqual(c, expected, 1e-4));
}

/*
Scenario: shade_hit() with a transparent material
  Given w ← default_world()
    And floor ← plane() with:
      | transform                 | translation(0, -1, 0) |
      | material.transparency     | 0.5                   |
      | material.refractive_index | 1.5                   |
    And floor is added to w
    And ball ← sphere() with:
      | material.color     | (1, 0, 0)                  |
      | material.ambient   | 0.5                        |
      | transform          | translation(0, -3.5, -0.5) |
    And ball is added to w
    And r ← ray(point(0, 0, -3), vector(0, -√2/2, √2/2))
    And xs ← intersections(√2:floor)
  When comps ← prepare_computations(xs[0], r, xs)
    And color ← shade_hit(w, comps, 5)
  Then color = color(0.93642, 0.68642, 0.68642)
*/

TEST_F(DefaultWorldTest, ConfirmingColourWithATransparentMaterial) {
    Plane floor {};
    floor.SetTransform(Transformation().Translate(0, -1 , 0));
    Material floor_material {};
    floor_material.Transparency(0.5).RefractiveIndex(1.5);
    floor.SetMaterial(floor_material);
    default_world_.Add(&floor);

    Sphere ball {};
    Material ball_material {};
    ball_material.Surface(Colour { 1, 0, 0 }).Ambient(0.5);
    ball.SetMaterial(ball_material);
    ball.SetTransform(Transformation().Translate(0, -3.5, -0.5));
    default_world_.Add(&ball);

    Ray r { Point { 0, 0, -3 },
        Vector { 0, -DefaultWorldTest::kHalfSqrt2, DefaultWorldTest::kHalfSqrt2 }};
    Intersection i { DefaultWorldTest::kSqrt2, &floor };
    IntersectionList intersections {};
    intersections.Add(i);
    IntersectionComputation comps { i, r, &intersections };
    Colour color = default_world_.ColourAt(comps, 5),
           expected = Colour { 0.93642, 0.68642, 0.68642 };
    ASSERT_TRUE(ColoursAreEqual(color, expected));
}

/*
Scenario: shade_hit() with a reflective, transparent material
  Given w ← default_world()
    And r ← ray(point(0, 0, -3), vector(0, -√2/2, √2/2))
    And floor ← plane() with:
      | transform                 | translation(0, -1, 0) |
      | material.reflective       | 0.5                   |
      | material.transparency     | 0.5                   |
      | material.refractive_index | 1.5                   |
    And floor is added to w
    And ball ← sphere() with:
      | material.color     | (1, 0, 0)                  |
      | material.ambient   | 0.5                        |
      | transform          | translation(0, -3.5, -0.5) |
    And ball is added to w
    And xs ← intersections(√2:floor)
  When comps ← prepare_computations(xs[0], r, xs)
    And color ← shade_hit(w, comps, 5)
  Then color = color(0.93391, 0.69643, 0.69243)
*/

TEST_F(DefaultWorldTest, ConfirmingColourWithAReflectiveAndTransparentMaterial) {
    Ray r { Point { 0, 0, -3 },
        Vector { 0, -DefaultWorldTest::kHalfSqrt2, DefaultWorldTest::kHalfSqrt2 }};

    Plane floor {};
    floor.SetTransform(Transformation().Translate(0, -1 , 0));
    Material floor_material {};
    floor_material.Reflectivity(0.5).Transparency(0.5).RefractiveIndex(1.5);
    floor.SetMaterial(floor_material);
    default_world_.Add(&floor);

    Sphere ball {};
    Material ball_material {};
    ball_material.Surface(Colour { 1, 0, 0 }).Ambient(0.5);
    ball.SetMaterial(ball_material);
    ball.SetTransform(Transformation().Translate(0, -3.5, -0.5));
    default_world_.Add(&ball);

    Intersection i { DefaultWorldTest::kSqrt2, &floor };
    IntersectionList intersections {};
    intersections.Add(i);
    IntersectionComputation comps { i, r, &intersections };
    Colour color = default_world_.ColourAt(comps, 5),
           expected = Colour { 0.93391, 0.69643, 0.69243 };
    ASSERT_TRUE(ColoursAreEqual(color, expected));
}

/*
Scenario: The shadow when an object is between the point and the light but the
object casts no shadow
  Given w ← default_world()
    And p ← point(10, -10, 10)
   Then is_shadowed(w, p) is not true
*/

TEST_F(DefaultWorldTest, FindingNoShadowWhenAShadowlessObjectIsBetweenThePointAndLight) {
    default_world_.Remove(sphere2_); // Remove second sphere to keep things simple
    Material m1 = sphere1_->ShapeMaterial();
    m1.CastsShadow(false);
    sphere1_->SetMaterial(m1);
    Point point { 10, -10, 10 };
    ASSERT_FALSE(default_world_.InShadow(point));
}

/*
Scenario: The shadow when one shadowless object is between the point and the
light but the object contains another that does cast shadows
  Given w ← default_world()
    And p ← point(10, -10, 10)
   Then is_shadowed(w, p) is true
*/
TEST_F(DefaultWorldTest, FindingAShadowWhenAShadowlessObjectContainsAnotherObject) {
    // Same as previous test but sphere2_ is still inside sphere1_
    Material m1 = sphere1_->ShapeMaterial();
    m1.CastsShadow(false);
    sphere1_->SetMaterial(m1);
    Point point { 10, -10, 10 };
    ASSERT_TRUE(default_world_.InShadow(point));
}

/*
Scenario: The shadow when a shadowless object is between the point and the
light and there is another object beside it that does cast shadows
*/

TEST(WorldTest, FindingAShadowWhenAShadowlessObjectIsBesideAnother) {
    World world {};
    Light point_light { Point(-10, 10, 0), Colour(1, 1, 1) };
    world.Add(&point_light);

    Sphere sphere1 {}, sphere2 {};
    // sphere1 is at the origin and casts no shadow
    Material m {};
    m.CastsShadow(false);
    sphere1.SetMaterial(m);
    world.Add(&sphere1);

    // sphere2 is 3X as big and right beside sphere1 along the x-axis
    sphere2.SetTransform(Transformation().Scale(3, 3, 3).Translate(-4, 0, 0));
    world.Add(&sphere2);

    Point point { 10, -10, 0 }; // located diagonally from light through the origin
    ASSERT_TRUE(world.InShadow(point));
}