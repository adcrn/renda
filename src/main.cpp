#include <iostream>
#include <random>
#include "camera.h"
#include "float.h"
#include "hitable_list.h"
#include "ray.h"
#include "sphere.h"

vec3 color(const ray& r, hitable *world)
{
    hit_record rec;
    if (world->hit(r, 0.0, MAXFLOAT, rec))
    {
        return 0.5 * vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
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

    int length = 200;
    int height = 100;
    int num_samples = 100;

    // Header for the PPM file
    std::cout << "P3\n" << length << " " << height << "\n255\n";

    // Add objects to scene
    hitable *l[2];
    l[0] = new sphere(vec3(0, 0, -1), 0.5);
    l[1] = new sphere(vec3(0, -100.5, -1), 100);
    hitable *world = new hitable_list(l, 2);

    // Camera to view scene
    camera cam;

    // Set color information for each pixel
    for (int j = height - 1; j >= 0; j--)
    {
        for (int i = 0; i < length; i++)
        {
            vec3 col(0, 0, 0);
           
            // Average over the amount of samples
            for (int s = 0; s < num_samples; s++)
            {
                float u = float(i + blah(gen)) / float(length);
                float v = float(j + blah(gen)) / float(height);

                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_parameter(2.0);
                col += color(r, world);
            }

            col /= float(num_samples);

            // Convert normalized color to full color
            int ir = int(255.99 * col.r());
            int ig = int(255.99 * col.g());
            int ib = int(255.99 * col.b());

            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}
