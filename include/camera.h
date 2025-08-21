#ifndef RAY_TRACER_CAMERA_H
#define RAY_TRACER_CAMERA_H

#include "transformations.h"
#include "ray.h"
#include "canvas.h"
#include "world.h"

class Camera {
    int horizontal_;
    int vertical_;
    double field_of_view_;
    Matrix transform_;
    Matrix inverse_transform_;
    double half_width_;
    double half_height_;
    double pixel_size_;

    public:
        Camera(int horizontal, int vertical, double field_of_view);

        int Horizontal() const {
            return horizontal_;
        }

        int Vertical() const {
            return vertical_;
        }

        double FieldOfView() const  {
            return field_of_view_;
        }

        double PixelSize() {
            return pixel_size_;
        }

        const Matrix Transform() const {
            return transform_;
        }

        void SetTransform(const Matrix& transform) {
            transform_ *= transform;
            inverse_transform_ = transform_.Inverse();
        }

        const Ray RayAt(int pixel_x, int pixel_y) const;
        const Canvas Render(const World& world) const;
        const Canvas RenderConcurrent(const World& world) const;
};

#endif