#ifndef QUAD_H
#define QUAD_H
//==============================================================================================
// Originally written in 2016 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication
// along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==============================================================================================

#include "rtweekend.h"

#include "hittable.h"
#include "srt.h"


class quad : public hittable {
    public:
        quad() {}
        quad(shared_ptr<material> m_ptr) : mat_ptr(m_ptr) {}
        quad(shared_ptr<material> m_ptr, shared_ptr<srt> s_ptr) :
            mat_ptr(m_ptr), 
            srt_ptr(s_ptr) {}

        virtual bool hit(const ray& r, double t0, double t1, hit_record& rec) const;
        virtual bool bounding_box(double t0, double t1, aabb& output_box) const;
        shared_ptr<srt>       srt_ptr;
        shared_ptr<material>  mat_ptr;
};

bool quad::bounding_box(double t0, double t1, aabb& output_box) const {
    vec3 v0 = srt_ptr->vec_object_to_world(vec3(-0.5, -0.5, 0.0));
    vec3 v1 = srt_ptr->vec_object_to_world(vec3(-0.5,  0.5, 0.0));
    vec3 v2 = srt_ptr->vec_object_to_world(vec3( 0.5,  0.5, 0.0));
    vec3 v3 = srt_ptr->vec_object_to_world(vec3( 0.5, -0.5, 0.0));

    float x_min = ffmin(ffmin(v0.e[0], v1.e[0]), ffmin(v2.e[0], v3.e[0]));
    float y_min = ffmin(ffmin(v0.e[1], v1.e[1]), ffmin(v2.e[1], v3.e[1]));
    float z_min = ffmin(ffmin(v0.e[2], v1.e[2]), ffmin(v2.e[2], v3.e[2]));
    float x_max = ffmax(ffmax(v0.e[0], v1.e[0]), ffmax(v2.e[0], v3.e[0]));
    float y_max = ffmax(ffmax(v0.e[1], v1.e[1]), ffmax(v2.e[1], v3.e[1]));
    float z_max = ffmax(ffmax(v0.e[2], v1.e[2]), ffmax(v2.e[2], v3.e[2]));
    output_box = aabb(vec3(x_min - 0.001, y_min - 0.001, z_min - 0.001),
                      vec3(x_max + 0.001, y_max + 0.001, z_max + 0.001));
    return true;
}

bool quad::hit(const ray& r, double t0, double t1, hit_record& rec) const {
    ray r_o = srt_ptr->ray_world_to_object(r);

    auto t = -r_o.origin().z() / r_o.direction().z();
    if (t < t0 || t > t1)
        return false;

    auto x_o = r_o.origin().x() + t*r_o.direction().x();
    auto y_o = r_o.origin().y() + t*r_o.direction().y();
    if (x_o < -0.5 || x_o > 0.5 || y_o < -0.5 || y_o > 0.5)
        return false;
    
    rec.u = x_o + 0.5;
    rec.v = y_o + 0.5;
    rec.t = t;
    vec3 outward_normal = srt_ptr->vec_object_to_world(vec3(0, 0, 1));
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;
    rec.p = r.at(t);

    return true;
}

#endif
