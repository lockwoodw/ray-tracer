#ifndef RAY_TRACER_CANVAS_H
#define RAY_TRACER_CANVAS_H

#include <iostream>
#include <string>
#include "colour.h"

class Canvas {
    int width_;
    int height_;
    Colour** pixels_;

    public:
        Canvas(int width, int height);
        ~Canvas();

        Colour* operator[](int row);
        Colour At(int row, int column) const;

        int Width() const { return width_; }
        int Height() const { return height_; }
};

class PPMv3 {
    const Canvas& canvas_;
    int max_colour_;

    public:
        static const int kMaxColourDefault;
        static const std::string kVersion;
        static const int kMaxCharsPerLine;
        PPMv3(const Canvas& c): canvas_ { c }, max_colour_ { kMaxColourDefault } {}
        PPMv3(const Canvas& c, int max_colour): canvas_ { c }, max_colour_ { max_colour } {}
        int normalize(double value) const;

        friend std::ostream& operator<<(std::ostream& os, const PPMv3& ppm);
};

#endif