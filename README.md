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
single-threaded counterpart (at least by my own testing).

### Current Progress
I've completed the first book in Peter Shirley's _Ray Tracing_ series. The only
issue with the multi-threaded version right now is that it seems to open "too
many files" to `/dev/urandom`, which is what `std::random_device` uses.

### Sources
- [Ray Tracing in One Weekend](https://github.com/petershirley/raytracinginoneweekend)
- [Ray Tracing: The Next Week](https://github.com/petershirley/raytracingthenextweek)
- [Ray Tracing: The Rest of Your Life](https://github.com/petershirley/raytracingtherestofyourlife)
- [Physically-Based Rendering](http://www.pbr-book.org/3ed-2018/contents.html)
- [scratchapixel.com](scratchapixel.com)
