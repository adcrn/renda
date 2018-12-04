#include <iostream>
#include <random>
#include "camera.h"
#include "float.h"
#include "hitable_list.h"
#include "ray.h"
#include "sphere.h"

hitable* random_scene()
{
    // Initialize list and scene floor.
    int n = 500;
    hitable **l = new hitable*[n+1];
    l[0] = new sphere(vec3(0, -1000, 0), 1000, new diffuse(vec3(0.5, 0.5, 0.5)));
    
    int i = 1;
    for (int a = -11; a < 11; a++)
    {
        for (int b = -11; b < 11; b++)
        {
            float choose_material = drand48();
            vec3 center(a + 0.9 * drand48(), 0.2, b + 0.9 * drand48());
            if ((center - vec3(4, 0.2, 0)).length() > 0.9)
            {
                if (choose_material < 0.8)
                {
                    l[i++] = new sphere(center, 0.2, new diffuse(vec3(drand48() * drand48(), drand48() * drand48(), drand48() * drand48())));
                }
                else if (choose_material < 0.95)
                {
                    l[i++] = new sphere(center, 0.2, new metal(vec3(0.5 * (1 + drand48()), 0.5 * (1 + drand48()), 0.5 * (1 + drand48())), 0.5 * drand48()));
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

int main()
{
    int width = 1200;
    int height = 800;
    int num_samples = 50;

    // Header for the PPM file
    std::cout << "P3\n" << width << " " << height << "\n255\n";

    hitable *world = random_scene();

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
                float u = float(i + drand48()) / float(width);
                float v = float(j + drand48()) / float(height);

                ray r = cam.get_ray(u, v);
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
