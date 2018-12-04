# renda
An exercise in raytracing.

## Compile
Single-threaded: `g++ -std=c++11 -O3 src/main_single.cpp -o renda_single`

Multi-threaded: `g++ -std=c++11 -O3 src/main.cpp -o renda`

## Use
Single-threaded: `./renda_single`

Multi-threaded: `./renda`

## Notes
Currently, the multi-threaded version is at least ~3x faster than its
single-threaded counterpart (at least by my own testing). The following
1200x800 image took ~3 minutes to render on a 2017 MacBook Pro 2.3 GHz Intel Core i5 processor.

![Lots and lots of spheres](https://i.imgur.com/JI5OfV5.jpg)

I've also moved the multithreading process to a more sensible `std::thread` model
instead of using `std::async`.

### Fixes for Potential Issues
The program may terminate because `drand48()` creates "too many open
files" with `/dev/urandom`. Running `ulimit -n [big_number]` will fix the
issue.

### Current Progress
Finished the implementation of techniques described in _Ray Tracing in One
Weekend_. The next goal is to implement acceleration structures and more shapes.

### Sources
- [Ray Tracing in One Weekend](https://github.com/petershirley/raytracinginoneweekend)
- [Ray Tracing: The Next Week](https://github.com/petershirley/raytracingthenextweek)
- [Ray Tracing: The Rest of Your Life](https://github.com/petershirley/raytracingtherestofyourlife)
- [Physically-Based Rendering](http://www.pbr-book.org/3ed-2018/contents.html)
- [scratchapixel.com](scratchapixel.com)
