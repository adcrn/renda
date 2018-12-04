# renda
An exercise in raytracing.

## Compile
Single-threaded: `g++ -std=c++11 -O3 src/main_single.cpp -o renda_single`

Multi-threaded: `g++ -std=c++11 -O3 src/main.cpp -o renda`

## Use
Single-threaded: `./renda_single`

Multi-threaded: `./renda`

## Notes
Currently, the multi-threaded version is _at least_ ~3x faster than its
single-threaded counterpart (at least by my own testing). The following
1920x1080 image at 1000 samples per pixel took ~64 minutes to render on a 3.2 GHz Intel Core i5 processor.

![Lots and lots of spheres](https://i.imgur.com/zhEihdD.jpg)

### Times
`main.cpp`, 500 spheres

| System                 | 3.2 GHz Desktop| 2.7GHz Macbook  |
| ---------------------- |:--------------:| ---------------:|
| 1920x1080, 1000 samples| 64 minutes     | _untested_      |
| 1200x800, 50 samples   | 121 seconds    | 169 seconds     |

`main_simple.cpp`, 500 spheres

| System                 | 3.2 GHz Desktop| 2.7GHz Macbook  |
| ---------------------- |:--------------:| ---------------:|
| 1920x1080, 1000 samples| _untested_     | _untested_      |
| 1200x800, 50 samples   | 514 seconds    | 589 seconds     |

### Fixes for Potential Issues
The program may terminate because `drand48()` creates "too many open
files" with `/dev/urandom`. Running `ulimit -n [big_number]` will fix the
issue.

The program initially used `std::async` to generate a promise/future for each
pixel that needed to be traced. Early work resulted in interleaved writes to
`std::cout`, which would lead to a scrambled image upon piping stdout to a PPM
image. Futures that operated on an image array were then stored in a vector to work within the constraints of the `std::async` paradigm. Issues related to this implementation were deemed more trouble than they were worth and the program was moved to a more sensible `std::thread` implementation instead.

### Current Progress
Finished the implementation of techniques described in _Ray Tracing in One
Weekend_. The next goal is to implement acceleration structures and more shapes.

### Sources
- [Ray Tracing in One Weekend](https://github.com/petershirley/raytracinginoneweekend)
- [Ray Tracing: The Next Week](https://github.com/petershirley/raytracingthenextweek)
- [Ray Tracing: The Rest of Your Life](https://github.com/petershirley/raytracingtherestofyourlife)
- [Physically-Based Rendering](http://www.pbr-book.org/3ed-2018/contents.html)
- [scratchapixel.com](scratchapixel.com)
