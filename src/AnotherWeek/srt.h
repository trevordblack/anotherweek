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
            mat4x4 o2w_scale = scale_matrix(sx, sy, sz);
            mat4x4 w2o_scale = scale_matrix(1/sx, 1/sy, 1/sz);

            mat4x4 o2w_rot_x = rotation_x_matrix(rx); 
            mat4x4 o2w_rot_y = rotation_y_matrix(ry); 
            mat4x4 o2w_rot_z = rotation_z_matrix(rz); 
            mat4x4 w2o_rot_x = rotation_x_matrix(-rx); 
            mat4x4 w2o_rot_y = rotation_y_matrix(-ry); 
            mat4x4 w2o_rot_z = rotation_z_matrix(-rz); 

            mat4x4 o2w_trans = translation_matrix(tx, ty, tz);
            mat4x4 w2o_trans = translation_matrix(-tx, -ty, -tz);

            o2w = o2w_scale * o2w_rot_x * o2w_rot_y * o2w_rot_z * o2w_trans;
            w2o = w2o_scale * w2o_rot_x * w2o_rot_y * w2o_rot_z * w2o_trans;
        }

        vec3 vec_object_to_world(const vec3 &v);

        vec3 vec_world_to_object(const vec3 &v);
        vec3 point_world_to_object(const vec3 &p);
        ray  ray_world_to_object(const ray  &r);

        mat4x4 o2w;
        mat4x4 w2o;
};

vec3 srt::vec_object_to_world(const vec3 &v) {
    return o2w * v;    
}

vec3 srt::vec_world_to_object(const vec3 &v) {
    return w2o * v;    
}

vec3 srt::point_world_to_object(const vec3 &p) {
    vec4 new_p = w2o * vec4(p.e[0], p.e[1], p.e[2], 1.0);
    return vec3(new_p.e[0] / new_p.e[3],
                new_p.e[1] / new_p.e[3],
                new_p.e[2] / new_p.e[3]);
}

ray srt::ray_world_to_object(const ray &r) {
    ray new_ray;
    new_ray.tm = r.tm;
    new_ray.orig = point_world_to_object(r.orig);
    new_ray.dir = vec_world_to_object(r.dir);
    return new_ray;
}

#endif
