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
    //
};

class cube : public hitable
{
    //
};

#endif
