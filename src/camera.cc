#include <cmath>
#include "camera.h"
#include "space.h"
#include "colour.h"

Camera::Camera(int horizontal, int vertical, double field_of_view):
        horizontal_ { horizontal },
        vertical_ { vertical },
        field_of_view_ { field_of_view },
        transform_ { Matrix::Identity(4) },
        inverse_transform_ { Matrix::Identity(4) } {
    double half_view = std::tan(field_of_view_ / 2);
    double aspect = static_cast<double>(horizontal_) / vertical_;
    if (aspect >= 1) {
        half_width_ = half_view;
        half_height_ = half_view / aspect;
    }
    else {
        half_width_ = half_view * aspect;
        half_height_ = half_view;
    }
    pixel_size_ = half_width_ * 2 / horizontal;
}

// Compute in world coords the ray passing through the given pixel (canvas coords)
const Ray Camera::RayAt(int pixel_x, int pixel_y) const {
    // Calculate x,y offsets from bottom-left corner of canvas to the center
    // of the pixel; add 0.5 because (pixel_x, pixel_y) is bottom left
    // coordinates of the pixel
    double x_offset = (pixel_x + 0.5) * pixel_size_;
    double y_offset = (pixel_y + 0.5) * pixel_size_;

    // Calculate untransformed coords of the pixel in world space.
    // The camera looks towards -z in world space, so +x in the world is on
    // the left from the camera's POV.
    double world_x = half_width_ - x_offset;
    double world_y = half_height_ - y_offset;

    // Apply camera's transform to orient world relative to camera
    // (moving camera 2 units to the right is the same as moving the world 2
    // units to the left).
    // The canvas is positioned at z = -1 in world space.
    Point world_point { world_x, world_y, -1 },
          world_origin { 0, 0, 0 },
          pixel = inverse_transform_ * world_point,
          origin = inverse_transform_ * world_origin;
    Vector direction = pixel - origin;
    Ray ray { origin, direction.Normalize() };
    return ray;
}

const Canvas Camera::Render(const World& world) const {
    Canvas image { horizontal_, vertical_ };
    for (int row = 0; row < vertical_; row++) {
        for (int column = 0; column < horizontal_; column++) {
            Ray ray = RayAt(column, row);
            Colour colour = world.ColourAt(ray);
            image[row][column] = colour;
        }
    }
    return image;
}