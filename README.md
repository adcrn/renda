# renda
A toy raytracer.

Currently, I'm going through Peter Shirley's Ray Tracing series, but hope to
extend the raytracer myself with no guidance.

## Compile
`g++ -std=c++11 src/main.cpp -o renda`

## Use
`./renda [thread_chunk_size]`

The thread chunk size allows the user to figure out which amount of contiguous
pixels to assign to a thread so that each core does equal work.
