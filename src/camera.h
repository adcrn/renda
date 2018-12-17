#ifndef CAMERAH
#define CAMERAH
#include "ray.h"

vec3 random_in_unit_disk()
{
    vec3 p;
    do
    {
        p = 2.0 * vec3(drand48(), drand48(), 0) - vec3(1,1,0);
    } while (dot(p, p) >= 1.0);

    return p;
}

class camera
{
    public:
        camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect,
               float aperture, float focus_dist, float t0, float t1)
        {
            // Shutter open and close
            time0 = t0;
            time1 = t1;

            // Lens and vertical field of view
            lens_radius = aperture / 2;
            float theta = vfov * M_PI / 180;
            float half_height = tan(theta / 2);
            float half_width = aspect * half_height;
            
            // Camera coordinate system
            origin = lookfrom;
            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);
            upper_left_corner = vec3(-half_width, half_height, -1.0);
            upper_left_corner = origin - focus_dist * half_width * u + focus_dist * half_height * v - focus_dist * w;
            horizontal = 2 * half_width * focus_dist * u;
            vertical = 2 * half_height * focus_dist * v;
        }

        ray get_ray(float s, float t) const
        {
            vec3 rd = lens_radius * random_in_unit_disk();
            vec3 offset = u * rd.x() + v * rd.y();
            float time = time0 + drand48() * (time1 - time0);
            return ray(origin + offset, upper_left_corner + s * horizontal - t * vertical - origin - offset - time);
        }

        vec3 u, v, w;
        vec3 upper_left_corner;
        vec3 horizontal;
        vec3 vertical;
        vec3 origin;
        float lens_radius;
        float time0, time1;
};

#endif
