#include <iostream>
#include "ray.h"

vec3 color(const ray& r)
{
    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);    
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

    // Set color information for each pixel
    for (int j = height - 1; j >= 0; j--)
    {
        for (int i = 0; i < length; i++)
        {
            // Components of vector
            float u = float(i) / float(length);
            float v = float(j) / float(height);
            ray r(origin, lower_left_corner + u * horizontal + v * vertical);
            vec3 col = color(r);

            // Convert normalized color to full color
            int ir = int(255.99 * col.r());
            int ig = int(255.99 * col.g());
            int ib = int(255.99 * col.b());

            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}
