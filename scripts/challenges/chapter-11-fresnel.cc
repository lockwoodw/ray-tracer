/*
The Ray Tracer Challenge: Chapter 11

Reproduce the image that demonstrates total internal reflection and the Fresnel
effect (p. 159).

The scene was configured based on the YAML posted here:
https://forum.raytracerchallenge.com/post/438/thread
*/

#define _USE_MATH_DEFINES // for M_PI

#include <cmath>
#include <iostream>

#include "colour.h"
#include "material.h"
#include "world.h"
#include "plane.h"
#include "camera.h"
#include "canvas.h"
#include "sphere.h"
#include "pattern.h"

/*
- add: light
  intensity: [ 0.9, 0.9, 0.9 ]
  at: [ 2, 10, -5 ]
*/

Light WorldLight() {
    Point origin { 2, 10, -5 };
    Colour colour { 0.9, 0.9, 0.9 };
    return Light { origin, colour };
}

/*
- add: camera
  width: 300
  height: 300
  field-of-view: 0.45
  from: [ 0, 0, -5 ]
  to: [ 0, 0, 0 ]
  up: [ 0, 1, 0 ]
*/

Camera SceneCamera() {
    Camera camera { 300, 300, M_PI / 4 };
    Point from { 0, 0, -5 }, to { 0, 0, 0 };
    Vector up { 0, 1, 0 };
    camera.SetTransform(ViewTransform { from, to, up });
    return camera;
}

/*
# wall
- add: plane
  transform:
    - [ rotate-x, 1.5708 ]
    - [ translate, 0, 0, 10 ]
  material:
    pattern:
      type: checkers
      colors:
        - [ 0.15, 0.15, 0.15 ]
        - [ 0.85, 0.85, 0.85 ]
    ambient: 0.8
    diffuse: 0.2
    specular: 0
*/

Plane Wall() {
    static CheckerPattern pattern { Colour { 0.15, 0.15, 0.15 }, Colour { 0.85, 0.85, 0.85 } };
    Plane wall {};
    wall.SetTransform(
        Transformation()
        .RotateX(M_PI/2)
        .Translate(0, 0, 10)
    );
    Material wall_m = Material()
        .Ambient(0.8)
        .Diffuse(0.2)
        .Specular(0)
        .SurfacePattern(&pattern);
    wall.SetMaterial(wall_m);
    return wall;
}

/*
# glass ball
- add: sphere
  material:
    color: [ 1, 1, 1 ]
    ambient: 0
    diffuse: 0
    specular: 0.9
    shininess: 300
    reflective: 0.9
    transparency: 0.9
    refractive-index: 1.5
*/

Sphere GlassMarble() {
    Sphere s {};
    Material m = Material()
        .Transparency(0.9)
        .RefractiveIndex(1.5)
        .Diffuse(0)
        .Ambient(0)
        .Reflectivity(0.9)
        .Shininess(300)
        .Specular(0.9)
        .Surface(Colour { 1, 1, 1 });
    s.SetMaterial(m);
    return s;
}

/*
# hollow center
- add: sphere
  transform:
    - [ scale, 0.5, 0.5, 0.5 ]
  material:
    color: [ 1, 1, 1 ]
    ambient: 0
    diffuse: 0
    specular: 0.9
    shininess: 300
    reflective: 0.9
    transparency: 0.9
    refractive-index: 1.0000034
*/

Sphere AirBubble() {
    Sphere s {};
    Material m = Material()
        .Transparency(0.9)
        .RefractiveIndex(1.0000034)
        .Diffuse(0)
        .Ambient(0)
        .Specular(0.9)
        .Reflectivity(0.9)
        .Shininess(300);
    s.SetMaterial(m);
    s.SetTransform(Transformation().Scale(0.5));
    return s;
}

int main(int argc, char** argv) {
    World world {};

    Light light = WorldLight();
    world.Add(&light);

    Plane wall = Wall();
    world.Add(&wall);

    Sphere marble = GlassMarble();
    world.Add(&marble);

    Sphere bubble = AirBubble();
    world.Add(&bubble);

    Camera camera = SceneCamera();
    Canvas canvas = camera.Render(world);
    PPMv3 ppm { canvas };
    std::cout << ppm;
    return 0;
}
