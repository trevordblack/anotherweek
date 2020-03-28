#ifndef AARECT_H
#define AARECT_H
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

#include "common/rtweekend.h"
#include "hittable.h"
#include "srt.h"

class rect : public hittable {
    public:
        rect() {}
        rect(material* mat_ptr) : mp(mat_ptr) {}
        rect(material* mat_ptr, srt* srt0) : mp(mat_ptr) {
            srtp = srt0;
        }

        virtual bool hit(const ray& r, double t0, double t1, hit_record& rec) const;
        virtual bool bounding_box(double t0, double t1, aabb& output_box) const;
        srt       *srtp;
        material  *mp;
};

bool rect::bounding_box(double t0, double t1, aabb& output_box) const {
    vec3 v0 = srtp->transform_vec(vec3(-0.5, 0.0, -0.5));
    vec3 v1 = srtp->transform_vec(vec3(-0.5, 0.0,  0.5));
    vec3 v2 = srtp->transform_vec(vec3( 0.5, 0.0,  0.5));
    vec3 v3 = srtp->transform_vec(vec3( 0.5, 0.0, -0.5));

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

bool rect::hit(const ray& r, double t0, double t1, hit_record& rec) const {
    ray new_ray = srtp->transform_ray(r);
    auto t = -new_ray.origin().y() / new_ray.direction().y();
    if (t < t0 || t > t1)
        return false;
    auto x = new_ray.origin().x() + t*new_ray.direction().x();
    auto z = new_ray.origin().z() + t*new_ray.direction().z();
    if (x < -0.5 || x > 0.5 || z < -0.5 || z > 0.5)
        return false;
    rec.u = x + 0.5;
    rec.v = z + 0.5;
    rec.t = t;
    rec.mat_ptr = mp;
    rec.p = new_ray.at(t);
    rec.normal = srtp->transform_vec(vec3(0, 1, 0));
    return true;
}

#endif
