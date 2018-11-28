# renda
An exercise in raytracing.

## Compile
Single-threaded: `g++ -std=c++11 src/main_single.cpp -o renda_single`

Multi-threaded: `g++ -std=c++11 src/main.cpp -o renda`

## Use
Single-threaded: `./renda_single`

Multi-threaded: `./renda [thread_chunk_size]`

Note: The thread chunk size allows the user to define a certain amount of contiguous
pixels to assign to a thread so that each core does equal work.

## Notes
Currently, the multi-threaded version is ~2.4-2.6x faster than its
single-threaded counterpart (at least by my own testing). The following
1200x800 image took ~33 minutes to render with 50 samples per pixel at a chunk size of 40 pixels on a 2017 MacBook Pro 2.3 GHz Intel Core i5 processor.

![Lots and lots of spheres](https://i.imgur.com/2OdwWIk.jpg)

### Fixes for Potential Issues
The program may terminate because `std::random_engine` creates "too many open
files" with `/dev/urandom`. Running `ulimit -n [big_number]` will fix the
issue.

### Current Progress
Finished the implementation of techniques described in _Ray Tracing in One
Weekend_. The net goal is to implement acceleration structures and more shapes.

### Sources
- [Ray Tracing in One Weekend](https://github.com/petershirley/raytracinginoneweekend)
- [Ray Tracing: The Next Week](https://github.com/petershirley/raytracingthenextweek)
- [Ray Tracing: The Rest of Your Life](https://github.com/petershirley/raytracingtherestofyourlife)
- [Physically-Based Rendering](http://www.pbr-book.org/3ed-2018/contents.html)
- [scratchapixel.com](scratchapixel.com)
