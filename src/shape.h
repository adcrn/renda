#ifndef SHAPEH
#define SHAPEH
#include "hitable.h"
#include "material.h"

class material;

/*class object : public hitable
{
    public:
        object() : color(drand48(), drand48(), drand48()) {}
        virtual ~object() {}
        virtual bool hit(const ray &, float tmin, float tmax, hit_record& rec) const = 0;
        virtual void get_surface_data(const vec3 &, vec3 &, vec2 &) const = 0;
        vec3 color;
};*/

class sphere : public hitable
{
    public:
        sphere() {}
        sphere(vec3 cen, float r, material *m) : center(cen), radius(r), mat_ptr(m) {};

        vec3 center;
        float radius;
        material *mat_ptr;

        bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const
        {
            vec3 oc = r.origin() - center;
            float a = dot(r.direction(), r.direction());
            float b = dot(oc, r.direction());
            float c = dot(oc, oc) - radius * radius;
            float discriminant = b * b - a * c;

            // If it does hit, check both sides of sphere
            if (discriminant > 0)
            {
                float temp = (-b - sqrt(b * b - a * c)) / a;
                if (temp < tmax && temp > tmin)
                {
                    rec.t = temp;
                    rec.p = r.point_at_parameter(rec.t);
                    rec.normal = (rec.p - center) / radius;
                    rec.mat_ptr = mat_ptr;
                    return true;
                }

                temp = (-b + sqrt(b * b - a * c)) / a;
                if (temp < tmax && temp > tmin)
                {
                    rec.t = temp;
                    rec.p = r.point_at_parameter(rec.t);
                    rec.normal = (rec.p - center) / radius;
                    rec.mat_ptr = mat_ptr;
                    return true;
                }
            }

            return false;
        }

        void get_surface_data(const vec3& point_hit, vec3& norm_point, vec2& text_coor) const
        {
            norm_point = point_hit - center;
            norm_point.normalize();

            text_coor.e[0] = (1 + atan2(norm_point.z(), norm_point.x()) / M_PI) * 0.5;
            text_coor.e[1] = acosf(norm_point.y()) / M_PI;
        }
};

class moving_sphere : public hitable
{
    public:
        moving_sphere() {}
        moving_sphere(vec3 cen0, vec3 cen1, float t0, float t1, float r, material *m) :
                      center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(m) {};

        vec3 center(float time) const;

        // Different locations at different time steps
        vec3 center0, center1;
        float time0, time1;
        
        float radius;
        material *mat_ptr;

        bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const
        {
            vec3 oc = r.origin() - center(r.time());
            float a = dot(r.direction(), r.direction());
            float b = dot(oc, r.direction());
            float c = dot(oc, oc) - radius * radius;
            float discriminant = b * b - a * c;

            // If it does hit, check both sides of sphere
            if (discriminant > 0)
            {
                float temp = (-b - sqrt(b * b - a * c)) / a;
                if (temp < tmax && temp > tmin)
                {
                    rec.t = temp;
                    rec.p = r.point_at_parameter(rec.t);
                    rec.normal = (rec.p - center(r.time())) / radius;
                    rec.mat_ptr = mat_ptr;
                    return true;
                }

                temp = (-b + sqrt(b * b - a * c)) / a;
                if (temp < tmax && temp > tmin)
                {
                    rec.t = temp;
                    rec.p = r.point_at_parameter(rec.t);
                    rec.normal = (rec.p - center(r.time())) / radius;
                    rec.mat_ptr = mat_ptr;
                    return true;
                }
            }

            return false;
        }

        void get_surface_data(const vec3& point_hit, vec3& norm_point, vec2& text_coor) const
        {
            norm_point = point_hit - (center1 - center0);
            norm_point.normalize();

            text_coor.e[0] = (1 + atan2(norm_point.z(), norm_point.x()) / M_PI) * 0.5;
            text_coor.e[1] = acosf(norm_point.y()) / M_PI;
        }
};

class triangle : public hitable
{
    public:
        triangle() {}
        triangle(vec3 p0, vec3 p1, vec3 p2, material *m) : p0(p0), p1(p1), p2(p2), mat_ptr(m)
        {
            v0v1 = p1 - p0;
            v0v2 = p2 - p0;
            surface_normal = cross(v0v1, v0v2).normalize();
        }

        bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const
        {
            vec3 p_vec = cross(r.direction(), v0v2);
            float det = dot(v0v1, p_vec);

            if (fabs(det) < 1e-8)
            {
                return false;
            }

            float inv_det = 1 / det;

            vec3 t_vec = r.origin() - p0;
            float u = dot(t_vec, p_vec) * inv_det;
            if (u < 0 || u > 1)
            {
                return false;
            }

            vec3 q_vec = cross(t_vec, v0v1);
            float v = dot(q_vec, r.direction()) * inv_det;
            if (v < 0 || u + v > 1)
            {
                return false;
            }

            //float t = dot(v0v2, q_vec) * inv_det;

            return true;
        }

        float area() const
        {
            //
        }
    
        vec3 p0, p1, p2;
        vec3 v0v1, v0v2;
        material *mat_ptr;
        vec3 surface_normal;
};

class cube : public hitable
{
    //
};

#endif
