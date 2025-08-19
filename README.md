# Ray Tracer

A C++ implementation of Jamis Buck's [Ray Tracer Challenge](http://raytracerchallenge.com/) (Pragmatic Bookshelf, 2019).

## To Do
* Cones, chapter 13.
* Chapter 15, "Triangles".
* Chapter 16, "Constructive Solid Geometry".
* Chapter 17, "Next Steps".

## Technologies Used
* C++ 17+
* CMake
* Google Test

## Known Issues
* For `ShapeGroup::Divide(threshold)`, small values of `threshold` produce incorrect results.
* For scenes involving multiple lights, shadows are not applied consistently.

## License
[MIT](LICENSE) © William Lockwood