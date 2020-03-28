#ifndef SRT_H
#define SRT_H

#include "mat4x4.h"
#include "vec4.h"
#include "common/vec3.h"
#include "common/ray.h"

mat4x4 scale_matrix(double sx, double sy, double sz)
{
    return mat4x4(sx,  0,  0, 0,
                   0, sy,  0, 0,
                   0,  0, sz, 0,
                   0,  0,  0, 1);
}

mat4x4 translation_matrix(double tx, double ty, double tz)
{
    return mat4x4(1, 0, 0, tx,
                  0, 1, 0, ty,
                  0, 0, 1, tz,
                  0, 0, 0,  1);
}

mat4x4 rotation_x_matrix(double theta)
{
    double c = cos(degrees_to_radians(theta));
    double s = sin(degrees_to_radians(theta));  
    return mat4x4(1, 0,  0, 0,
                  0, c, -s, 0,
                  0, s,  c, 0,
                  0, 0,  0, 1);
}

mat4x4 rotation_y_matrix(double theta)
{
    double c = cos(degrees_to_radians(theta));
    double s = sin(degrees_to_radians(theta));  
    return mat4x4( c, 0, s, 0,
                   0, 1, 0, 0,
                  -s, 0, c, 0,
                   0, 0, 0, 1);
}

mat4x4 rotation_z_matrix(double theta)
{
    double c = cos(degrees_to_radians(theta));
    double s = sin(degrees_to_radians(theta));  
    return mat4x4(c, -s, 0, 0,
                  s,  c, 0, 0,
                  0,  0, 1, 0,
                  0,  0, 0, 1);
}

class srt {
    public:
        srt() {}
        srt(double sx, double sy, double sz,
            double rx, double ry, double rz,
            double tx, double ty, double tz)
        {
            mat4x4 scale = scale_matrix(1/sx, 1/sy, 1/sz);
            mat4x4 rot_x = rotation_x_matrix(-rx); 
            mat4x4 rot_y = rotation_y_matrix(-ry); 
            mat4x4 rot_z = rotation_z_matrix(-rz); 
            mat4x4 trans = translation_matrix(-tx, -ty, -tz);

            m = scale * rot_x * rot_y * rot_z * trans;
        }

        vec3 transform_vec(const vec3 &v);
        ray  transform_ray(const ray  &r);
        vec3  transform_point(const vec3 &p);

        mat4x4 m;
};

vec3 srt::transform_vec(const vec3 &v) {
    return m*v;    
}

vec3 srt::transform_point(const vec3 &p) {
    vec4 new_p = m*vec4(p.e[0], p.e[1], p.e[2], 1.0);
    return vec3(new_p.e[0] / new_p.e[3],
                new_p.e[1] / new_p.e[3],
                new_p.e[2] / new_p.e[3]);
}

ray srt::transform_ray(const ray &r) {
    ray new_ray;
    new_ray.tm = r.tm;
    new_ray.orig = transform_point(r.orig);
    new_ray.dir = transform_vec(r.dir);
    return new_ray;
}

#endif
