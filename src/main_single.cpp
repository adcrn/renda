#include <iostream>
#include <random>
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

int main()
{
    //Will be used to obtain a seed for the random number engine
    std::random_device rd;

    //Standard mersenne_twister_engine seeded with rd()
    std::mt19937 gen(rd());
    
    std::uniform_real_distribution<float> blah(0.0, 1.0);

    int width = 200;
    int height = 100;
    int num_samples = 100;

    // Header for the PPM file
    std::cout << "P3\n" << width << " " << height << "\n255\n";

    // Add objects to scene
    hitable *l[5];
    l[0] = new sphere(vec3(0, 0, -1), 0.5, new diffuse(vec3(0.1, 0.2, 0.5)));
    l[1] = new sphere(vec3(0, -100.5, -1), 100, new diffuse(vec3(0.8, 0.8, 0.0)));
    l[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
    l[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
    l[4] = new sphere(vec3(-1, 0, -1), -0.45, new dielectric(1.5));
    hitable *world = new hitable_list(l, 5);

    // Camera to view scene
    vec3 lookfrom(3, 3, 2);
    vec3 lookat(0, 0, -1);
    float dist_to_focus = (lookfrom - lookat).length();
    float aperture = 2.0;
    camera cam(lookfrom, lookat, vec3(0, 1, 0), 20, float(width)/float(height), aperture, dist_to_focus);

    // Set color information for each pixel
    for (int j = height - 1; j >= 0; j--)
    {
        for (int i = 0; i < width; i++)
        {
            vec3 col(0, 0, 0);
           
            // Average over the amount of samples
            for (int s = 0; s < num_samples; s++)
            {
                float u = float(i + blah(gen)) / float(width);
                float v = float(j + blah(gen)) / float(height);

                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_parameter(2.0);
                col += color(r, world, 0);
            }

            col /= float(num_samples);
            col = vec3(sqrt(col.r()), sqrt(col.g()), sqrt(col.b()));

            // Convert normalized color to full color
            int ir = int(255.99 * col.r());
            int ig = int(255.99 * col.g());
            int ib = int(255.99 * col.b());

            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}
