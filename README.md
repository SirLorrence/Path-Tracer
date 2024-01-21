# Ray Tracer

A Ray Tracer Renderer using C++. This project is aimed at helping me understand the fundamentals of Ray-tracing and how it works.

### Links: 
- [Site page, with a small write-up with issues I've faced with multi-threading](https://www.laurencesadler.com/ray-tracer)

## Features

* Implementation of all features included in [Ray Tracing In One Weekend](https://github.com/RayTracing/raytracing.github.io/blob/master/books/RayTracingInOneWeekend.html)
  * Spheres
  * Surface Normals
  * Antialiasing
  * Diffuse Materials
  * Metal
  * Dielectrics
  * Positionable Camera
  * Defocus Blur (“depth of field”)
- PNG image output
- Multi-Threading (with C++ STD and OpenMP)
- Render Time Information (ms) 




## Installation + Build

This program has been tested on Pop OS and MacOS but should work under any system that has cmake and a C++ compiler installed. (Clion, Visual Studio)

Once project files are downloaded and you are currently in the project directory, you can generate the Makefile using the following command:

```
mkdir build
cd build/
cmake ..
```
Additionally in the CMakeLists.txt you can enabled different types of rendering, by default its single threaded.
```
Remove '#' to enable feature
#add_compile_definitions(MTHREAD)
#add_compile_definitions(MTHREAD_V2) 
#add_compile_definitions(ENABLE_MP) <-- OpenMP Render
#add_compile_definitions(MTHREAD_TILE) <--- Tile MT Render
```
Now you can build the binary by running:

```
make
```

To generate the image, run the output binary in the terminal/cmd, which will result in the file appearing in the "images" folder


## Project status

### On Hold

## Roadmap
- Project Refactoring
- SDL Project conversion
- Runtime Rendering (being able to move the camera in a graphics window)



## Authors
- Laurence Sadler - [SirLorrence](https://gitlab.com/SirLorrence)

## Built with
- [Tiny PNG Output](https://www.nayuki.io/page/tiny-png-output) - to write PNG file
- [OpenMP](https://www.openmp.org/)

## License
This code is released under the [MIT License](https://gitlab.com/SirLorrence/raytracer/-/blob/main/LICENSE).

## References
- [_Ray Tracing in One Weekend_](https://raytracing.github.io/books/RayTracingInOneWeekend.html)
- [An Overview of the Ray-Tracing Rendering Technique
](https://scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-overview/ray-tracing-rendering-technique-overview.html)
- [3D Math Primer for Graphics and Game Development.](https://gamemath.com/) - Digitial and Physical 
- [ChatGPT](chat.openai.com) - How to create a CMake file and cryptic compiler errors
- [Adobe | What is a PPM file?](https://www.adobe.com/creativecloud/file-types/image/raster/ppm-file.html)


