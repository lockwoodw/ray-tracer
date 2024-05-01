#include "pattern.h"

#include <gtest/gtest.h>

#include "colour.h"
#include "shape.h"
#include "sphere.h"
#include "matrix.h"
#include "transformations.h"

class TestPattern: public Pattern {
    public:
        TestPattern(): Pattern {} {}
        const Colour ColourAt(const Point& p) const {
            return Colour { p.X(), p.Y(), p.Z() };
        }
        bool operator==(const Pattern& p) const {
            const TestPattern* other = dynamic_cast<const TestPattern*>(&p);
            return other != nullptr;
        }
};

/*
Background:
  Given black ← color(0, 0, 0)
    And white ← color(1, 1, 1)
*/

class DefaultPatternTest: public testing::Test {
    protected:
        Colour black_;
        Colour white_;

    public:
        DefaultPatternTest(): black_ { Colour { 0, 0, 0 } }, white_ { Colour { 1, 1, 1 } } {}
};

/*
Scenario: Creating a stripe pattern
  Given pattern ← stripe_pattern(white, black)
  Then pattern.a = white
    And pattern.b = black
*/

TEST_F(DefaultPatternTest, CreatingAStripePattern) {
    StripePattern pattern { white_, black_ };
    ASSERT_EQ(pattern.A(), white_);
    ASSERT_EQ(pattern.B(), black_);
}

/*
Scenario: A stripe pattern is constant in y
  Given pattern ← stripe_pattern(white, black)
  Then stripe_at(pattern, point(0, 0, 0)) = white
    And stripe_at(pattern, point(0, 1, 0)) = white
    And stripe_at(pattern, point(0, 2, 0)) = white
*/

TEST_F(DefaultPatternTest, ConfirmingAStripePatternIsConstantInY) {
    StripePattern pattern { white_, black_ };
    ASSERT_EQ(pattern.ColourAt(Point { 0, 0, 0 }), white_);
    ASSERT_EQ(pattern.ColourAt(Point { 0, 1, 0 }), white_);
    ASSERT_EQ(pattern.ColourAt(Point { 0, 2, 0 }), white_);
}

/*
Scenario: A stripe pattern is constant in z
  Given pattern ← stripe_pattern(white, black)
  Then stripe_at(pattern, point(0, 0, 0)) = white
    And stripe_at(pattern, point(0, 0, 1)) = white
    And stripe_at(pattern, point(0, 0, 2)) = white
*/

TEST_F(DefaultPatternTest, ConfirmingAStripePatternIsConstantInZ) {
    StripePattern pattern { white_, black_ };
    ASSERT_EQ(pattern.ColourAt(Point { 0, 0, 0 }), white_);
    ASSERT_EQ(pattern.ColourAt(Point { 0, 0, 1 }), white_);
    ASSERT_EQ(pattern.ColourAt(Point { 0, 0, 2 }), white_);
}

/*
Scenario: A stripe pattern alternates in x
  Given pattern ← stripe_pattern(white, black)
  Then stripe_at(pattern, point(0, 0, 0)) = white
    And stripe_at(pattern, point(0.9, 0, 0)) = white
    And stripe_at(pattern, point(1, 0, 0)) = black
    And stripe_at(pattern, point(-0.1, 0, 0)) = black
    And stripe_at(pattern, point(-1, 0, 0)) = black
    And stripe_at(pattern, point(-1.1, 0, 0)) = white
*/

TEST_F(DefaultPatternTest, ConfirmingAStripePatternAlternatesInX) {
    StripePattern pattern { white_, black_ };
    ASSERT_EQ(pattern.ColourAt(Point { 0, 0, 0 }), white_);
    ASSERT_EQ(pattern.ColourAt(Point { 0.9, 0, 1 }), white_);
    ASSERT_EQ(pattern.ColourAt(Point { 1, 0, 2 }), black_);
    ASSERT_EQ(pattern.ColourAt(Point { -0.1, 0, 0 }), black_);
    ASSERT_EQ(pattern.ColourAt(Point { -1, 0, 1 }), black_);
    ASSERT_EQ(pattern.ColourAt(Point { -1.1, 0, 2 }), white_);
}

/*
Scenario: Stripes with an object transformation
  Given object ← sphere()
    And set_transform(object, scaling(2, 2, 2))
    And pattern ← stripe_pattern(white, black)
  When c ← stripe_at_object(pattern, object, point(1.5, 0, 0))
  Then c = white
*/

TEST_F(DefaultPatternTest, ConfirmingStripesWithAnObjectTransformation) {
    Sphere object {};
    object.SetTransform(Transformation().Scale(2, 2, 2));
    StripePattern pattern { white_, black_ };
    ASSERT_EQ(pattern.ObjectColourAt(&object, Point { 1.5, 0, 0 }), white_);
}

/*
Scenario: Stripes with a pattern transformation
  Given object ← sphere()
    And pattern ← stripe_pattern(white, black)
    And set_pattern_transform(pattern, scaling(2, 2, 2))
  When c ← stripe_at_object(pattern, object, point(1.5, 0, 0))
  Then c = white
*/

TEST_F(DefaultPatternTest, ConfirmingStripesWithAPatternTransformation) {
    Sphere object {};
    StripePattern pattern { white_, black_ };
    pattern.SetTransform(Transformation().Scale(2, 2, 2));
    ASSERT_EQ(pattern.ObjectColourAt(&object, Point { 1.5, 0, 0 }), white_);
}

/*
Scenario: Stripes with both an object and a pattern transformation
  Given object ← sphere()
    And set_transform(object, scaling(2, 2, 2))
    And pattern ← stripe_pattern(white, black)
    And set_pattern_transform(pattern, translation(0.5, 0, 0))
  When c ← stripe_at_object(pattern, object, point(2.5, 0, 0))
  Then c = white
*/

TEST_F(DefaultPatternTest, ConfirmingStripesWithObjectAndPatternTransformations) {
    Sphere object {};
    object.SetTransform(Transformation().Scale(2, 2, 2));
    StripePattern pattern { white_, black_ };
    pattern.SetTransform(Transformation().Translate(0.5, 0, 0));
    ASSERT_EQ(pattern.ObjectColourAt(&object, Point { 2.5, 0, 0 }), white_);
}

/*
Scenario: The default pattern transformation
  Given pattern ← test_pattern()
  Then pattern.transform = identity_matrix
*/

TEST(PatternTest, ConfirmingTheDefaultPatternTransformation) {
    TestPattern pattern {};
    ASSERT_EQ(pattern.Transform(), Matrix::Identity(4));
}

/*
Scenario: Assigning a transformation
  Given pattern ← test_pattern()
  When set_pattern_transform(pattern, translation(1, 2, 3))
  Then pattern.transform = translation(1, 2, 3)
*/

TEST(PatternTest, AssigningATransform) {
    TestPattern pattern {};
    Matrix transform = Transformation().Translate(1, 2, 3);
    pattern.SetTransform(transform);
    ASSERT_EQ(pattern.Transform(), transform);
}

/*
Scenario: A pattern with an object transformation
  Given shape ← sphere()
    And set_transform(shape, scaling(2, 2, 2))
    And pattern ← test_pattern()
  When c ← pattern_at_shape(pattern, shape, point(2, 3, 4))
  Then c = color(1, 1.5, 2)
*/

TEST(PatternTest, ConfirmingAPatternWithAnObjectTransformation) {
    Sphere shape {};
    shape.SetTransform(Transformation().Scale(2, 2, 2));
    TestPattern pattern {};
    Point p { 2, 3, 4 };
    Colour expected { 1, 1.5, 2 };
    ASSERT_EQ(pattern.ObjectColourAt(&shape, p), expected);
}

/*
Scenario: A pattern with a pattern transformation
  Given shape ← sphere()
    And pattern ← test_pattern()
    And set_pattern_transform(pattern, scaling(2, 2, 2))
  When c ← pattern_at_shape(pattern, shape, point(2, 3, 4))
  Then c = color(1, 1.5, 2)
*/

TEST(PatternTest, ConfirmingAPatternWithAPatternTransformation) {
    Sphere shape {};
    TestPattern pattern {};
    pattern.SetTransform(Transformation().Scale(2, 2, 2));
    Point p { 2, 3, 4 };
    Colour expected { 1, 1.5, 2 };
    ASSERT_EQ(pattern.ObjectColourAt(&shape, p), expected);
}

/*
Scenario: A pattern with both an object and a pattern transformation
  Given shape ← sphere()
    And set_transform(shape, scaling(2, 2, 2))
    And pattern ← test_pattern()
    And set_pattern_transform(pattern, translation(0.5, 1, 1.5))
  When c ← pattern_at_shape(pattern, shape, point(2.5, 3, 3.5))
  Then c = color(0.75, 0.5, 0.25)
*/

TEST(PatternTest, ConfirmingAPatternWithObjectAndPatternTransformations) {
    Sphere shape {};
    shape.SetTransform(Transformation().Scale(2, 2, 2));
    TestPattern pattern {};
    pattern.SetTransform(Transformation().Translate(0.5, 1, 1.5));
    Point p { 2.5, 3, 3.5 };
    Colour expected { 0.75, 0.5, 0.25 };
    ASSERT_EQ(pattern.ObjectColourAt(&shape, p), expected);
}

/*
Scenario: A gradient linearly interpolates between colors
  Given pattern ← gradient_pattern(white, black)
  Then pattern_at(pattern, point(0, 0, 0)) = white
    And pattern_at(pattern, point(0.25, 0, 0)) = color(0.75, 0.75, 0.75)
    And pattern_at(pattern, point(0.5, 0, 0)) = color(0.5, 0.5, 0.5)
    And pattern_at(pattern, point(0.75, 0, 0)) = color(0.25, 0.25, 0.25)
*/

/*
Scenario: A ring should extend in both x and z
  Given pattern ← ring_pattern(white, black)
  Then pattern_at(pattern, point(0, 0, 0)) = white
    And pattern_at(pattern, point(1, 0, 0)) = black
    And pattern_at(pattern, point(0, 0, 1)) = black
    # 0.708 = just slightly more than √2/2
    And pattern_at(pattern, point(0.708, 0, 0.708)) = black
*/

/*
Scenario: Checkers should repeat in x
  Given pattern ← checkers_pattern(white, black)
  Then pattern_at(pattern, point(0, 0, 0)) = white
    And pattern_at(pattern, point(0.99, 0, 0)) = white
    And pattern_at(pattern, point(1.01, 0, 0)) = black
*/

/*
Scenario: Checkers should repeat in y
  Given pattern ← checkers_pattern(white, black)
  Then pattern_at(pattern, point(0, 0, 0)) = white
    And pattern_at(pattern, point(0, 0.99, 0)) = white
    And pattern_at(pattern, point(0, 1.01, 0)) = black
*/

/*
Scenario: Checkers should repeat in z
  Given pattern ← checkers_pattern(white, black)
  Then pattern_at(pattern, point(0, 0, 0)) = white
    And pattern_at(pattern, point(0, 0, 0.99)) = white
    And pattern_at(pattern, point(0, 0, 1.01)) = black
*/