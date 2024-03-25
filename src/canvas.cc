#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <string>
#include <array>
#include "canvas.h"

Canvas::Canvas(int width, int height): width_ { width }, height_ { height } {
    pixels_ = new Colour*[height_];
    for (int i = 0; i < height_; i++) {
        pixels_[i] = new Colour[width_];
    }
}

Canvas::~Canvas() {
    for (int i = 0; i < height_; i++) {
        delete[] pixels_[i];
    }
    delete[] pixels_;
}

Colour* Canvas::operator[](int row) {
    if (row < 0 || row >= height_) {
        throw std::runtime_error("Row index out of bounds");
    }
    return pixels_[row];
}

Colour Canvas::At(int row, int column) const {
    if (row < 0 || row >= height_) {
        throw std::runtime_error("Row index out of bounds");
    }
    if (column < 0 || column >= width_) {
        throw std::runtime_error("Column index out of bounds");
    }
    return pixels_[row][column];
}

const std::string PPMv3::kVersion = "P3";
const int PPMv3::kMaxColourDefault = 255;
const int PPMv3::kMaxCharsPerLine = 70;

// Each RGB value has to lie between 0-255; clamp values greater than 255 or less than 0
int PPMv3::normalize(double value) const {
    return std::max(0, std::min(static_cast<int>(std::nearbyint(value * max_colour_)), max_colour_));
}

std::ostream& operator<<(std::ostream& os, const PPMv3& ppm) {
    // Header:
    os << ppm.kVersion << std::endl;
    os << ppm.canvas_.Width() << " " << ppm.canvas_.Height() << std::endl;
    os << ppm.max_colour_ << std::endl;
    // Data: no line can exceed kMaxCharsPerLine
    for (int i = 0; i < ppm.canvas_.Height(); i++) {
        int nchars = 0;
        for (int j = 0; j < ppm.canvas_.Width(); j++) {
            Colour pixel = ppm.canvas_.At(i, j);
            std::array<std::string, 3> colours = {{
                std::to_string(ppm.normalize(pixel.Red())),
                std::to_string(ppm.normalize(pixel.Green())),
                std::to_string(ppm.normalize(pixel.Blue()))
            }};
            for (std::string colour : colours) {
                // Add 1 to length for preceding space character
                int length = colour.length() + (nchars > 0 ? 1 : 0);
                // Confirm we can output colour on existing line.
                // Total length can't exceed kMaxCharsPerLine - 1
                // (subtract 1 for the newline).
                if (nchars + length <= (PPMv3::kMaxCharsPerLine - 1)) {
                    os << (nchars > 0 ? " " : "") << colour;
                    nchars += length;
                }
                else {
                    // Start new line
                    os << std::endl << colour;
                    nchars = colour.length();
                }
            }
        }
        os << std::endl; // End of row
    }
    return os;
}