#include <iostream>
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
    int length = 200;
    int height = 100;

    // Header for the PPM file
    std::cout << "P3\n" << length << " " << height << "\n255\n";

    vec3 lower_left_corner(-2.0, -1.0, -1.0);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);


    hitable *l[2];
    l[0] = new sphere(vec3(0, 0, -1), 0.5);
    l[1] = new sphere(vec3(0, -100.5, -1), 100);
    hitable *world = new hitable_list(l, 2);

    // Set color information for each pixel
    for (int j = height - 1; j >= 0; j--)
    {
        for (int i = 0; i < length; i++)
        {
            // Components of vector
            float u = float(i) / float(length);
            float v = float(j) / float(height);
            ray r(origin, lower_left_corner + u * horizontal + v * vertical);
            
            vec3 col = color(r, world);

            // Convert normalized color to full color
            int ir = int(255.99 * col.r());
            int ig = int(255.99 * col.g());
            int ib = int(255.99 * col.b());

            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}
