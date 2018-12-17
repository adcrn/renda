#ifndef RAYH
#define RAYH
#include "vec3.h"

class ray
{
    public:
        ray() {}
        ray(const vec3& a, const vec3& b, float t_i) { A = a; B = b; _time = t_i;}
        
        // Start of ray
        vec3 origin() const { return A; }
        
        // Could be a unit length vector, might look into this later
        vec3 direction() const { return B; }

        // Time interval of camera shutter
        float time() const { return _time; }

        // Ray function
        vec3 point_at_parameter(float t) const { return A + t*B; }

        vec3 A;
        vec3 B;
        float _time;
};

#endif
