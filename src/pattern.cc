#include "pattern.h"

#include <cmath>

const Colour Pattern::ObjectColourAt(const Shape* object, const Point& world_point) const {
    Point object_point = object->ConvertWorldPointToObjectSpace(world_point),
          pattern_point = transform_.Inverse() * object_point;
    return ColourAt(pattern_point);
}

bool SolidPattern::operator==(const Pattern& p) const {
    const SolidPattern* other = dynamic_cast<const SolidPattern*>(&p);
    return (other != nullptr) ? (colour_ == other->colour_) : false;
}

std::mt19937_64 SpeckledPattern::mt_engine_ = std::mt19937_64 { static_cast<long unsigned int>(time(nullptr)) };
std::uniform_real_distribution<double> SpeckledPattern::urd_ { 0.0, 1.0 };
const double SpeckledPattern::kDefaultDarkThreshold { 0.3 };
const double SpeckledPattern::kDefaultLightThreshold { 0.1 };
const double SpeckledPattern::kDefaultAttenuation { 0.1 };

bool SpeckledPattern::operator==(const Pattern& p) const {
    const SpeckledPattern* other = dynamic_cast<const SpeckledPattern*>(&p);
    return (other != nullptr) ? (colour_ == other->colour_) : false;
}

Colour SpeckledPattern::AdjustColour(Colour& c, bool darken) const {
    std::array<double, 3> colour_points {
         c.Red(), c.Green(), c.Blue()
    };
    if (darken) {
        for (auto& point: colour_points) {
            point = std::max(point - point * urd_(mt_engine_) * attenuation_, 0.0);
        }
    }
    else {
        for (auto& point: colour_points) {
            point = std::min(point + point * urd_(mt_engine_) * attenuation_, 1.0);
        }
    }
    return Colour { colour_points[0], colour_points[1], colour_points[2] };
}

const Colour SpeckledPattern::ColourAt(const Point& p) const {
    Colour copy = colour_;
    if (urd_(mt_engine_) <= dark_threshold_) {
        // Change colour if randomly generated number is within the threshold;
        // if so, darken the colour if randomly generated number
        return AdjustColour(copy, urd_(mt_engine_) > light_threshold_);
    }
    return colour_;
}

TwoColourMetaPattern::TwoColourMetaPattern(const Colour& a, const Colour& b): Pattern {}, owned_ { true } {
    a_ = new SolidPattern(a);
    b_ = new SolidPattern(b);
}

TwoColourMetaPattern::~TwoColourMetaPattern() {
    if (owned_) {
        delete a_;
        delete b_;
    }
}

// Override the base class method because we need to calculate the world point
// in pattern space for all patterns
const Colour TwoColourMetaPattern::ObjectColourAt(const Shape* object, const Point& world_point) const {
    Point object_point = object->Transform().Inverse() * world_point;
    return ColourAt(object_point);
}

const Colour StripePattern::ColourAt(const Point& object_point) const {
    Point p = transform_.Inverse() * object_point;
    const Pattern* pattern = (static_cast<int>(std::floor(p.X())) % 2) == 0 ? a_ : b_;
    return pattern->ColourAt(object_point);
}

bool StripePattern::operator==(const Pattern& p) const {
    const StripePattern* other = dynamic_cast<const StripePattern*>(&p);
    return (other != nullptr) && (owned_ == other->owned_) && (*a_ == *other->a_) && (*b_ == *other->b_);
}

const Colour GradientPattern::ColourAt(const Point& object_point) const {
    // Blending function: calculate distance between colours a and b;
    // multiply distance by fractional portion of x coordinate;
    // add this product to the base colour
    Point p = transform_.Inverse() * object_point;
    Colour a = a_->ColourAt(object_point),
           b = b_->ColourAt(object_point);
    return a + ((b - a) * (p.X() - std::floor(p.X())));
}

bool GradientPattern::operator==(const Pattern& p) const {
    const GradientPattern* other = dynamic_cast<const GradientPattern*>(&p);
    return (other != nullptr) && (owned_ == other->owned_) && (*a_ == *other->a_) && (*b_ == *other->b_);
}

const Colour RingPattern::ColourAt(const Point& object_point) const {
    // Test distance between a_ and b_ in both x and z dimensions
    Point p = transform_.Inverse() * object_point;
    double x = p.X(),
           z = p.Z(),
           distance = std::floor(std::sqrt(x * x + z * z));
    const Pattern* pattern = (static_cast<int>(distance) % 2 == 0) ? a_ : b_;
    return pattern->ColourAt(object_point);
}

bool RingPattern::operator==(const Pattern& p) const {
    const RingPattern* other = dynamic_cast<const RingPattern*>(&p);
    return (other != nullptr) && (owned_ == other->owned_) && (*a_ == *other->a_) && (*b_ == *other->b_);
}

const Colour CheckerPattern::ColourAt(const Point& object_point) const {
    // Apply stripe algorithm but in all three dimensions
    Point p = transform_.Inverse() * object_point;
    const Pattern* pattern = (static_cast<int>(std::floor(p.X()) + std::floor(p.Y()) + std::floor(p.Z())) % 2 == 0)
        ? a_ : b_;
    return pattern->ColourAt(object_point);
}

bool CheckerPattern::operator==(const Pattern& p) const {
    const CheckerPattern* other = dynamic_cast<const CheckerPattern*>(&p);
    return (other != nullptr) && (owned_ == other->owned_) && (*a_ == *other->a_) && (*b_ == *other->b_);
}

const Colour RadialGradientPattern::ColourAt(const Point& object_point) const {
    // Combine ring and gradient patterns: each band in the ring will be a gradient.
    // Determine the starting pattern for the gradient, based on the band.
    Point p = transform_.Inverse() * object_point;
    double x = p.X(),
           z = p.Z(),
           distance = std::floor(std::sqrt(x * x + z * z));
    Colour a = a_->ColourAt(object_point),
           b = b_->ColourAt(object_point),
           starting_colour = (static_cast<int>(distance) % 2 == 0) ? a : b,
           ending_colour = (starting_colour == a) ? b : a;
    return starting_colour + ((ending_colour - starting_colour) * (p.X() - std::floor(p.X())));
}

bool RadialGradientPattern::operator==(const Pattern& p) const {
    const RadialGradientPattern* other = dynamic_cast<const RadialGradientPattern*>(&p);
    return (other != nullptr) && (owned_ == other->owned_) && (*a_ == *other->a_) && (*b_ == *other->b_);
}

Colour AveragePatternBlender::Blend(std::vector<Pattern*> patterns, const Point& object_point) const {
    Colour sum { 0, 0, 0 };
    for (Pattern* pattern: patterns) {
        sum += pattern->ColourAt(object_point);
    }
    return sum / patterns.size();
}

bool AveragePatternBlender::operator==(const PatternBlender& pb) const {
    const AveragePatternBlender* other = dynamic_cast<const AveragePatternBlender*>(&pb);
    return other != nullptr;
}

// Override the base class method because we need to calculate the world point
// in pattern space for both patterns
const Colour BlendedPattern::ObjectColourAt(const Shape* object, const Point& world_point) const {
    Point object_point = object->Transform().Inverse() * world_point;
    return ColourAt(object_point);
}

const Colour BlendedPattern::ColourAt(const Point& p) const {
    return blender_->Blend(patterns_, p);
}

bool BlendedPattern::operator==(const Pattern& p) const {
    const BlendedPattern* other = dynamic_cast<const BlendedPattern*>(&p);
    return (other != nullptr) && (*blender_ == *other->blender_) && (patterns_ == other->patterns_);
}

const int PerlinNoise::kPermutation[256] { 151,160,137,91,90,15,
   131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
   190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
   88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
   77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
   102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
   135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
   5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
   223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
   129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
   251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
   49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
   138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};

int PerlinNoise::kP[512] {};

PerlinNoise::PerlinNoise() {
    int n_permutations = sizeof(kPermutation) / sizeof(int);
    for (int i = 0; i < n_permutations ; i++) {
        kP[n_permutations+i] = kP[i] = kPermutation[i];
    }
}

double PerlinNoise::Grad(int hash, double x, double y, double z) const {
    int h = hash & 15;                      // CONVERT LO 4 BITS OF HASH CODE
    double u = h<8 ? x : y,                 // INTO 12 GRADIENT DIRECTIONS.
           v = h<4 ? y : h==12||h==14 ? x : z;
    return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
}

double PerlinNoise::Noise(const Point& point) const {
    double x = point.X(), y = point.Y(), z = point.Z();
    int X = static_cast<int>(std::floor(x)) & 255,     // FIND UNIT CUBE THAT
        Y = static_cast<int>(std::floor(y)) & 255,     // CONTAINS POINT.
        Z = static_cast<int>(std::floor(z)) & 255;
    x -= std::floor(x);                                // FIND RELATIVE X,Y,Z
    y -= std::floor(y);                                // OF POINT IN CUBE.
    z -= std::floor(z);
    double u = Fade(x),                                // COMPUTE FADE CURVES
           v = Fade(y),                                // FOR EACH OF X,Y,Z.
           w = Fade(z);
    int A = kP[X  ]+Y, AA = kP[A]+Z, AB = kP[A+1]+Z,      // HASH COORDINATES OF
        B = kP[X+1]+Y, BA = kP[B]+Z, BB = kP[B+1]+Z;      // THE 8 CUBE CORNERS,

    return Lerp(w, Lerp(v, Lerp(u, Grad(kP[AA  ], x  , y  , z   ),  // AND ADD
                                   Grad(kP[BA  ], x-1, y  , z   )), // BLENDED
                           Lerp(u, Grad(kP[AB  ], x  , y-1, z   ),  // RESULTS
                                   Grad(kP[BB  ], x-1, y-1, z   ))),// FROM  8
                   Lerp(v, Lerp(u, Grad(kP[AA+1], x  , y  , z-1 ),  // CORNERS
                                   Grad(kP[BA+1], x-1, y  , z-1 )), // OF CUBE
                           Lerp(u, Grad(kP[AB+1], x  , y-1, z-1 ),
                                   Grad(kP[BB+1], x-1, y-1, z-1 ))));
}

const Colour PerturbedPattern::ObjectColourAt(const Shape* object, const Point& world_point) const {
    Point object_point = object->Transform().Inverse() * world_point;
    double offset = generator_.Noise(object_point);
    Point perturbed { object_point.X() + offset, object_point.Y() + offset, object_point.Z() + offset };
    return ColourAt(perturbed);
}

const Colour PerturbedPattern::ColourAt(const Point& p) const {
    return pattern_->ColourAt(p);
}

bool PerturbedPattern::operator==(const Pattern& p) const {
    const PerturbedPattern* other = dynamic_cast<const PerturbedPattern*>(&p);
    return (other != nullptr) && (*pattern_ == *other->pattern_);
}
