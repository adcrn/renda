#include <iostream>

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
            // Normalize each of the color components
            float r = float(i) / float(length);
            float g = float(j) / float(height);
            float b = 0.2;

            // Convert normalized color to full color
            int ir = int(255.99 * r);
            int ig = int(255.99 * g);
            int ib = int(255.99 * b);

            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}
