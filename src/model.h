#ifndef MODEL
#define MODEL

#include <stdlib.h>
#include <vector>

#include <iostream>
#include <fstream>
#include <sstream>

#include "geometry.h"
#include "ray.h"
#include "hitable.h"
#include "material.h"
#include "shape.h"

class Model : public hitable
{
    public:
        Model() {}
        Model(std::string file, material *m)
        {
            material = m;

            std::ifstream in_file(file);
            std::string line, op;
            std::stringstream linestream;
            vec3 temp;

            float x, y, z;
            vec3 p0, p1, p2;

            while(std::getline(in_file, line))
            {
                linestream << line;
                linestream >> op;

                if (op == "v")
                {
                    linestream >> x >> y >> z;
                    vec3 temp(x, y, z);
                    vertices.push_back(temp);
                    
                }

                else if (op == "f")
                {
                    linestream >> p0 >> p1 >> p2;
                    faces.push_back(triangle(p0, p1, p2, m)); 
                }

                linestream.clear();
            }
        }

        bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const
        {
            vec3 normal(0, 0, 0);

            float t_hit = 10000000.0f;
            bool hit = false;

            for (auto f : faces)
            {
                if (f.hit(r, t_min, t_max, rec))
                {
                    if (rec.t < t_hit)
                    {
                        t_hit = rec.t;
                        normal = rec.normal;
                    }

                    hit = true;
                }
            }

            rec.t = t_hit;
            rec.normal = normal;
            rec.p = r.point_at_parameter(rec.t);

            return hit;
        }

        material *material;

        std::vector<vec3> vertices;
        std::vector<triangle> faces;

};

#endif
