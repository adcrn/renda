// Author: Aleander Decurnou
// Email: hi@lxnd.de
// Main driver for raytracer

#include <fstream>
#include <future>
#include <iostream>
#include <random>
#include <sstream>
#include <thread>
#include <vector>
#include "camera.h"
#include "float.h"
#include "hitable_list.h"
#include "ray.h"
#include "sphere.h"

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

int main(int argc, char** argv)
{
    int chunk_size = atoi(argv[1]);
    int width = 200;
    int height = 100;
    int num_samples = 100;

    int data[height][width];
    // Acts as a limit for the multithread loop
    std::size_t max = width * height;

    // cores is the number of concurrent threads supported
    std::size_t cores = std::thread::hardware_concurrency();
    std::vector<std::future<void>> future_vector;
    volatile std::atomic<std::size_t> count(0);

    // Header for the PPM file
    std::cout << "P3\n" << width << " " << height << "\n255\n";

    // Add objects to scene
    hitable *l[4];
    l[0] = new sphere(vec3(0, 0, -1), 0.5, new diffuse(vec3(0.8, 0.3, 0.3)));
    l[1] = new sphere(vec3(0, -100.5, -1), 100, new diffuse(vec3(0.8, 0.8, 0.0)));
    l[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
    l[3] = new sphere(vec3(-1, 0, -1), 0.5, new metal(vec3(0.8, 0.8, 0.8), 1.0));
    hitable *world = new hitable_list(l, 4);

    // Camera to view scene
    camera cam;

    std::string output_filename = "test.ppm";

    // For each thread that the system can support
    while (cores--)
    {
        future_vector.emplace_back(
            std::async(std::launch::async, [=, &count]()
            {
                // Thread-local PRNG for the averaging step
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_real_distribution<> dis(0.0, 1.0);

                // Each thread works on its own section of pixels
                while (count < max)
                {
                    std::size_t chunk_index = count;
                    count += chunk_size;
                    std::size_t chunk_end = chunk_index + chunk_size;

                    while (chunk_index < chunk_end)
                    {
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
                        int ir = int(255.99 * col.r());
                        int ig = int(255.99 * col.g());
                        int ib = int(255.99 * col.b());

                        // Using a stringstream to prevent interleaving writes
                        //std::stringstream ss;
                        //ss << ir << " " << ig << " " << ib << "\n";
                        //std::cout << ss.str();
                    }
                }
            }));
    }
}
