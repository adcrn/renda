#ifndef GEOMETRY
#define GEOMETRY

#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>

class vec2
{
    public:
    
        // Empty constructor
        vec2() {}

        // Define dimensional attributes
        vec2(float e0, float e1) { e[0] = e0; e[1] = e1; }

        // Getters for vector magnitudes
        // Note: the "inline" keyword tells the compiler to replace the function
        // call for this function/method with the body of the function itself in
        // order to reduce overhead when calling it.
        inline float x() const { return e[0]; }
        inline float y() const { return e[1]; }

        // Vector operations
        inline const vec2& operator+() const { return *this; }
        inline vec2 operator-() const { return vec2(-e[0], -e[1]); }
        inline float operator[](int i) const { return e[i]; }
        inline float& operator[](int i) { return e[i]; }

        inline vec2 operator+ (const vec2 &v) { return vec2(e[0] + v.e[0], e[1] + v.e[1]); }
        inline vec2 operator/ (const float t) { return vec2(e[0] / t, e[1] / t); }
        inline vec2 operator* (const float t) { return vec2(e[0] * t, e[1] * t); }
        inline vec2& operator/= (const float t) { e[0] /= t; e[1] /= t; return *this; };
        inline vec2& operator*= (const float t) { e[0] *= t; e[1] *= t; return *this; };

        inline float length() const
        {
            return sqrt(e[0]*e[0] + e[1]*e[1]);
        }

        inline float squared_length() const
        {
            return e[0]*e[0] + e[1]*e[1];
        }

        inline void make_unit_vector();

        float e[2];
};

inline std::istream& operator>>(std::istream &is, vec2 &t)
{
    is >> t.e[0] >> t.e[1];
    return is;
}

inline std::ostream& operator<<(std::ostream &os, vec2 &t)
{
    os << t.e[0] << t.e[1];
    return os;
}

class vec3
{
    public:
    
        // Empty constructor
        vec3() {}

        // Define dimensional attributes
        vec3(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }

        // Getters for vector magnitudes
        // Note: the "inline" keyword tells the compiler to replace the function
        // call for this function/method with the body of the function itself in
        // order to reduce overhead when calling it.
        inline float x() const { return e[0]; }
        inline float y() const { return e[1]; }
        inline float z() const { return e[2]; }
        inline float r() const { return e[0]; }
        inline float g() const { return e[1]; }
        inline float b() const { return e[2]; }

        // Vector operations
        inline const vec3& operator+() const { return *this; }
        inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
        inline float operator[](int i) const { return e[i]; }
        inline float& operator[](int i) { return e[i]; }

        inline vec3& operator+=(const vec3 &v2);
        inline vec3& operator-=(const vec3 &v2);
        inline vec3& operator*=(const vec3 &v2);
        inline vec3& operator/=(const vec3 &v2);
        inline vec3& operator*=(const float t);
        inline vec3& operator/=(const float t);

        inline float length() const
        {
            return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
        }

        inline float squared_length() const
        {
            return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }

        inline void make_unit_vector();

        inline vec3& normalize()
        {
            float n = squared_length();

            if (n > 0)
            {
                float factor = 1 / sqrt(n);
                e[0] *= factor, e[1] *= factor, e[2] *= factor;
            }

            return *this;
        }

        float e[3];
};

inline std::istream& operator>>(std::istream &is, vec3 &t)
{
    is >> t.e[0] >> t.e[1] >> t.e[2];
    return is;
}

inline std::ostream& operator<<(std::ostream &os, vec3 &t)
{
    os << t.e[0] << t.e[1] << t.e[2];
    return os;
}

inline void vec3::make_unit_vector()
{
    float k = 1.0 / sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
    e[0] *= k;
    e[1] *= k;
    e[2] *= k;
}

inline vec3 operator+(const vec3 &v1, const vec3 &v2)
{
    return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline vec3 operator-(const vec3 &v1, const vec3 &v2)
{
    return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline vec3 operator*(const vec3 &v1, const vec3 &v2)
{
    return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline vec3 operator/(const vec3 &v1, const vec3 &v2)
{
    return vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline vec3 operator*(float t, const vec3 &v)
{
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator/(const vec3 &v, float t)
{
    return vec3(v.e[0] / t, v.e[1] / t, v.e[2] / t);
}

inline vec3 operator*(const vec3 &v, float t)
{
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline float dot(const vec3 &v1, const vec3 &v2)
{
    return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline vec3 cross(const vec3 &v1, const vec3 &v2)
{
    return vec3( (v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]),
               (-(v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0])),
                 (v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]));
}

inline vec3& vec3::operator+=(const vec3 &v)
{
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
}


inline vec3& vec3::operator-=(const vec3 &v)
{
    e[0] -= v.e[0];
    e[1] -= v.e[1];
    e[2] -= v.e[2];
    return *this;
}

inline vec3& vec3::operator*=(const vec3 &v)
{
    e[0] *= v.e[0];
    e[1] *= v.e[1];
    e[2] *= v.e[2];
    return *this;
}

inline vec3& vec3::operator/=(const vec3 &v)
{
    e[0] /= v.e[0];
    e[1] /= v.e[1];
    e[2] /= v.e[2];
    return *this;
}

inline vec3& vec3::operator*=(const float t)
{
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
}

inline vec3& vec3::operator/=(const float t)
{
    float k = 1.0 / t;
    e[0] *= k;
    e[1] *= k;
    e[2] *= k;
    return *this;
}

inline vec3 unit_vector(vec3 v)
{
    return v / v.length();
}

#endif
