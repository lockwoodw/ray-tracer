#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "canvas.h"

/*
Scenario: Creating a canvas
  Given c ← canvas(10, 20)
  Then c.width = 10
    And c.height = 20
    And every pixel of c is color(0, 0, 0)
*/

TEST(CanvasTest, CreatingACanvas) {
    int width { 10 }, height { 20 };
    Colour pixel { 0, 0, 0 };
    Canvas c { width, height };
    ASSERT_EQ(c.Width(), width);
    ASSERT_EQ(c.Height(), height);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            ASSERT_EQ(c[i][j], pixel);
        }
    }
}

/*
Scenario: Writing pixels to a canvas
  Given c ← canvas(10, 20)
    And red ← color(1, 0, 0)
  When write_pixel(c, 2, 3, red)
  Then pixel_at(c, 2, 3) = red
*/

TEST(CanvasTest, WritingPixels) {
    int width { 10 }, height { 20 };
    Colour pixel { 1, 0, 0 };
    Canvas c { width, height };
    // (x == 2, y == 3) means (row 3, column 2)
    c[3][2] = pixel;
    ASSERT_EQ(c[3][2], pixel);
}

/*
Scenario: Constructing the PPM header
  Given c ← canvas(5, 3)
  When ppm ← canvas_to_ppm(c)
  Then lines 1-3 of ppm are
    """
    P3
    5 3
    255
    """
*/

TEST(CanvasTest, ConstructingThePPMHeader) {
    int width { 5 }, height { 3 }, default_max { 255 };
    Canvas c { width, height };
    PPMv3 ppm { c };
    // output canvas to stream
    std::stringstream ss;
    ss << ppm;
    // build expected output
    std::vector<std::string> expected;
    expected.push_back("P3");
    expected.push_back(std::to_string(width) + " " + std::to_string(height));
    expected.push_back(std::to_string(default_max));
    // iterate over stream line by line and compare against expected
    int max_chars { 100 };
    char buffer[max_chars];
    for (int i = 0; i < expected.size(); i++) {
        ss.getline(buffer, max_chars);
        ASSERT_STREQ(expected[i].c_str(), buffer);
    }
}

TEST(CanvasTest, ConstructingThePPMHeaderWithNonDefaultMaxValue) {
    int width { 5 }, height { 3 }, default_max { 100 };
    Canvas c { width, height };
    PPMv3 ppm { c, default_max };
    // output canvas to stream
    std::stringstream ss;
    ss << ppm;
    // build expected output
    std::vector<std::string> expected;
    expected.push_back(PPMv3::kVersion);
    expected.push_back(std::to_string(width) + " " + std::to_string(height));
    expected.push_back(std::to_string(default_max));
    // iterate over stream line by line and compare against expected
    int max_chars { 100 };
    char buffer[max_chars];
    for (int i = 0; i < expected.size(); i++) {
        ss.getline(buffer, max_chars);
        ASSERT_STREQ(expected[i].c_str(), buffer);
    }
}

/*
Scenario: Constructing the PPM pixel data
  Given c ← canvas(5, 3)
    And c1 ← color(1.5, 0, 0)
    And c2 ← color(0, 0.5, 0)
    And c3 ← color(-0.5, 0, 1)
  When write_pixel(c, 0, 0, c1)
    And write_pixel(c, 2, 1, c2)
    And write_pixel(c, 4, 2, c3)
    And ppm ← canvas_to_ppm(c)
  Then lines 4-6 of ppm are
    """
    255 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    0 0 0 0 0 0 0 128 0 0 0 0 0 0 0
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 255
    """
*/

TEST(CanvasTest, ConstructingThePPMPixelData) {
    int width { 5 }, height { 3 };
    Canvas c { width, height };
    Colour c1 { 1.5, 0, 0 }, c2 { 0, 0.5, 0 }, c3 { -0.5, 0, 1 };
    c[0][0] = c1;
    c[1][2] = c2;
    c[2][4] = c3;
    PPMv3 ppm { c };
    // output canvas to stream
    std::stringstream ss;
    ss << ppm;
    // build expected output
    std::vector<std::string> expected, lines;
    expected.push_back("255 0 0 0 0 0 0 0 0 0 0 0 0 0 0");
    expected.push_back("0 0 0 0 0 0 0 128 0 0 0 0 0 0 0");
    expected.push_back("0 0 0 0 0 0 0 0 0 0 0 0 0 0 255");
    // collect actual output line by line
    int max_chars { 100 };
    char buffer[max_chars];
    do {
        ss.getline(buffer, max_chars);
        lines.push_back(std::string(buffer));
    } while (ss.gcount() > 0);
    ASSERT_GE(lines.size(), 6);
    // compare expected vs actual for lines 4-6
    for (int i = 0; i < expected.size(); i++) {
        ASSERT_STREQ(expected[i].c_str(), lines[i+3].c_str());
    }
}

/*
Scenario: Splitting long lines in PPM files
  Given c ← canvas(10, 2)
  When every pixel of c is set to color(1, 0.8, 0.6)
    And ppm ← canvas_to_ppm(c)
  Then lines 4-7 of ppm are
    """
    255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204
    153 255 204 153 255 204 153 255 204 153 255 204 153
    255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204
    153 255 204 153 255 204 153 255 204 153 255 204 153
    """
*/

TEST(CanvasTest, SplittingLongLines) {
    int width { 10 }, height { 2 };
    Canvas c { width, height };
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            Colour pixel { 1, 0.8, 0.6 };
            c[i][j] = pixel;
        }
    }
    PPMv3 ppm { c };
    // output canvas to stream
    std::stringstream ss;
    ss << ppm;
    // build expected output
    std::vector<std::string> expected, lines;
    expected.push_back("255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204");
    expected.push_back("153 255 204 153 255 204 153 255 204 153 255 204 153");
    expected.push_back("255 204 153 255 204 153 255 204 153 255 204 153 255 204 153 255 204");
    expected.push_back("153 255 204 153 255 204 153 255 204 153 255 204 153");
    // collect actual output line by line
    int max_chars { 100 };
    char buffer[max_chars];
    do {
        ss.getline(buffer, max_chars);
        lines.push_back(std::string(buffer));
    } while (ss.gcount() > 0);
    ASSERT_GE(lines.size(), 7);
    // compare expected vs actual for lines 4-7
    for (int i = 0; i < expected.size(); i++) {
        ASSERT_STREQ(expected[i].c_str(), lines[i+3].c_str());
    }
}

/*
Scenario: PPM files are terminated by a newline character
  Given c ← canvas(5, 3)
  When ppm ← canvas_to_ppm(c)
  Then ppm ends with a newline character
*/

TEST(CanvasTest, ConfirmingPPMFileEndsWithNewline) {
    int width { 5 }, height { 3 };
    Canvas c { width, height };
    PPMv3 ppm { c };
    std::stringstream ss;
    ss << ppm;
    std::string ppm_file = ss.str();
    ASSERT_EQ(ppm_file[ppm_file.length() - 1], '\n');
}