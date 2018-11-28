// Author: Aleander Decurnou
// Email: hi@lxnd.de
// Main driver for raytracer

#include <fstream>
#include <future>
#include <iostream>
#include <random>
#include <sstream>
#include <stdio.h>
#include <thread>
#include <vector>
#include "camera.h"
#include "float.h"
#include "hitable_list.h"
#include "ray.h"
#include "sphere.h"

typedef struct RGBPixel
{
    int red;
    int green;
    int blue;
} RGBPixel;

hitable* random_scene()
{
    // Random generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0, 1.0);

    // Initialize list and scene floor.
    int n = 500;
    hitable **l = new hitable*[n+1];
    l[0] = new sphere(vec3(0, -1000, 0), 1000, new diffuse(vec3(0.5, 0.5, 0.5)));
    
    int i = 1;
    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            float choose_material = dis(gen);
            vec3 center(a + 0.9 * dis(gen), 0.2, b + 0.9 * dis(gen));
            if ((center - vec3(4, 0.2, 0)).length() > 0.9)
            {
                if (choose_material < 0.8)
                {
                    l[i++] = new sphere(center, 0.2, new diffuse(vec3(dis(gen) * dis(gen), dis(gen) * dis(gen), dis(gen) * dis(gen))));
                }
                else if (choose_material < 0.95)
                {
                    l[i++] = new sphere(center, 0.2, new metal(vec3(0.5 * (1 + dis(gen)), 0.5 * (1 + dis(gen)), 0.5 * (1 + dis(gen))), 0.5 * dis(gen)));
                }
                else
                {
                    l[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }

    l[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    l[i++] = new sphere(vec3(-4, 1, 0), 1.0, new diffuse(vec3(0.4, 0.2, 0.1)));
    l[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

    return new hitable_list(l, i);
}

vec3 color(const ray& r, hitable *world, int depth) 
{
    hit_record rec;
    if (world->hit(r, 0.001, MAXFLOAT, rec))
    {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        {
            return attenuation * color(scattered, world, depth + 1);
        }
        else
        {
            return vec3(0,0,0);
        }
    }
    else
    {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);    
    }
}

void writePPMFile(std::vector<std::future<void>>& fut_vec, RGBPixel* data, int width, int height, int max, std::string filename)
{
    // Fulfill all promises inside of future vector,
    // guaranteed to be in proper order (non-interleaved)
    for (auto &e: fut_vec)
    {
        e.get();
    }

    std::ofstream output;
    output.open(filename);

    // Header for the PPM file
    output << "P3\n" << width << " " << height << "\n255\n";

    // Print out RGB values for each pixel
    for (int i = 0; i < max; i++)
    {
        output << data[i].red << " " << data[i].green << " " << data[i].blue << "\n";
    }

    output.close();
}

int main(int argc, char** argv)
{
    std::cout << "renda: version 0.1" << "\n";
    std::cout << "=====================" << "\n";
    std::cout << "Thread chunk size: " << argv[1] << "\n";
    std::cout << "Image will be written to test.ppm" << "\n";
    std::cout << "=====================" << "\n";

    // Allow for user to define how many pixels should be
    // rendered at a time per thread; should be an even divisor 
    int chunk_size = atoi(argv[1]);

    const int width = 1200;
    const int height = 800;
    int num_samples = 50;

    // Acts as a limit for the multithread loop
    std::size_t max = width * height;
    RGBPixel* data = new RGBPixel[max];

    // cores is the number of concurrent threads supported
    std::size_t cores = std::thread::hardware_concurrency();
    std::vector<std::future<void>> future_vector;
    volatile std::atomic<std::size_t> count(0);

    hitable *world = random_scene();

    // Camera to view scene
    vec3 lookfrom(13, 2, 3);
    vec3 lookat(0, 0, 0);
    float dist_to_focus = 10.0;
    float aperture = 0.1;
    camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(width)/float(height), aperture, dist_to_focus);

    std::string output_filename = "test.ppm";

    // For each thread that the system can support
    while (cores--)
    {
        // Thread-local PRNG for the averaging step
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(0.0, 1.0);

        // Each pixel will be calculated
        while (count < max)
        {
            // Launch each pixel calculation as an asynchronous task
            // whose results will be fetched later.
            future_vector.emplace_back(
                std::async(std::launch::async, [=, &count, &gen, &dis]()
                {
                    // Get and update atomic counter at top of task so
                    // other threads don't use unreliable values
                    std::size_t chunk_index = count;
                    count += chunk_size;
                    std::size_t chunk_end = chunk_index + chunk_size;

                    // Threads will calculate chunk_size pixels at a time
                    // in order to fairly spread the work between cores
                    while (chunk_index < chunk_end)
                    {
                        int current_pixel = chunk_index;
                        
                        // The idea here is that the desired pixel on a row
                        // will always be less than the length of the row and
                        // that after going through one whole row, y will floor
                        // to the next number.
                        int x = chunk_index % width;
                        int y = chunk_index / width;
                        chunk_index++;

                        vec3 col(0, 0, 0);

                        // Average over the amount of samples
                        for (int s = 0; s < num_samples; s++)
                        {
                            float u = float(x + dis(gen)) / float(width);
                            float v = float(y + dis(gen)) / float(height);

                            ray r = cam.get_ray(u, v);
                            col += color(r, world, 0);
                        }

                        col /= float(num_samples);
                        col = vec3(sqrt(col.r()), sqrt(col.g()), sqrt(col.b()));

                        // Convert normalized color to full color
                        data[current_pixel].red = int(255.99 * col.r());
                        data[current_pixel].green = int(255.99 * col.g());
                        data[current_pixel].blue = int(255.99 * col.b());
                    }
                }));
        }
    }

    writePPMFile(future_vector, data, width, height, max, output_filename);
    delete[] data;
}
