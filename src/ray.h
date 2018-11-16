#ifndef RAYH
#define RAYH
#include "vec3.h"

class ray
{
    public:
        ray() {}
        ray(const vec3& a, const vec3& b) { A = a; B = b; }
        
        // Start of ray
        vec3 origin() const { return A; }
        
        // Could be a unit length vector, might look into this later
        vec3 direction() const { return B; }

        // Ray function
        vec3 point_at_parameter(float t) const { return A + t*B; }

        vec3 A;
        vec3 B;
};

#endif
