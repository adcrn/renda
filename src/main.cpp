#include <iostream>
#include "vec3.h"

int main()
{
    int length = 200;
    int height = 100;

    // Header for the PPM file
    std::cout << "P3\n" << length << " " << height << "\n255\n";

    // Set color information for each pixel
    for (int j = height - 1; j >= 0; j--)
    {
        for (int i = 0; i < length; i++)
        {
            vec3 col(float(i) / float(length), float(j) / float(height), 0.2);

            // Convert normalized color to full color
            int ir = int(255.99 * col.r());
            int ig = int(255.99 * col.g());
            int ib = int(255.99 * col.b());

            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}
